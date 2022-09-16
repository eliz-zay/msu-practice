import numpy as np
import sys
import os
from dotenv import load_dotenv

import io_helper

load_dotenv()
config = io_helper.get_config()


def get_sensors(task):
    try:
        file_name = os.getenv('TASK_DIR') + str(task) + '.npz'
        data = np.load(file_name)

        sensors = data['a']
        sensors = sensors.clip(min = 0) # filter from negative values

        if len(sensors) > config['total_sensors_num'] * 2:
            sensors = sensors[0::2] # every second item contains irrelevant data

        data.close()
        return np.nan_to_num(sensors)
    except FileNotFoundError:
        print(f"File {file_name} not found")
        sys.exit()
