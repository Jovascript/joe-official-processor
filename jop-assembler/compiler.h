//
// Created by Joseph on 18/09/2018.
//

#ifndef JOP_COMPILER_H
#define JOP_COMPILER_H

#include <string>
#include <fstream>
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

struct AssemblyInstruction {
    // Can be resolved or not resolved.
    jop::dtype inst;
    std::string label;
    std::vector<jop::dtype> data;
    // The label it references.
    std::string labelRef;
    bool isResolved() const;
    int inst_size() const;
};

std::vector<jop::byte> compile(std::shared_ptr<std::ifstream> f);


#endif //JOP_COMPILER_H
