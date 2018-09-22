//
// Created by Joseph on 22/09/2018.
//

#ifndef JOP_PROC_TYPES_H
#define JOP_PROC_TYPES_H

#include <vector>
#include <math.h>

namespace jop {
    typedef unsigned char dtype;
    typedef int addrtype;
    typedef unsigned char byte;

    const int addr_data_scale = sizeof(addrtype)/ sizeof(dtype);
    const int data_byte_scale = sizeof(dtype)/ sizeof(byte);

    template <typename T>
    std::vector<dtype> dataFrom(T inp) {

        int ltimes = sizeof(T) / sizeof(dtype);
        std::vector<dtype> data(static_cast<unsigned long long int>(ltimes));
        for (int i = 0; i < ltimes; i++) {
            data[ltimes-1-i] = inp >> (i * 8 * sizeof(dtype));
        }
        return data;
    }

    addrtype addressFromData(std::vector<dtype> input_data);
    std::vector<dtype> dataFromAddress(addrtype address);

    std::vector<dtype> dataFromBytes(std::vector<byte> bytes);
    std::vector<byte> bytesFromData(std::vector<dtype> data);

}

#endif //JOP_PROC_TYPES_H
