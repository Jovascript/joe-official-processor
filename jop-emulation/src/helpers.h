//
// Created by Joseph on 23/09/2018.
//

#ifndef JOP_HELPERS_H
#define JOP_HELPERS_H

#include <boost/format.hpp>
#include <constants.h>

namespace jop {
    ProcessorException proc_err(boost::format msg);
    ProcessorException proc_err(std::string msg);

}

#endif //JOP_HELPERS_H
