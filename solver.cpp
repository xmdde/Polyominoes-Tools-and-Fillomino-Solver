#include "solver.h"

void Solver::test() {
    fillomino.print();
    for (uint8_t i = 0; i < rows; i++)
        for (uint8_t j = 0; j < cols; j++)
            if (fillomino.isCellAClue(i,j))
                fillomino.crossSection(polyominoes,i,j);
    fillomino.print();
}

void Solver::getGeneratedPolyominoes(const std::string& filename) {
    polyominoes.resize(9);
    
    std::ifstream file(filename);
    int size;
    std::string text;

    while (file >> size && std::getline(file, text)) {
        if (!text.empty() && text[0] == ' ') {
            text = text.substr(1);
        }
        polyominoes[size].push_back(text);
    }
    file.close();
}

void Solver::solve() {
    bool solved = false;
    fillomino.completeOneOption();
    for (uint8_t i = 0; i < rows; i++)
        for (uint8_t j = 0; j < cols; j++)
            if (fillomino.isCellAClue(i,j))
                fillomino.crossSection(polyominoes,i,j);
    //fillomino.print();

    for (uint8_t i = 0; i < rows; i++) {
        for (uint8_t j = 0; j < cols; j++) {
            if (fillomino.isCellAClue(i,j)) {
                const uint8_t n = fillomino.getNum(i,j);
                for (const auto& polyomino : polyominoes[n]) {
                    std::vector<std::vector<Cell>> board;
                    if (fillomino.processCode(polyomino,i,j,board)) {
                        solved = nextStep(i,j,board);
                        if (solved) {
                            std::cout << "udalo sie!\n";
                            return;
                        }
                    }
                }
            }
        }
    }
}

bool Solver::nextStep(uint8_t i, uint8_t j, const std::vector<std::vector<Cell>>& b) const {
    //std::cout << "next_step(" << static_cast<int>(i) << ',' << static_cast<int>(j) << ")\n";
    Fillomino f(rows, cols, b);
    f.completeOneOption();
    // f.print();
    // std::cout << '\n';

    bool solved = f.isSolved();  // na teraz
    if (solved) {
        std::cout << "normalnie rozwiazane\n";
        f.print();
        return true;
    }

    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            //std::cout << "i,j: " << static_cast<int>(i) << ',' << static_cast<int>(j) << "\n";
            if (f.isCellAClue(i,j)) {
                const uint8_t n = f.getNum(i,j);
                for (const auto& polyomino : polyominoes[n]) {
                    // std::cout << static_cast<int>(n) << ' ' << polyomino << '\n';
                    std::vector<std::vector<Cell>> board;
                    // std::cout << static_cast<int>(i) << ',' << static_cast<int>(j) << '\n';
                    if (f.processCode(polyomino, i, j, board)) {
                        solved = nextStep(i, j, board);
                        if (!solved) {
                            continue;
                        }
                        return true;
                    }
                }
                return false;
            }
        }
    }

    bool s = f.isSolved();
    return s;
}
