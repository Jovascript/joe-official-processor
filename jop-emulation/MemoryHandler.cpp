//
// Created by Joseph on 11/09/2018.
//

#include "MemoryHandler.h"
#include "constants.h"
#include <iostream>

namespace jop {
    MemoryHandler::MemoryHandler(int start_address, int length) : start_address(start_address) {
        // End address is the final address in the memory store
        end_address = start_address + length - 1;

    }

    bool MemoryHandler::holds(int address) {
        return address >= start_address && address <= end_address;
    }

    bool MemoryHandler::intersects(int start_adr, int end_adr) {
        if (start_adr >= start_address && start_adr <= end_address) {
            return true;
        } else if (end_adr >= start_address && end_adr <= end_address) {
            return true;
        }
        return false;
    }

    bool MemoryHandler::intersects(MemoryHandler *mem) {
        return mem->intersects(start_address, end_address);
    }


    RAMSegment::RAMSegment(int start_address, int length) : MemoryHandler(start_address, length) {
        mem = std::make_unique<dtype[]>(static_cast<size_t>(length));
    }


    dtype RAMSegment::fetch(int address) {
        if (holds(address)) {

            return mem[address - start_address];

        } else {
            throw proc_err("Memory Reference out of bounds.");
        }
    }

    void RAMSegment::write(int address, dtype value) {
        if (holds(address)) {
            mem[address - start_address] = value;
        } else {
            throw proc_err("Memory reference out of bounds.");
        }
    }

    void RAMSegment::load(int len, dtype *data) {
        for (int i = 0; i < len; i++) {
            mem[i] = data[i];
        }
    }
}

