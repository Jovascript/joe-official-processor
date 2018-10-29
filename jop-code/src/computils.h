//
// Created by Joseph on 26/10/2018.
//

#ifndef JOP_UTILS_H
#define JOP_UTILS_H


#include <string>
#include <sstream>
#include <vector>
#include <antlr4-runtime.h>


std::string indent(std::string x);

template <class T>
std::string stringify(T v) {
    std::stringstream x;
    x << v;
    return x.str();
}

template<>
std::string stringify(std::vector<char> v);

template <class U>
std::string stringify(std::vector<U> v) {
    std::stringstream x;
    for (U n: v) {
        x << n << " ";
    }
    return x.str();
}


void compiler_warn(std::string message, antlr4::ParserRuleContext* token);
void compiler_warn(std::string message, antlr4::tree::TerminalNode* token);
void compiler_error(std::string message, antlr4::ParserRuleContext* token);
void compiler_error(std::string message, antlr4::tree::TerminalNode* token);

class CompilerError : public std::exception {
private:
    std::string message_;
public:
    explicit CompilerError(std::string message);

    const char *what() const throw() override {
        return message_.c_str();
    }

};

#endif //JOP_UTILS_H
