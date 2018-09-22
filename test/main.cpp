//
// Created by Joseph on 22/09/2018.
//
#include <jopemulation.h>
#include <iostream>

int main() {
    int x = 400;
    auto m = jop::dataFrom(x);
    for (jop::dtype d: m) {
        std::cout << int(d) << std::endl;
    }

    auto y = jop::addressFromData(m);
    std::cout << y << std::endl;

    return 0;
}

