#include <utility>

//
// Created by Joseph on 13/09/2018.
//

#include "constants.h"
namespace jop {
    ProcessorException::ProcessorException(std::string message) : message_(std::move(message)) {

    }

    ProcessorException proc_err(boost::format msg) {
        return ProcessorException(msg.str());
    }

    ProcessorException proc_err(std::string msg) {
        return ProcessorException(std::move(msg));
    }
}