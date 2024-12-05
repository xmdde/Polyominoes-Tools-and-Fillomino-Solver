#include "generator.h"
#include "fillomino.h"
#include "solver.h"

#include <chrono>
#include <iostream>
#include <string>

int main(int argc, const char* argv[]) {
    if (argc < 2) {
        std::cerr << "Expected input file\nUsage: ./poly [fillomino_file polyomino_file | -g polyomino_file]\n";
        return 1;
    }

    if (std::string(argv[1]).compare("-g") == 0) {
        Generator gen(9, argv[2]);
        gen.generate();
    } else {
        Solver solver(argv[2], argv[1]);
        auto start = std::chrono::high_resolution_clock::now();
        solver.solve();
        auto stop = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = stop - start;
        std::cout << elapsed.count() << "\n";
    }
    return 0;
}
