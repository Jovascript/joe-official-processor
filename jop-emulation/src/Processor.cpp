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
            // Separate into register(if needed) and instruction.

            // Register is bottom byte
            auto reg = static_cast<unsigned char>(temp);
            if (reg > 7) {
                throw proc_err("Register value too large.");
            }

            // Instruction is 2nd byte, excluding last bit.
            auto inst = static_cast<Instruction>((temp >> 8) & 0b0111111);

            // Top bit is whether, if the instruction takes a mem address, it is from a register.
            bool addr_from_reg = static_cast<bool>((temp >> (8 + 7)) & 1);

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
                    registers[reg] = fetch_from_memory(data[0], addr_from_reg);
                    break;
                case Instruction::MVRM:
                    set_to_memory(data[0], registers[reg], addr_from_reg);
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
                    if (addr_from_reg) {
                        if (data[0] < 8) {
                            pc = registers[data[0]];
                        } else {
                            throw proc_err("Register too large.");
                        }
                    } else {
                        pc = data[0];
                    }
                    break;
                case Instruction::JPZ:
                    if (registers[0] == 0) {
                        if (addr_from_reg) {
                            if (data[0] < 8) {
                                pc = registers[data[0]];
                            } else {
                                throw proc_err("Register too large.");
                            }
                        } else {
                            pc = data[0];
                        }
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


    dtype Processor::fetch_from_memory(int address, bool from_reg) {
        if (from_reg) {
            if (address < 8) {

                address = registers[address];
            } else {
                throw proc_err("Register too large.");
            }
        }
        for (const auto &handler : handlers) {
            if (handler->holds(address)) {
                return handler->fetch(address);
            }
        }
        throw proc_err(boost::format("Memory out of Bounds - No memory device exists at address %X") % address);
    }

    void Processor::set_to_memory(int address, dtype data, bool from_reg) {
        if (from_reg) {
            if (address < 8) {
                address = registers[address];
            } else {
                throw proc_err("Register too large.");
            }
        }
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
