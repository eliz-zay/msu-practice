import numpy as np
import matplotlib.pyplot as plt

import io_helper
import sensor_helper


def plot(task, borders = [], sensor_num = -1):
    config = io_helper.get_config()

    borders_normal = config["job_hang_borders"]["normal"][f"{task}"] if f"{task}" in config["job_hang_borders"]["normal"] else []
    borders_abnormal = config["job_hang_borders"]["abnormal"][f"{task}"] if f"{task}" in config["job_hang_borders"]["abnormal"] else []

    sensors = sensor_helper.get_sensors(task)

    timestamps_per_sensor = sensors.shape[1]
    print(f"length: {timestamps_per_sensor}")

    def set_borders():
        if len(borders):
            for border in borders:
                plt.axvline(x = border, color = 'cyan')

            return

        for border_pair in borders_normal:
            for border in border_pair:
                plt.axvline(x = border, color = 'cyan')

        for border_pair in borders_abnormal:
            for border in border_pair:
                plt.axvline(x = border, color = 'magenta')

    
    if sensor_num != -1:
        plt.figure(figsize = (10, 4))
        x = np.arange(0, timestamps_per_sensor, 1)
        y = np.nan_to_num(sensors[sensor_num])
        set_borders()

        plt.title(config["sensor_names"][int(sensor_num/2)])
        plt.xlabel("Time")
        plt.ylabel("Value")
        plt.plot(x, y)
    else:
        plt.figure(figsize=(7, 30))
        
        for i in range(config["total_sensors_num"]):
            x = np.arange(0, timestamps_per_sensor, 1)
            y = np.nan_to_num(sensors[i])
            set_borders()

            plt.subplot(len(config["sensor_names"]), 1, i + 1)
            plt.title(config["sensor_names"][i])
            plt.xlabel("Time")
            plt.ylabel("Value")
            plt.plot(x, y)
        
        plt.tight_layout()


def plot_sensor(tasks, sensor_num):
    config = io_helper.get_config()
    plt.figure(figsize=(10, 20))
    idx = 0
    for task in tasks:
        idx += 1
        sensors = sensor_helper.get_sensors(task)
        timestamps_per_sensor = sensors.shape[1]

        x = np.arange(0, timestamps_per_sensor, 1)
        y = np.nan_to_num(sensors[sensor_num])

        plt.subplot(len(tasks), 1, idx)
        plt.title(config["sensor_names"][sensor_num])
        plt.plot(x, y)

    plt.tight_layout()
    plt.show()


def plot_histogram(abnormal_var, normal_var, sensor_num = -1):
    config = io_helper.get_config()
    sensors_num = len(config["sensor_names"])

    if sensor_num == -1:
        for i in range(sensors_num):
            print(config["sensor_names"][i])

            plt.hist(abnormal_var[i], bins = 2000, density = False, label = 'abnormal', stacked = True, alpha = 0.5)
            plt.hist(normal_var[i], bins = 2000, density = False, label = 'normal', stacked = True, alpha = 0.5)
            plt.axvline(x = np.median(abnormal_var[i]), color = 'teal', label = 'abnormal median')
            plt.axvline(x = np.median(normal_var[i]), color = 'orangered', label = 'normal median')
            plt.yscale(value = "log")
            left, right = plt.xlim()
            plt.xlim(left / 100, np.median(normal_var[i]) * 1.5)
            plt.legend(loc = 'upper right')
            plt.xlabel("Quartile deviation")
            plt.show()
    else:
        print(config["sensor_names"][sensor_num])

        plt.hist(abnormal_var[sensor_num], bins = 500, density = False, label = 'abnormal', stacked = True, alpha = 0.5)
        plt.hist(normal_var[sensor_num], bins = 4000, density = False, label = 'normal', stacked = True, alpha = 0.5)
        plt.axvline(x = np.median(abnormal_var[sensor_num]), color = 'royalblue', label = 'abnormal median')
        plt.axvline(x = np.median(normal_var[sensor_num]), color = 'orangered', label = 'normal median')
        plt.yscale(value = "log")
        left, right = plt.xlim()
        plt.xlim(left / 100, np.median(normal_var[sensor_num]) * 1.5)
        plt.legend(loc = 'upper right')
        plt.xlabel("Quartile deviation")
        plt.show()
