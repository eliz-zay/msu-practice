import sys
from enum import Enum

class BFThresholdMode(Enum):
    CUSTOM_LIKE = 1
    PERCENT_OPTIMAL = 2

sys.modules[__name__] = BFThresholdMode