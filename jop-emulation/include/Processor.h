//
// Created by Joseph on 09/09/2018.
//

#ifndef JOP_PROCESSOR_H
#define JOP_PROCESSOR_H

#include <memory>
#include "InputOutputHandler.h"
#include "MemoryHandler.h"
#include "proc_types.h"


namespace jop {

    enum class Instruction {
        NOP,
        HLT,
        MVMR, // Move Memory to Register
        MVRM, // Move Register to Memory
        MVRR, // Move Register to Register
        MVLR, // Move Literal to Register
        ADD,
        SUB,
        MUL,
        DIV,
        AND,
        OR,
        NOT,
        JP,
        JPZ,
        OUT,
        INP,
        PUSH,
        POP,
        JPNZ,
        JPP,
        JPN
    };

    class Processor {
    public:
        void set_io_handler(std::shared_ptr<InputOutputHandler> handler);

        void add_memory(std::shared_ptr<MemoryHandler> handler);

        void run();

    private:
        dtype fetch_from_memory(int address, bool from_reg = false);

        void set_to_memory(int address, dtype data, bool from_reg = false);

        dtype get_register_value(int reg);

        void set_register(int reg, dtype value);

        dtype registers[7];
        std::vector<std::shared_ptr<MemoryHandler>> handlers;
        std::shared_ptr<InputOutputHandler> iohandler;

        unsigned int pc{0};
        dtype sp{0};

    };

    int instruction_requires_data(Instruction m);

}
#endif //JOP_PROCESSOR_H
