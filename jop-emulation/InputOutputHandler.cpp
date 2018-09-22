//
// Created by Joseph on 12/09/2018.
//

#include "InputOutputHandler.h"
#include "proc_types.h"
#include <iostream>
#include <limits>
namespace jop {
    dtype StandardIOHandler::handle_input() {
        while (true) {
            int x;
            std::cout << "Input: ";
            scanf("%d", &x);
            if (x > std::numeric_limits<dtype>::max()) {
                std::cout << "Too large. Try again." << std::endl;
            } else {
                return static_cast<dtype>(x);
            }
            std::cout << "Bad byte. Try again." << std::endl;
        }
    }

    void StandardIOHandler::handle_output(dtype x) {
        printf("%X", x);
    }
}