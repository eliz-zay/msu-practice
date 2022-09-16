#ifndef ENGINE_EXCEPTION_H
#define ENGINE_EXCEPTION_H

#include <string>
#include <exception>

#include "EngineStatusEnum.h"

using namespace std;

class EngineException: public std::exception {
    protected:
        EngineStatusEnum status;
    public:
        EngineException(EngineStatusEnum status);

        const char* what() const throw();

        int getStatus();
        
        ~EngineException() throw();
};

#endif