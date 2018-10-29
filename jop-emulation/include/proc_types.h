//
// Created by Joseph on 22/09/2018.
//

#ifndef JOP_PROC_TYPES_H
#define JOP_PROC_TYPES_H

#include <vector>
#include <limits>

namespace jop {
    typedef signed short dtype;
    typedef unsigned char byte;

    const long MAX_DTYPE = std::numeric_limits<dtype>::max();

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

    std::vector<dtype> dataFromBytes(const std::vector<byte> &bytes);
    std::vector<byte> bytesFromData(const std::vector<dtype> &data);

}

#endif //JOP_PROC_TYPES_H
