//
// Created by Joseph on 23/09/2018.
//

#include "helpers.h"

namespace jop {
    ProcessorException proc_err(boost::format msg) {
        return ProcessorException(msg.str());
    }

    ProcessorException proc_err(std::string msg) {
        return ProcessorException(std::move(msg));
    }

}


