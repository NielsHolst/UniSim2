#ifndef UNISIMLIB_TEST_H
#define UNISIMLIB_TEST_H
#include <vector>

namespace UniSim {

struct Input {
    int start, length;
};

struct Output {
    int *sequence;
    int size;
};

extern "C" Output __declspec(dllexport) run1(Input input);

extern "C" void __declspec(dllexport) run2(const Input *input, Output *output); // Caller owns both pointers

}

#endif 
