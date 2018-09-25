//
// Created by Joseph on 11/09/2018.
//

#ifndef JOP_MEMORYHANDLER_H
#define JOP_MEMORYHANDLER_H

#include <memory>
#include "proc_types.h"

namespace jop {

    class MemoryHandler {
    public:
        MemoryHandler(int start_address, int length);

        bool holds(int address);

        bool intersects(int start_adr, int end_adr);

        bool intersects(MemoryHandler *mem);

        virtual dtype fetch(int address) = 0;

        virtual void write(int address, dtype value) = 0;


    protected:
        int end_address;


    protected:
        int start_address;
    };


    class RAMSegment : public MemoryHandler {
    public:
        RAMSegment(int start_address, int length);

        void load(int len, dtype data[]);

        dtype fetch(int address) override;

        void write(int address, dtype value) override;

    private:
        std::unique_ptr<dtype[]> mem;
    };
}
#endif //JOP_MEMORYHANDLER_H
