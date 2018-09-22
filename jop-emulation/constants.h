//
//
// Created by Joseph on 12/09/2018.
//

#ifndef JOP_CONSTANTS_H

#define JOP_CONSTANTS_H

#include <exception>
#include <string>
#include "boost/format.hpp"

namespace jop {

    class ProcessorException : public std::exception {
    private:
        std::string message_;
    public:
        explicit ProcessorException(std::string message);

        const char *what() const throw() override {
            return message_.c_str();
        }

    };


    ProcessorException proc_err(boost::format msg);

    ProcessorException proc_err(std::string msg);

}
#endif //JOP_CONSTANTS_H
