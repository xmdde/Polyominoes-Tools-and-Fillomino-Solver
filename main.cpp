#include "generator.h"
#include "fillomino.h"
#include "solver.h"

#include <iostream>

int main(int argc, const char* argv[]) {
    if (argc < 2) {
        std::cerr << "Expected input file\nUsage: ./poly file_name\n";
        return 1;
    }

    // Generator gen(8, "boards/codes.txt");
    // gen.generate();
/*
    Fillomino f(argv[1]);
    std::cout << f.isValid() << std::endl;
    std::cout << f.processCode("3x2", 0, 2);
*/
    // f.print();
    Solver solver("boards/codes.txt");
    solver.tescik();
    return 0;
}
