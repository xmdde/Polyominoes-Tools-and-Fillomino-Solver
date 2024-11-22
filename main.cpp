#include "generator.h"
#include "fillomino.h"
#include "solver.h"

#include <iostream>
#include <string>

int main(int argc, const char* argv[]) {
    if (argc < 2) {
        std::cerr << "Expected input file\nUsage: ./poly file_name\n";
        return 1;
    }

    if (std::string(argv[1]).compare("-g") == 0) {
        Generator gen(9, "boards/codes.txt");
        gen.generate();
    } else {
        Solver solver("boards/codes.txt", argv[1]);
        solver.solve();
    }
    return 0;
}
