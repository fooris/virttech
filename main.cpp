#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <sys/time.h>

#include "code_generator.h"
#include "VM.h"
#include "interpreters/NaiveVM.h"
#include "interpreters/IndirectVM.h"
#include "interpreters/DirectVM.h"
#include "interpreters/DirectSuperVM.h"
#include "code_generation/CodeGeneratorVM.h"

#define NUMBER_RUNS 1

struct scenario {
    unsigned int size;
    int seed;
    const int probabilities[5];
};

double measure_run(VM &&vm);

int _main() {
    int size = 16;
    char program[]{
        OP_INC3A,
        OP_DECA,
        OP_SETL,
        OP_DECA,
        OP_BACK7,
        OP_SETL,
        OP_CLRA,

        OP_INC3A,
        OP_INC3A,
        OP_INC3A,
        OP_INC3A,
        OP_INC3A,

        OP_INC3A,
        OP_BACK7,
        OP_DECA,
        OP_HALT
    };

    CodeGeneratorVM vm(program, size);
    std::cout << "run" << std::endl;
    vm.run();
    std::cout << vm.getA() << std::endl;
    std::cout << vm.getL() << std::endl;
}

int main() {
    std::vector<scenario> scenarios = {
            {10000, 1, {0, 1, 0, 0, 0},},
            {10000, 1, {1, 1, 1, 0, 0},},

            {10000, 1, {1, 9, 1, 5, 5},},
            {10000, 2, {1, 9, 1, 5, 5},},
            {10000, 3, {1, 9, 1, 5, 5},},

            {50000, 1, {1, 9, 1, 5, 5},},
            {50000, 2, {1, 9, 1, 5, 5},},
            {50000, 3, {1, 9, 1, 5, 5},},
    };

    int nr = 0;
    std::cout << "> Running benchmark for " << scenarios.size() << " scenarios at " << NUMBER_RUNS << " repetitions."
              << std::endl << std::endl;
    std::cout << "s\tnaive\t\t\t\tindirect\t\t\tdirect\t\t\t\tdirect_super\t\tCodeGeneratorVM" << std::endl;

    for (auto s : scenarios) {
        nr++;
//        if (nr != 6) {
//            continue;
//        }

        char *program = new char[s.size];
        init(program, s.size, s.probabilities, s.seed);

        std::vector<double> measurements;
        measurements.push_back(measure_run(NaiveVM{program, s.size}));
        measurements.push_back(measure_run(IndirectVM{program, s.size}));
        measurements.push_back(measure_run(DirectVM{program, s.size}));
        measurements.push_back(measure_run(DirectSuperVM{program, s.size}));
        measurements.push_back(measure_run(CodeGeneratorVM{program, s.size}));

        std::cout << nr << ":\t";
        for (auto m : measurements) {
            std::cout << std::fixed << m << std::setprecision(6) << " seconds";
            std::cout << (m == *std::min_element(measurements.begin(), measurements.end()) ? " <-" : "") << "\t";
        }
        std::cout << std::endl;

        delete[] program;
    }

    return 0;
}

double measure_run(VM &&vm) {
    struct timeval start, end;
    gettimeofday(&start, nullptr);
    for (int i = 0; i < NUMBER_RUNS; ++i) {
        vm.reset();
        vm.run();
    }
    gettimeofday(&end, nullptr);

//    std::cout << vm.getName() << ": a=" << vm.getA() << " l=" << vm.getL() << " ip=" <<  vm.getIP()  << std::endl;

    double time_taken;
    time_taken = ((double) end.tv_sec - start.tv_sec) * 1e6;
    time_taken = (time_taken + ((double) end.tv_usec - start.tv_usec)) * 1e-6;
    return time_taken;
}

