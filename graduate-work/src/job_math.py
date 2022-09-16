import numpy as np
import scipy.stats as stats
import os
from dotenv import load_dotenv

import io_helper
import sensor_helper
import plot_helper
import file_io
import evaluator as eval
import BFThresholdMode
import BehaviorClass

config = io_helper.get_config()
load_dotenv()

GPU_LOAD_IDX = config['gpu_load_idx']
TOTAL_SENSORS_NUM = config['total_sensors_num']


def get_measure(sensor, borders):
    begin = borders[0]
    end = borders[1]

    wnd_size = config["wnd_size"]
    num_of_wnd = end - begin - wnd_size + 1

    if num_of_wnd <= 0:
        return []
    
    perc_arr = np.array([sensor[begin + wnd_idx : begin + wnd_idx + wnd_size] for wnd_idx in range(num_of_wnd)])
    qs = np.transpose(np.percentile(perc_arr, [25, 75], axis = 1))
    variation = np.array([(item[1] - item[0]) / 2 if item[1] + item[0] >= config["EPS"] else 0.0 for item in qs])

    return variation


def get_variation_of_behavior_classes(read_variation_from_cache, to_plot_hist = False):
    def get_class_variation(behavior_class):
        var = [[] for s in range(config['total_sensors_num'])]
        for task in behavior_class:
            borders = behavior_class[task]
            sensors = sensor_helper.get_sensors(task)

            for i in range(config['total_sensors_num']):
                sensor = sensors[i]
                for inn_borders in borders:
                    partial_var = get_measure(sensor, inn_borders)
                    var[i] = np.concatenate((var[i], partial_var))

        return var

    normal_var = []
    abnormal_var = []

    if read_variation_from_cache:
        normal_var = file_io.from_file(os.getenv('CACHE_NORM_PATH'))
        abnormal_var = file_io.from_file(os.getenv('CACHE_ABNORM_PATH'))
    else:
        class_normal = config["job_hang_borders"]["normal"]
        class_abnormal = config["job_hang_borders"]["abnormal"]

        normal_var = get_class_variation(class_normal)
        abnormal_var = get_class_variation(class_abnormal)

        file_io.to_file(os.getenv('CACHE_NORM_PATH'), normal_var)
        file_io.to_file(os.getenv('CACHE_ABNORM_PATH'), abnormal_var)

    if to_plot_hist:
        plot_helper.plot_histogram(abnormal_var, normal_var, -1)

    return [normal_var, abnormal_var]


def get_threshold(mode, read_variation_from_cache = True):
    [normal_var, abnormal_var] = get_variation_of_behavior_classes(read_variation_from_cache)

    thresholds = np.zeros(TOTAL_SENSORS_NUM)

    for sensor_idx in range(TOTAL_SENSORS_NUM):
        norm = normal_var[sensor_idx]
        abnorm = abnormal_var[sensor_idx]

        diff = np.median(norm) - np.median(abnorm) # must be > 0
        step = diff / 20

        if diff < 0:
            thresholds[sensor_idx] = -1
            continue

        thld = 0

        if mode == BFThresholdMode.PERCENT_OPTIMAL:
            min_func = 100
            val = np.median(abnorm)
            while val < np.median(norm):
                perc1 = stats.percentileofscore(abnorm, val)
                perc2 = stats.percentileofscore(norm, val)

                if -perc1 + perc2 < min_func:
                    min_func = -perc1 + perc2
                    thld = val

                val += step

        elif mode == BFThresholdMode.CUSTOM_LIKE:
            thld = np.median(abnorm) + 18 * step

        thresholds[sensor_idx] = thld

    return thresholds


def get_job_hang_flags(sensors, threshold, check_cold_start = False):
    critical_inactive_size = config['critical_inactive_size']
    timeline_length = sensors.shape[1] - config['job_tail']
    active_wnd_num = (
        config['job_hang_active_wnd_num'],
        config['cold_start_active_wnd_num']
    )[check_cold_start]

    borders_active = (
        [0, timeline_length - critical_inactive_size], # for job_hang
        [critical_inactive_size, timeline_length]      # for cold_start
    )[check_cold_start]

    borders_inactive = (
        [timeline_length - critical_inactive_size, timeline_length], # for job_hang
        [0, critical_inactive_size]                                  # for cold_start
    )[check_cold_start]

    metric_active = np.array([get_measure(sensors[idx], borders_active) for idx in range(TOTAL_SENSORS_NUM)])
    metric_inactive = np.array([get_measure(sensors[idx], borders_inactive) for idx in range(TOTAL_SENSORS_NUM)])

    max_sample = lambda sample: max(sample) if len(sample) != 0 else 0
    flag_gpu = None
    if max_sample(metric_active[GPU_LOAD_IDX]) > 0 and max_sample(metric_inactive[GPU_LOAD_IDX]) < config['EPS']:
        flag_gpu = True
    elif max_sample(metric_inactive[GPU_LOAD_IDX]) > config['EPS']:
        flag_gpu = False

    flag_val = [
        sum(elem > threshold[i] for elem in metric_active[i]) > active_wnd_num
        and all(elem < threshold[i] for elem in metric_inactive[i])
        if threshold[i] >= 0 and i != GPU_LOAD_IDX else None
        for i in range(TOTAL_SENSORS_NUM)
    ]

    flag_val[GPU_LOAD_IDX] = flag_gpu

    return flag_val


