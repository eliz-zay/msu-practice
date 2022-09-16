import os
import importlib
from dotenv import load_dotenv

import io_helper
import sensor_helper
import job_math as jm
import file_io
import evaluator as eval
import BFThresholdMode
import BehaviorClass

importlib.reload(io_helper)
importlib.reload(sensor_helper)
importlib.reload(file_io)
importlib.reload(jm)
importlib.reload(eval)

load_dotenv()
config = io_helper.get_config()


def test(task_sensors, target_flags, threshold_mode, eval_accuracy = False):
    threshold = jm.get_threshold(threshold_mode, read_variation_from_cache = True)

    tasks_predictions = dict()

    for task in task_sensors:
        sensors = task_sensors[task]

        if jm.is_task_long_enough(sensors):
            tasks_predictions[task] = {
                BehaviorClass.JOB_HANG: jm.check_if_job_hang(sensors, threshold, target_flags),
                BehaviorClass.STALL: jm.check_if_stall(sensors, threshold, target_flags),
                BehaviorClass.COLD_START: jm.check_if_cold_start(sensors, threshold, target_flags)
            }
        else:
            tasks_predictions[task] = {
                BehaviorClass.JOB_HANG: False, BehaviorClass.STALL: False, BehaviorClass.COLD_START: False
            }

    if eval_accuracy:
        eval.confusion_matrix(tasks_predictions, BehaviorClass.JOB_HANG, print_summary = True)
        eval.confusion_matrix(tasks_predictions, BehaviorClass.STALL, print_summary = True)
        eval.confusion_matrix(tasks_predictions, BehaviorClass.COLD_START, print_summary = True)
    else:
        io_helper.predictions_to_file(tasks_predictions)


def get_tasks_sensors():
    tasks = file_io.get_file_names(os.getenv('TASK_DIR'))

    task_sensors = dict()
    for task in tasks:
        task_sensors[task] = sensor_helper.get_sensors(task)

    return task_sensors


def run():
    test(
        get_tasks_sensors(),
        config['best_sensor_combination'],
        BFThresholdMode.PERCENT_OPTIMAL,
        eval_accuracy = False
    )


run()
