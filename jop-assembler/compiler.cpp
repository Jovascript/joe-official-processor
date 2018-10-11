//
// Created by Joseph on 18/09/2018.
//

#include <boost/algorithm/string.hpp>
#include <fstream>
#include "compiler.h"


#include "lexer.h"


class compiler {
public:
    explicit compiler(std::shared_ptr<std::istream> &f) : _lexer(f) {}

    std::vector<jop::dtype> compile() {
        std::vector<std::unique_ptr<CodeUnit>> parse_res;

        _lexer.consumeToken();

        while (!_lexer.parse_token(Token::tok_eof)) {
            parse_res.push_back(parse_instruction());
        }

        int current_address = 0;
        // Populate label dicts
        for (const auto &ln: parse_res) {
            if (!ln->label.empty()) {
                addr_manage.registerLabelLocation(ln->label, current_address);
            }
            current_address += ln->get_size();
        }

        for (auto &ln: parse_res) {
            if (!ln->is_resolved()) {
                ln->resolve();
            }
        }
        std::vector<jop::dtype> result;
        for (auto &ln: parse_res) {
            auto instruct_data = ln->generate_data();
            result.insert(result.end(), instruct_data.begin(), instruct_data.end());
        }
        return result;
    }

    std::unique_ptr<CodeUnit> parse_instruction() {
        bool directive = false;
        std::string label;
        auto tokn = readIdentifier();
        auto n = tokn.identifier;


        if (_lexer.parse_token(Token::tok_colon)) {
            label = n;
            // Parse the identifier

            n = readIdentifier().identifier;

        }
        if (n[0] == '.') {
            directive = true;
        }
        boost::to_upper(n);

        if (directive) {
            auto x = std::make_unique<DataBlock>();
            if (!label.empty()) {
                x->label = label;
            }
            if (n == ".DAT") {
                while (auto d = _lexer.parse_token(Token::tok_literal)) {
                    x->data.push_back(static_cast<unsigned short &&>(d->literal));
                }
            } else {
                cry("Expected directive");
            }
            return x;
        } else {
            auto x = std::make_unique<AssemblyInstruction>();
            if (!label.empty()) {
                x->label = label;
            }
            if (n == "LDA") {
                // 2 args, addr, reg(MVMR)
                x->inst = jop::Instruction::MVMR;
                auto tok = readAddressSpace();
                switch (tok.type) {
                    case Token::tok_reg:
                        x->uses_register = true;
                        x->data.push_back(tok.reg);
                        break;
                    case Token::tok_literal: {
                        auto dat = jop::dataFromAddress(static_cast<jop::addrtype>(tok.literal));
                        x->data.insert(x->data.end(), dat.begin(), dat.end());
                        break;
                    }
                    case Token::tok_identifier:
                        x->addressReference = addr_manage.getLabelDestination(tok.identifier);
                        break;
                }

                // Now read the register.
                x->reg = readRegisterSpace().reg;

            } else if (n == "STA") {
                // 2 args, reg, addr(MVRM)
                x->inst = jop::Instruction::MVRM;
                // Now read the register.
                x->reg = readRegisterSpace().reg;


                auto tok = readAddressSpace();
                switch (tok.type) {
                    case Token::tok_reg:
                        x->uses_register = true;
                        x->data.push_back(tok.reg);
                        break;
                    case Token::tok_literal: {
                        auto dat = jop::dataFromAddress(static_cast<jop::addrtype>(tok.literal));
                        x->data.insert(x->data.end(), dat.begin(), dat.end());
                        break;
                    }
                    case Token::tok_identifier:
                        x->addressReference = addr_manage.getLabelDestination(tok.identifier);
                        break;
                }
            } else if (n == "MV") {
                // 2 args; reg, reg(MVRR) or reg, literal(MVLR)

                // In the case of RR, read register, in LR, read literal
                if (auto y = _lexer.parse_token(Token::tok_reg)) {

                    // TODO: Figure out a way to make this work when addresses are bigger? than dtype

                    x->data.push_back(y->reg);
                    x->inst = jop::Instruction::MVRR;
                } else {
                    try {
                        auto tok = readLiteralSpace();
                        if (tok.type == Token::tok_literal) {
                            x->addressReference = addr_manage.getLabelDestination(tok.identifier);
                        } else {
                            // Is literal
                            x->data.push_back(static_cast<unsigned short &&>(tok.literal));
                        }
                        x->inst = jop::Instruction::MVLR;
                    } catch (CompilerError &error1) {
                        throw CompilerError(_lexer.format_expected("Expected register or literal(value or label)"));
                    }
                }

                // Read register
                x->reg = readRegisterSpace().reg;

            } else if (n == "NOP") {
                x->inst = jop::Instruction::NOP;
            } else if (n == "HALT") {
                x->inst = jop::Instruction::HLT;
            } else if (n == "ADD") {
                x->inst = jop::Instruction::ADD;
                x->reg = readRegisterSpace().reg;
            } else if (n == "SUB") {
                x->inst = jop::Instruction::SUB;
                x->reg = readRegisterSpace().reg;
            } else if (n == "MUL") {
                x->inst = jop::Instruction::MUL;
                x->reg = readRegisterSpace().reg;
            } else if (n == "DIV") {
                x->inst = jop::Instruction::DIV;
                x->reg = readRegisterSpace().reg;
            } else if (n == "AND") {
                x->inst = jop::Instruction::AND;
                x->reg = readRegisterSpace().reg;
            } else if (n == "OR") {
                x->inst = jop::Instruction::OR;
                x->reg = readRegisterSpace().reg;
            } else if (n == "NOT") {
                x->inst = jop::Instruction::NOT;
                x->reg = readRegisterSpace().reg;
            } else if (n == "JP" || n == "JPZ") {
                x->inst = n == "JP" ? jop::Instruction::JP : jop::Instruction::JPZ;

                auto tok = readAddressSpace();
                switch (tok.type) {
                    case Token::tok_reg:
                        x->uses_register = true;
                        x->data.push_back(tok.reg);
                        break;
                    case Token::tok_literal: {
                        auto dat = jop::dataFromAddress(static_cast<jop::addrtype>(tok.literal));
                        x->data.insert(x->data.end(), dat.begin(), dat.end());
                        break;
                    }
                    case Token::tok_identifier:
                        x->addressReference = addr_manage.getLabelDestination(tok.identifier);
                        break;
                }
            } else if (n == "OUT") {
                x->inst = jop::Instruction::OUT;
            } else if (n == "IN") {
                x->inst = jop::Instruction::INP;
            } else {

                cry("Expecting instruction");
            }

            return x;
        }
    }

