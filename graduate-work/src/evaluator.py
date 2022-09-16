import os
import sys
import numpy as np
from sklearn.metrics import balanced_accuracy_score
from dotenv import load_dotenv

import BehaviorClass

load_dotenv()


def get_balanced_acc(tasks_predictions, behavior_class):
    hang_tasks = open(get_markup_path_by_class(behavior_class), 'r').read().split('\n')

    pred = np.zeros(len(tasks_predictions))
    actual = np.zeros(len(tasks_predictions))

    i = 0
    for task in tasks_predictions:
        pred[i] = 1 if tasks_predictions[task][behavior_class] else 0
        actual[i] = 1 if task in hang_tasks else 0
        i += 1

    acc = balanced_accuracy_score(actual, pred)

    return acc


def confusion_matrix(tasks_predictions, behavior_class, print_summary = False):
    hang_tasks = open(get_markup_path_by_class(behavior_class), 'r').read().split('\n')

    confusion_matrix = np.zeros((2,2))

    for task in tasks_predictions:
        actual_hang = True if task in hang_tasks else False
        
        prediction = tasks_predictions[task][behavior_class]

        if actual_hang and prediction:
            confusion_matrix[0][0] += 1
        elif actual_hang and not prediction:
            confusion_matrix[0][1] += 1
        elif not actual_hang and prediction:
            confusion_matrix[1][0] += 1
        elif not actual_hang and not prediction:
            confusion_matrix[1][1] += 1

    if print_summary:
        total = len(tasks_predictions)
        fn = confusion_matrix[0][1]
        fp = confusion_matrix[1][0]
        print(f"\n * * * Behavior class: {behavior_class.value} * * *")
        print(f"\nBalanced accuracy: {'{:.1f}'.format(get_balanced_acc(tasks_predictions, behavior_class) * 100)}%   ")
        print(f"\nAccuracy: {'{:.1f}'.format((1 - (fn + fp) / total) * 100)}%   ")
        print(f"False pos: {'{:.1f}'.format(fp / total * 100)}%   ")
        print(f"False neg: {'{:.1f}'.format(fn / total * 100)}%   ")
        print("\n\nPredicted\tAbnorm\tNorm\nActual   ")
        print(f"Abnorm\t\t{int(confusion_matrix[0][0])}\t{int(confusion_matrix[0][1])}   ")
        print(f"Norm\t\t{int(confusion_matrix[1][0])}\t{int(confusion_matrix[1][1])}   ")
        print()

    return [
        (confusion_matrix[0][0] + confusion_matrix[1][1]) / len(tasks_predictions), # accuracy
        confusion_matrix[1][0] / len(tasks_predictions), # false positive
        confusion_matrix[0][1] / len(tasks_predictions)  # false negative
    ]


def get_markup_path_by_class(behavior_class):
    if behavior_class == BehaviorClass.JOB_HANG:
        return os.getenv('JOB_HANG_TASKS_MARKUP_PATH')
    if behavior_class == BehaviorClass.STALL:
        return os.getenv('STALL_TASKS_MARKUP_PATH')
    if behavior_class == BehaviorClass.COLD_START:
        return os.getenv('COLD_START_TASKS_MARKUP_PATH')
    
    print('Invalid behavior class')
    sys.exit()
