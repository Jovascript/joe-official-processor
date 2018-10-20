//
// Created by Joseph on 09/09/2018.
//

#include "Processor.h"
#include <boost/format.hpp>
#include <iostream>
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
            // reg is only used once checked, so no need to check now
            auto reg = static_cast<unsigned char>(temp);

            // Instruction is 2nd byte, excluding last bit.
            auto inst = static_cast<Instruction>((temp >> 8) & 0b0111111);

            // Top bit is whether, if the instruction takes a mem address, it is from a register.
            bool addr_from_reg = static_cast<bool>((temp >> (8 + 7)) & 1);

            // Increment program counter
            pc++;

            std::vector<dtype> data;

            // MVRR takes a register
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
                    set_register(reg, fetch_from_memory(data[0], addr_from_reg));
                    break;
                case Instruction::MVRM:
                    set_to_memory(data[0], get_register_value(reg), addr_from_reg);
                    break;
                case Instruction::MVRR:
                    set_register(reg, get_register_value(data[0]));
                    break;
                case Instruction::MVLR:
                    set_register(reg, data[0]);
                    break;
                case Instruction::ADD:
                    registers[0] += get_register_value(reg);
                    break;
                case Instruction::SUB:
                    registers[0] -= get_register_value(reg);
                    break;
                case Instruction::MUL:
                    registers[0] *= get_register_value(reg);
                    break;
                case Instruction::DIV:
                    registers[0] /= get_register_value(reg);
                    break;
                case Instruction::AND:
                    registers[0] &= get_register_value(reg);
                    break;
                case Instruction::OR:
                    registers[0] |= get_register_value(reg);
                    break;
                case Instruction::NOT:
                    registers[0] = ~registers[0];
                    break;
                case Instruction::JP:
                    if (addr_from_reg) {
                        pc = get_register_value(data[0]);

                    } else {
                        pc = data[0];
                    }
                    break;
                case Instruction::JPZ:
                    if (registers[0] == 0) {
                        if (addr_from_reg) {
                                pc = get_register_value(data[0]);
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
                case Instruction::PUSH:
                    set_to_memory(sp, get_register_value(reg));
                    sp--;
                    break;
                case Instruction::POP:
                    sp++;
                    set_register(reg, fetch_from_memory(sp));
                    break;
            }

        }
    }


    dtype Processor::fetch_from_memory(int address, bool from_reg) {
        if (from_reg) {
            address = get_register_value(address);
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
            address = get_register_value(address);
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
            case Instruction::MVRR:
                return 1;
            default:
                return 0;
        }
    }

    void Processor::set_io_handler(std::shared_ptr<InputOutputHandler> handler) {
        iohandler = std::move(handler);

    }

    dtype Processor::get_register_value(int reg) {
        if (reg > 8) {
            throw proc_err(boost::format("Reference to out-of-bounds register %u") % reg);
        } else {
            if (reg == 7) {
                // PC
                return static_cast<dtype>(pc);
            } else if (reg == 8) {
                // SP
                return sp;
            }
            else {
                return registers[reg];
            }
        }
    }

    void Processor::set_register(int reg, dtype value) {
        if (reg > 8) {
            throw proc_err(boost::format("Reference to out-of-bounds register %u") % reg);
        } else {
            if (reg == 7) {
                // PC
                pc = value;
            } else if (reg == 8) {
                // SP
                sp = value;
            } else {
                registers[reg] = value;
            }
        }
    }



}