def check_if_job_hang(sensors, threshold, target_flags):
    flags = get_job_hang_flags(sensors, threshold)
    return (
        flags[GPU_LOAD_IDX] == True
        or flags[GPU_LOAD_IDX] == None
            and sum(flags[i] == True for i in target_flags) >= len(target_flags) / 2
    )


def check_if_cold_start(sensors, threshold, target_flags):
    flags = get_job_hang_flags(sensors, threshold, check_cold_start = True)
    return (
        flags[GPU_LOAD_IDX] == True
        or flags[GPU_LOAD_IDX] == None
            and sum(flags[i] == True for i in target_flags) >= len(target_flags) / 2
    )


def check_if_stall(sensors, threshold, target_flags):
    critical_inactive_size = config['critical_inactive_size']
    critical_inactive_num_of_wnd = config['num_of_wnd']
    timeline_length = sensors.shape[1] - config['job_tail']
    offset_from_end = config['offset_from_end']

    # Search for stall window from the start
    metric_start = np.array([
        get_measure(sensors[idx], [0, timeline_length - offset_from_end]) if idx in target_flags else None
        for idx in range(TOTAL_SENSORS_NUM)
    ])
    
    stall_start = -1
    for start in range(timeline_length - critical_inactive_size - offset_from_end):
        is_each_sensor_hang = all(
            all(item < threshold[flag] for item in metric_start[flag][start : start + critical_inactive_num_of_wnd])
            for flag in [i for i in target_flags if threshold[i] > 0]
        )

        if is_each_sensor_hang:
            stall_start = start
            break

    if stall_start <= 0:
        return False

    # Check if task will be active again after the window
    if stall_start + critical_inactive_size > timeline_length:
        return False

    borders_after_stall = [stall_start + critical_inactive_size, timeline_length]
    
    metric_end = np.array([
        get_measure(sensors[idx], borders_after_stall) if idx in target_flags else None
        for idx in range(TOTAL_SENSORS_NUM)
    ])

    # Check if the majority of target sensons is active after stall
    active_flags_count = sum(any(elem > threshold[flag] for elem in metric_end[flag]) for flag in target_flags)
    is_active_in_end = active_flags_count > len(target_flags) / 2
    
    return is_active_in_end


def is_task_long_enough(sensors):
    timeline_length = sensors.shape[1] - config['job_tail']
    return timeline_length >= config['critical_inactive_size'] + config['wnd_size']


def brute_force_job_hang(threshold_mode):
    task_raw_flags = dict()
    none_flags_idx = set()

    all = file_io.get_file_names(os.getenv('TASK_DIR'))

    thresholds = get_threshold(threshold_mode)

    for i in range(len(all)):
        task = all[i]
        sensors = sensor_helper.get_sensors(task)
        if not is_task_long_enough(sensors):
            continue

        task_raw_flags[task] = get_job_hang_flags(sensors, thresholds)

        for i in range(TOTAL_SENSORS_NUM):
            if task_raw_flags[task][i] == None:
                none_flags_idx.add(i)

    # Exclude some sensors
    none_flags_idx.add(2) # mem_store
    none_flags_idx.add(4) # mem_load
    none_flags_idx.add(10) # cpu_nice (always zero)
    none_flags_idx.add(16) # gpu_mem_load
    none_flags_idx.add(17) # gpu_mem_usage

    best_accuracy = [0,0,0]
    acc_val = -1

    for comb in range((int)(pow(2, TOTAL_SENSORS_NUM))):
        tasks_predictions = dict()

        for bad_flag in none_flags_idx: # set bag flag bits to zero
            comb = comb & ~(1 << bad_flag)

        num_of_current_sensors = bin(comb).count("1")

        for task in task_raw_flags:
            flag_vec = task_raw_flags[task]
            num_of_hang_sensors = sum(flag_vec[i] for i in range(TOTAL_SENSORS_NUM) if ((comb & (1 << i)) >> i) == 1)
            prediction = num_of_hang_sensors >= (num_of_current_sensors / 2)
            tasks_predictions[task] = bool(prediction)

        acc = eval.get_balanced_acc(tasks_predictions, BehaviorClass.JOB_HANG)

        if acc > best_accuracy[0]:
            best_accuracy[0] = acc
            acc_val = comb
        
    print(best_accuracy[0])
    print(f"{acc_val} = {'{0:b}'.format(acc_val)}")


# Draws table with all tasks in rows and sensor flags in columns
def get_job_hang_task_table(target_set, threshold_mode):
    threshold = get_threshold(threshold_mode)

    task_table = dict()

    hang = open(eval.get_markup_path_by_class(BehaviorClass.JOB_HANG), 'r').read().split('\n')
    all = file_io.get_file_names(os.getenv('TASK_DIR'))

    for task in all:
        job_hang_mark = task in hang
        sensors = sensor_helper.get_sensors(task)
        if not is_task_long_enough(sensors):
            continue

        flag = get_job_hang_flags(task, threshold)
        test_hang = sum(flag[i] == True for i in target_set) >= len(target_set) / 2
        task_table[task] = (job_hang_mark, test_hang, flag)

    for task in task_table:
        print(task, end = '\t')
        (job_hang_mark, test_hang, flag) = task_table[task]
        int_flags = [int(x) if x != None else -1 for x in flag]
        print(job_hang_mark, end = '\t')
        print(test_hang, end = '\t')
        for x in int_flags:
            print(x, end = "\t")
        print()
