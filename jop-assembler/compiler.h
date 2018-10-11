//
// Created by Joseph on 18/09/2018.
//

#ifndef JOP_COMPILER_H
#define JOP_COMPILER_H


#include <jopemulation.h>
#include <unordered_map>

#include "compilation_helpers.h"

class AddressAllocationHandler {
private:
    std::unordered_map<std::string, std::shared_ptr<AddressAllocation>> labels;
public:
    AddressAllocationHandler();
    std::shared_ptr<AddressAllocation> getLabelDestination(std::string label);
    void registerLabelLocation(std::string label, jop::addrtype loc);
};

class CodeUnit {
public:
    virtual int get_size() = 0;
    virtual std::vector<jop::dtype> generate_data() = 0;
    virtual bool is_resolved() = 0;
    virtual bool resolve() = 0;
    std::string label;
};

class AssemblyInstruction: public CodeUnit {
public:

    int get_size() override;

    std::vector<jop::dtype> generate_data() override;

    bool is_resolved() override;

    bool resolve() override;

    std::shared_ptr<AddressAllocation> addressReference;
    jop::Instruction inst;
    jop::dtype reg;
    std::vector<jop::dtype> data;
    bool uses_register = false;

private:
    bool resolved;

};

class DataBlock: public CodeUnit {
public:
    int get_size() override;

    std::vector<jop::dtype> generate_data() override;

    bool is_resolved() override;

    bool resolve() override;

    std::vector<jop::dtype> data;

};

std::vector<jop::byte> compile(std::shared_ptr<std::ifstream> f);


#endif //JOP_COMPILER_H
