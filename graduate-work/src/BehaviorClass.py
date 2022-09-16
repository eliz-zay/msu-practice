import sys
from enum import Enum

class BehaviorClass(Enum):
    JOB_HANG = 'job_hang'
    STALL = 'stall'
    COLD_START = 'cold_start'


sys.modules[__name__] = BehaviorClass