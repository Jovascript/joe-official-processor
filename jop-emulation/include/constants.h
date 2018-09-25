//
//
// Created by Joseph on 12/09/2018.
//

#ifndef JOP_CONSTANTS_H
#define JOP_CONSTANTS_H

#include <exception>
#include <string>

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

}
#endif //JOP_CONSTANTS_H
