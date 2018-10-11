//
// Created by Joseph on 09/09/2018.
//

#include "Processor.h"
#include <boost/format.hpp>
#include "helpers.h"


namespace jop {

    void Processor::add_memory(std::shared_ptr<MemoryHandler> handler) {
        for (const auto &m: handlers) {
            if (m->intersects(handler.get())) {
                throw "Memory Collision";
            }
        }
        handlers.push_back(handler);
    }

    void Processor::run() {
        bool halted = false;
        while (!halted) {
            // Get new instruction
            dtype temp = fetch_from_memory(pc);
            // Separate into register(if needed) and instruction(0-31) - will extend if needed.
            auto reg = static_cast<dtype>(temp & 0b00000111);
            auto inst = static_cast<Instruction>(temp >> 3);
            // Increment program counter
            pc++;

            std::vector<dtype> data;
            if (instruction_requires_register(inst)) {
                // Get the byte
                dtype d = fetch_from_memory(pc);
                if (d < 8) {
                    data.push_back(d);
                } else {
                    throw proc_err(boost::format("Unknown register %d") % d);
                }
                pc++;
            }
            for (int i = 0; i < instruction_requires_data(inst); i++) {
                // Get another byte
                data.push_back(fetch_from_memory(pc));
                pc++;
            }

            switch (inst) {
                case Instruction::NOP:
                    break;
                case Instruction::HLT:
                    halted = true;
                    break;
                case Instruction::MVMR:
                    registers[reg] = fetch_from_memory(addressFromData(data));
                    break;
                case Instruction::MVRM:
                    set_to_memory(addressFromData(data), registers[reg]);
                    break;
                case Instruction::MVRR:
                    registers[reg] = registers[data[0]];
                    break;
                case Instruction::MVLR:
                    registers[reg] = data[0];
                    break;
                case Instruction::ADD:
                    registers[0] += registers[reg];
                    break;
                case Instruction::SUB:
                    registers[0] -= registers[reg];
                    break;
                case Instruction::MUL:
                    registers[0] *= registers[reg];
                    break;
                case Instruction::DIV:
                    registers[0] /= registers[reg];
                    break;
                case Instruction::AND:
                    registers[0] &= registers[reg];
                    break;
                case Instruction::OR:
                    registers[0] |= registers[reg];
                    break;
                case Instruction::NOT:
                    registers[0] = ~registers[0];
                    break;
                case Instruction::JP:
                    pc = addressFromData(data);
                    break;
                case Instruction::JPZ:
                    if (registers[0] == 0) {
                        pc = addressFromData(data);
                    }
                    break;
                case Instruction::OUT:
                    iohandler->handle_output(registers[0]);
                    break;
                case Instruction::INP:
                    registers[0] = iohandler->handle_input();
                    break;
            }

        }
    }


    dtype Processor::fetch_from_memory(int address) {
        for (const auto &handler : handlers) {
            if (handler->holds(address)) {
                return handler->fetch(address);
            }
        }
        throw proc_err(boost::format("Memory out of Bounds - No memory device exists at address %X") % address);
    }

    void Processor::set_to_memory(int address, dtype data) {
        for (auto &handler: handlers) {
            if (handler->holds(address)) {
                handler->write(address, data);
                return;
            }
        }
        throw proc_err(boost::format("Memory out of Bounds - No memory device exists at address %X") % address);
    }


    int instruction_requires_data(Instruction m) {
        switch (m) {
            case Instruction::MVMR:
            case Instruction::MVRM:
            case Instruction::JP:
            case Instruction::JPZ:
                return sizeof(addrtype)/ sizeof(dtype);
            case Instruction::MVLR:
                return 1;
            default:
                return 0;
        }
    }

    void Processor::set_io_handler(std::shared_ptr<InputOutputHandler> handler) {
        iohandler = std::move(handler);

    }


    bool instruction_requires_register(Instruction m) {
        switch (m) {
            case Instruction::MVRR:
                return true;
            default:
                return false;
        }
    }


}
