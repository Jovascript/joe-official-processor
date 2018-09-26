#include <utility>


//
// Created by Joseph on 18/09/2018.
//

#include "compiler.h"
#include <unordered_set>
#include <iostream>
#include <boost/algorithm/string.hpp>
#include <unordered_map>
#include <bitset>

bool AssemblyInstruction::isResolved() const {
    return (labelRef.empty()) || (!data.empty());
}

int AssemblyInstruction::inst_size() const {
    return isResolved() ? static_cast<int>(1 + data.size()) : 1 + jop::addr_data_scale;
}


class compiler {
public:
    explicit compiler(std::shared_ptr<std::ifstream> f): current_char() {
        _f = std::move(f);
    }
    std::vector<jop::dtype> compile() {
        nextChar();
        std::vector<AssemblyInstruction> parse_res;
        while (!_f->eof()) {

            parse_res.push_back(parse_instruction());
        }

        std::unordered_map<std::string, jop::addrtype> labels;
        int current_address = 0;
        // Populate label dicts
        for (const auto &ln: parse_res) {
            if (!ln.label.empty()) {
                labels[ln.label] = current_address;
            }
            current_address += ln.inst_size();
        }

        for (auto &ln: parse_res) {
            if (!ln.isResolved()) {
                auto it = labels.find(ln.labelRef);
                if (it != labels.end()) {
                    auto bytes = jop::dataFromAddress(it->second);
                    ln.data.insert(ln.data.end(), bytes.begin(), bytes.end());
                } else {
                    throw CompilerError("Unknown label " + ln.labelRef);
                }
            }
        }
        std::vector<jop::dtype> result;
        for (auto &ln: parse_res) {
            result.push_back(ln.inst);
            result.insert(result.end(), ln.data.begin(), ln.data.end());
        }
        return result;
    }

    AssemblyInstruction parse_instruction() {
        AssemblyInstruction x;
        trim_whitespace();
        auto n = readIdentifier();

        trim_whitespace();

        if (current_char == ':') {
            nextChar();
            // n is the label
            x.label = n;
            // Parse the identifier
            trim_whitespace();
            n = readIdentifier();
            trim_whitespace();
        }
        boost::to_upper(n);
        if (n == "LDA") {
            // 2 args, addr, reg(MVMR)
            x.inst = 0x2 << 3;
            trim_whitespace();
            try {
                auto bytes = jop::dataFrom(readValue());
                x.data.insert(x.data.end(), bytes.begin(), bytes.end());
            } catch (CompilerError &error) {
                x.labelRef = readIdentifier();
            }
            // Now read the register.
            trim_whitespace();
            x.inst |= readRegister() & 0b00000111;
        } else if (n == "STA") {
            // 2 args, reg, addr(MVRM)
            x.inst = 0x3 << 3;
            // Now read the register.
            trim_whitespace();
            x.inst |= readRegister() & 0b00000111;
            trim_whitespace();
            try {
                auto bytes = jop::dataFrom(readValue());
                x.data.insert(x.data.end(), bytes.begin(), bytes.end());
            } catch (CompilerError &error) {
                x.labelRef = readIdentifier();
            }
        } else if (n == "MV"){
            // 2 args; reg, reg(MVRR) or reg, literal(MVLR)

            // Now read the register.
            trim_whitespace();
            x.inst = static_cast<jop::byte>(readRegister() & 0b00000111);
            trim_whitespace();
            try {
                x.data.push_back(readByteValue());
                // Literal
                x.inst |= 0x5 << 3;
            } catch (CompilerError &error) {
                x.data.push_back(readRegister());
                // Register
                x.inst |= 0x4 << 3;
            }
        } else if (n == "NOP") {
            x.inst = 0x0 << 3;
        } else if (n == "HALT") {
            x.inst = 0x1 << 3;
        } else if (n == "ADD") {
            x.inst = 0x6 << 3 | readRegister();
        } else if (n == "SUB") {
            x.inst = 0x7 << 3 | readRegister();
        } else if (n == "MUL") {
            x.inst = 0x8 << 3 | readRegister();
        } else if (n == "DIV") {
            x.inst = 0x9 << 3 | readRegister();
        } else if (n == "AND") {
            x.inst = 0xA << 3 | readRegister();
        } else if (n == "OR") {
            x.inst = 0xB << 3 | readRegister();
        } else if (n == "NOT") {
            x.inst = 0xC << 3;
        } else if (n == "JP" || n == "JPZ") {
            x.inst = n == "JP" ? 0xD << 3 : 0xE << 3;
            trim_whitespace();
            try {
                auto bytes = jop::dataFrom(readValue());
                x.data.insert(x.data.end(), bytes.begin(), bytes.end());
            } catch (CompilerError &error) {
                x.labelRef = readIdentifier();
            }
        } else if (n == "OUT") {
            x.inst = 0xF << 3;
        } else if (n == "IN") {
            x.inst = 0x10 << 3;
        } else {
            cry("Expecting instruction");
        }
        trim_whitespace();
        return x;
    }

    std::string readIdentifier() {
        std::string x;

        if (isalpha(current_char) || current_char == '_') {
            x += current_char;
            nextChar();
            while(isalnum(current_char) || current_char == '_' ) {
                x+= current_char;
                nextChar();
            }
            return x;
        } else {
            cry("Expected identifier");
        }
        return x;
    }

    jop::byte readRegister() {
        auto s = readIdentifier();
        if (s.size() == 1) {
            auto reg = toupper(s[0]) - 65;
            if (reg < 8 && reg >= 0) {
                return static_cast<jop::byte>(reg);
            }
        }
        cry("Expected register");
        return 0;
    }

    jop::byte readByteValue() {
        return static_cast<jop::byte>(readValue());
    }

    int readValue() {
        int retint;
        if (current_char == '#') {
            *_f >> retint;
            nextChar();
        } else if (current_char == '0') {
            nextChar();
            if (current_char == 'x') {
                *_f >> std::hex >> retint;
                nextChar();
            } else {
                cry("Expecting 'x' for hex");
            }
        } else {
            cry("Expecting int type");
        }
        return retint;
    }

    void trim_whitespace() {
        while (isspace(current_char)) {
            nextChar();
        }
    }

    void cry(std::string msg) {
        msg += ", got ";
        msg += current_char;

        throw CompilerError(msg);
    }

    void nextChar() {

        if (!_f->eof()) {

            current_char = static_cast<char>(_f->get());
        } else {
            throw CompilerError("Reached end of file prematurely.");
        }
    }
private:
    std::shared_ptr<std::ifstream> _f;
    char current_char;
};

std::vector<jop::byte> compile(std::shared_ptr<std::ifstream> f) {
    compiler compiler1(std::move(f));
    auto insts = compiler1.compile();
    return jop::bytesFromData(insts);
}



CompilerError::CompilerError(std::string message) : message_(std::move(message)) {}
