#ifndef UNISIMLIB_TEST_H
#define UNISIMLIB_TEST_H

namespace UniSim {

struct Input {
    int start, length;
};

struct Output {
    int *sequence;
    int size;
};

extern "C" Output __declspec(dllimport) run1(Input input);

extern "C" void __declspec(dllimport) run2(const Input *input, Output *output); // Caller owns both pointers

}

#endif 
