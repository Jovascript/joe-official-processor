//
// Created by Joseph on 08/10/2018.
//

#ifndef JOP_HELPERS_H
#define JOP_HELPERS_H

#include <exception>
#include <string>
#include <jopemulation.h>


class CompilerError : public std::exception {
private:
    std::string message_;
public:
    explicit CompilerError(std::string message);

    const char *what() const throw() override {
        return message_.c_str();
    }

};

struct AddressAllocation {
    AddressAllocation(const std::string &label, bool resolved, jop::addrtype address);

    std::string label;
    bool resolved;
    jop::addrtype address;
};



#endif //JOP_HELPERS_H
