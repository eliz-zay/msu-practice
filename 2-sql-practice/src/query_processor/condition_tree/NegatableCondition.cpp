#pragma once

#include "NegatableCondition.h"

NegatableCondition::NegatableCondition(bool isNegated) {
    this->isNegated = isNegated;
}

void NegatableCondition::negate() {
    isNegated = !isNegated;
}
