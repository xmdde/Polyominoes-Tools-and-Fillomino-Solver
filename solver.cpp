#include "solver.h"

#include <chrono>

Solver::Solver(const std::string& file, const std::string& board_file) : fillomino(board_file) {
    auto start = std::chrono::high_resolution_clock::now();
    getGeneratedPolyominoes(file);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    //std::cout << "Czas wykonania: " << duration.count() << "\n";

    rows = fillomino.getRows();
    cols = fillomino.getCols();
}

void Solver::getGeneratedPolyominoes(const std::string& filename) {
    polyominoes.resize(10);

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
    fillomino.completeOneOption();

    for (int cnt = 0; cnt < 5; cnt++) {
        if (fillomino.isSolved()) {  // na teraz
            std::cout << "rozwiazane\n";
            fillomino.print();
            return;
        }

        for (uint8_t i = 0; i < rows; i++)
            for (uint8_t j = 0; j < cols; j++)
                if (fillomino.isCellAClue(i,j))
                    fillomino.crossSection(checked,polyominoes,i,j);
    }
    // fillomino.certainCells(polyominoes);

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
    std::vector<std::vector<bool>> checked(rows, std::vector<bool>(cols, false));
    Fillomino f(rows, cols, b);
    f.completeOneOption();

    for (uint8_t k = 0; k < rows; k++)
        for (uint8_t l = 0; l < cols; l++)
            if (f.isCellAClue(k,l))
                f.crossSection(checked,polyominoes,k,l);

    bool solved = f.isSolved();  // na teraz
    if (solved) {
        std::cout << "normalnie rozwiazane\n";
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

    bool s = f.isSolved();
    return s;
}
