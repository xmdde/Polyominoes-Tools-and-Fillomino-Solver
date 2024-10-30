#include "generator.h"
#include "fillomino.h"

#include <iostream>

int main(int argc, const char* argv[]) {
    if (argc < 2) {
        std::cerr << "Expected input file\nUsage: ./poly file_name\n";
        return 1;
    }

    Generator gen(8, "boards/codes.txt");
    gen.generate();
    // Fillomino f(argv[1]);
    // f.print();
    return 0;
}
