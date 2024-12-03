#include "solver.h"

#include <chrono>

Solver::Solver(const std::string& file, const std::string& board_file) : fillomino(board_file) {
    getGeneratedPolyominoes(file);

    rows = fillomino.getRows();
    cols = fillomino.getCols();
}

void Solver::getGeneratedPolyominoes(const std::string& filename) {
    polyominoes.resize(10);
    polyominoes[1].push_back("x");

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
    std::vector<std::vector<bool>> checked(rows, std::vector<bool>(cols, false));
    bool solved = false;
    if (!fillomino.validFromFile || !fillomino.areSizesValid() || !fillomino.isValid()) {
        std::cout << "incorrect fillomino\n";
        return;
    }

    fillomino.completeOneOption();

    if (fillomino.isSolved()) {
        std::cout << "solved\n";
        fillomino.print();
        return;
    }

    for (int cnt = 0; cnt < 8; cnt++) {
        for (uint8_t i = 0; i < rows; i++)
            for (uint8_t j = 0; j < cols; j++)
                if (fillomino.isCellAClue(i,j))
                    fillomino.crossSection(checked, polyominoes, i, j);

        if (fillomino.isSolved()) {
            std::cout << "solved\n";
            fillomino.print();
            return;
        }
    }

    for (uint8_t i = 0; i < rows; i++) {
        for (uint8_t j = 0; j < cols; j++) {
            if (fillomino.isCellAClue(i,j)) {
                const uint8_t n = fillomino.getNum(i,j);
                for (const auto& polyomino : polyominoes[n]) {
                    std::vector<std::vector<Cell>> board;
                    if (fillomino.processCode(polyomino,i,j,board)) {
                        solved = nextStep(i,j,board);
                        if (solved) {
                            return;
                        }
                    }
                }
            }
        }
    }

    bool s = completeEmptyCells(fillomino.board);
}

bool Solver::nextStep(uint8_t i, uint8_t j, const std::vector<std::vector<Cell>>& b) const {
    std::vector<std::vector<bool>> checked(rows, std::vector<bool>(cols, false));
    Fillomino f(rows, cols, b);

    bool solved = f.isSolved();
    if (solved) {
        std::cout << "solved\n";
        f.print();
        return true;
    }

    f.completeOneOption();
    for (uint8_t k = 0; k < rows; k++) {
        for (uint8_t l = 0; l < cols; l++) {
            if (f.isCellAClue(k,l))
                f.crossSection(checked,polyominoes,k,l);
        }
    }

    solved = f.isSolved();
    if (solved) {
        std::cout << "solved\n";
        f.print();
        return true;
    }

    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            if (f.isCellAClue(i,j)) {
                const uint8_t n = f.getNum(i,j);
                for (const auto& polyomino : polyominoes[n]) {
                    std::vector<std::vector<Cell>> board;
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

    bool s = completeEmptyCells(f.board);
    return s;
}

bool Solver::completeEmptyCells(const std::vector<std::vector<Cell>>& b) const {
    Fillomino f(rows, cols, b);
    bool solved = f.isSolved();
    if (solved) {
        std::cout << "solved\n";
        f.print();
        return true;
    }

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (f.getNum(i,j) == 0) {
                const int n = f.getPartialSize(b, i, j);
                int s = n;
                if (n > 9) {
                    s = 9;
                }
                for (s; s >= 1; --s) {
                    for (const auto& polyomino : polyominoes[s]) {
                        std::vector<std::vector<Cell>> board;
                        if (f.processCode(polyomino, i, j, board, s)) {
                            solved = completeEmptyCells(board);
                            if (!solved) {
                                continue;
                            }
                            return true;
                        }
                    }
                }
                return false;
            }
        }
    }
    return true;
}
