//
// Created by Joseph on 22/09/2018.
//

#include "proc_types.h"
#include "helpers.h"
#include <boost/format.hpp>

namespace jop {

    std::vector<dtype> dataFromBytes(const std::vector<byte> &bytes) {
        if (bytes.size() % data_byte_scale != 0) {
            throw proc_err("Data/byte vector length mismatch");
        } else {
            std::vector<dtype> data;
            data.reserve(bytes.size()/data_byte_scale);
            int j, i;
            dtype temp;
            for (i = 0; i < bytes.size(); i+=data_byte_scale) {
                temp = 0;
                for (j=0; j < data_byte_scale; j++) {
                    temp <<= 8;
                    temp |= bytes[i + j];
                }
                data.push_back(temp);
            }
            return data;
        }
    }

    std::vector<byte> bytesFromData(const std::vector<dtype> &data) {
        std::vector<byte> bytes;
        bytes.reserve(data.size() * data_byte_scale);
        int i;
        for (auto snum: data) {
            for (i = data_byte_scale-1; i >= 0; i--) {
                bytes.push_back(static_cast<byte>((snum & (0xFF << (i * 8))) >> (8 * i)));
            }
        }
        return bytes;
    }





}
