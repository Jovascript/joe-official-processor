//
// Created by Joseph on 22/09/2018.
//

#include <boost/format.hpp>
#include "proc_types.h"
#include "constants.h"
#include <iostream>

namespace jop {
    addrtype addressFromData(std::vector<dtype> input_data) {
        if (input_data.size() > sizeof(addrtype)) {
            throw proc_err(boost::format("Address cannot be length %u.") % input_data.size());
        } else {
            addrtype x = 0;
            for (dtype n: input_data) {
                x <<= 8* sizeof(dtype); // 8 Bits to byte
                x |= n;
            }
            return x;
        }
    }

    std::vector<dtype> dataFromAddress(addrtype address) {
        std::vector<dtype> returnee(static_cast<unsigned long long int>(addr_data_scale));
        for (int i = 0; i < addr_data_scale; i++)
            returnee[addr_data_scale - 1 - i] = static_cast<dtype>(address >> (sizeof(dtype) * i * 8));
        return returnee;
    }

    std::vector<dtype> dataFromBytes(std::vector<byte> bytes) {
        if (bytes.size() % data_byte_scale != 0) {
            throw proc_err("Data/byte vector length mismatch");
        } else {
            std::vector<dtype> data;
            int j;
            for (int i = 0; i < bytes.size(); i+=data_byte_scale) {
                data.push_back(0);
                for (j=0; j < data_byte_scale; j++) {
                    data[i] <<= 8;
                    data[i] |= bytes[i*data_byte_scale + j];
                }
            }
            return data;
        }
    }

    std::vector<byte> bytesFromData(std::vector<dtype> data) {
        std::vector<byte> bytes(data.size()*data_byte_scale);
        for (int j = 0; j < data.size(); j++) {
            for (int i = 0; i < data_byte_scale; i++) {
                bytes[j*data_byte_scale + (data_byte_scale - 1 - i)] = static_cast<byte>(data[j] >> (i * 8));
            }
        }
        return bytes;
    }





}
