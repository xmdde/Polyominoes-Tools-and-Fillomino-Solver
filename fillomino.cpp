#include "fillomino.h"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "cell.h"

uint8_t Fillomino::getPartialSize(const uint8_t i, const uint8_t j) const {
    std::vector<std::vector<bool>> used(rows, std::vector<bool>(cols, false));
    used[i][j] = true;
    uint8_t size = 1;
    getPartialSizeHelp(size, used, i, j);
    return size;
}

void Fillomino::getPartialSizeHelp(uint8_t& size, std::vector<std::vector<bool>>& used, const uint8_t i, const uint8_t j) const {
    if (isInBounds(i-1, j) && !used[i-1][j] && board[i-1][j] == board[i][j]) { // ^
        size++;
        used[i-1][j] = true;
        getPartialSizeHelp(size, used, i-1, j);
    }
    if (isInBounds(i, j+1) && !used[i][j+1] && board[i][j+1] == board[i][j]) { // >
        size++;
        used[i][j+1] = true;
        getPartialSizeHelp(size, used, i, j+1);
    }
    if (isInBounds(i+1, j) && !used[i+1][j] && board[i+1][j] == board[i][j]) { // d
        size++;
        used[i+1][j] = true;
        getPartialSizeHelp(size, used, i+1, j);
    }
    if (isInBounds(i, j-1) && !used[i][j-1] && board[i][j-1] == board[i][j]) { // <
        size++;
        used[i][j-1] = true;
        getPartialSizeHelp(size, used, i, j-1);
    }
}

void Fillomino::getBoardFromFile(const std::string& file) {
    std::ifstream f(file);
    //if (!f.is_open()) {}

    f >> rows;
    f >> cols;
    board.resize(rows);

    int n;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            f >> n;
            board[i].push_back(Cell(n));
        }
    }
    f.close();

    // to mozna przyspieszyc
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (board[i][j].getNum() != 0) {
                uint8_t size = getPartialSize(i, j);
                board[i][j].updateGroupSize(size);
            }
        }
    }
}

bool Fillomino::isInBounds(const uint8_t i, const uint8_t j) const {
    return i >= 0 && i < rows && j >= 0 && j < cols;
}

void Fillomino::print() const {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            std::cout << static_cast<int>(board[i][j].getNum()) << ' ';
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            std::cout << static_cast<int>(board[i][j].getGroupSize()) << ' ';
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            std::cout << board[i][j].finished << ' ';
        }
        std::cout << std::endl;
    }
}
