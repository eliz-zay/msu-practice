import os
import sys
import json
from dotenv import load_dotenv

import BehaviorClass

load_dotenv()


def get_config():
    config_file = open("config.json")
    config = json.load(config_file)

    return config


config = json.load(open("config.json"))


def get_task_idx():
    print("\nChoose task index: ", end = '')
    idx = input()
    
    return idx


def get_sensor_idx():
    config = get_config()
    print("\n%d %s" %(-1, "VIEW ALL SENSORS"))
    for i in range(len(config["sensor_names"])):
        print("%d %s" %(i, config["sensor_names"][i]))

    print("\nChoose sensor:")
    idx = int(input())

    if idx not in range(-1, len(config["sensor_names"])):
        print("Error: %d is not a sensor index!" %(idx))
        sys.exit()

    return idx


def num_to_set(value):
    get_bit = lambda value, bit_idx: (value & (1 << bit_idx)) >> bit_idx
    idxs = set()
    for i in range(config['total_sensors_num']):
        if get_bit(value, i):
            idxs.add(i)

    return idxs


def predictions_to_file(predictions):
    file = open(os.getenv('PREDICTIONS_FILE'), 'w')
    file.write( '{:<20s} {:<10s} {:<10s} {:<10s}\n'.format('Task', 'job_hang', 'stall', 'cold_start') )
    for task in predictions:
        file.write( '{:<20s} {:<10s} {:<10s} {:<10s}\n'.format(
            task,
            str(predictions[task][BehaviorClass.JOB_HANG]),
            str(predictions[task][BehaviorClass.STALL]),
            str(predictions[task][BehaviorClass.COLD_START])
        ) )

    file.close()
