#pragma once

#include "EngineException.h"

#include "EngineStatusStrings.h"

#include <iostream>
#include <exception>

EngineException::EngineException(EngineStatusEnum status) {
    this->status = status;
}

const char* EngineException::what() const throw () {
    return engineStatusString[status].c_str();
}

int EngineException::getStatus() {
    return status;
}

EngineException::~EngineException() throw() {}