    Token readAddressSpace() {
        if (auto reg = _lexer.parse_token(Token::tok_reg)) {
            return *reg;
        } else if (auto ident = _lexer.parse_token(Token::tok_identifier)) {
            return *ident;
        } else if (auto lit = _lexer.parse_token(Token::tok_literal)) {
            return *lit;
        } else {
            cry("Expected address(identifier, register or literal)");
        }
    }

    Token readLiteralSpace() {
        if (auto lit = _lexer.parse_token(Token::tok_literal)) {
            return *lit;
        } else if (auto ident = _lexer.parse_token(Token::tok_identifier)) {
            return *ident;
        } else {
            cry("Expected value(identifier or literal)");
        }
    }

    Token readRegisterSpace() {
        if (auto reg = _lexer.parse_token(Token::tok_reg)) {
            return *reg;
        } else {
            cry("Expected register");
        }
    }

    Token readIdentifier() {
        if (auto ident = _lexer.parse_token(Token::tok_identifier)) {
            return *ident;
        } else {
            cry("Expected identifier");
        }
    }

    void cry(const std::string &message) {
        auto x = _lexer.format_expected(message);
        throw CompilerError(x);
    }

private:
    lexer _lexer;
    AddressAllocationHandler addr_manage;
};


std::vector<jop::byte> compile(std::shared_ptr<std::ifstream> f) {
    auto x = std::dynamic_pointer_cast<std::istream>(f);
    compiler compiler1(x);
    auto insts = compiler1.compile();
    return jop::bytesFromData(insts);
}


AddressAllocationHandler::AddressAllocationHandler() = default;


int AssemblyInstruction::get_size() {
    // Hack assuming only data is address in addressed circumstances
    return is_resolved() ? static_cast<int>(1 + data.size()) : 1 + jop::addr_data_scale;
}

std::vector<jop::dtype> AssemblyInstruction::generate_data() {
    auto x = std::vector<jop::dtype>();
    x.reserve(static_cast<unsigned long>(get_size()));
    // Real top byte = instruction, lower byte is register
    // Top bit is whether register is used as address or literal value
    jop::dtype instruction_byte = (static_cast<jop::dtype>(inst) << 8) | reg;
    instruction_byte |= uses_register << 16;
    x.push_back(instruction_byte);
    if (!data.empty()) {
        x.insert(x.end(), data.begin(), data.end());
    }
    return x;
}

bool AssemblyInstruction::is_resolved() {
    return resolved;
}

bool AssemblyInstruction::resolve() {
    if (addressReference != nullptr && addressReference->resolved) {
        auto x = jop::dataFromAddress(addressReference->address);
        data.insert(data.end(), x.begin(), x.end());
        resolved = true;
    }
}

std::shared_ptr<AddressAllocation> AddressAllocationHandler::getLabelDestination(std::string label) {
    auto search = labels.find(label);
    if (search != labels.end()) {
        return search->second;
    } else {
        auto new_added = std::make_shared<AddressAllocation>(label, false, 0);
        labels[label] = new_added;
        return new_added;
    }
}

void AddressAllocationHandler::registerLabelLocation(std::string label, jop::addrtype loc) {
    auto search = labels.find(label);
    if (search != labels.end()) {
        if (search->second->resolved) {
            throw CompilerError("Label " + label + " is already defined!");
        } else {
            search->second->resolved = true;
            search->second->address = loc;
        }
    } else {
        auto new_added = std::make_shared<AddressAllocation>(label, true, loc);
        labels[label] = new_added;
    }

}


int DataBlock::get_size() {
    return static_cast<int>(data.size());
}

std::vector<jop::dtype> DataBlock::generate_data() {
    return data;
}

bool DataBlock::is_resolved() {
    return true;
}

bool DataBlock::resolve() {
    return true;
}

