#include "fillomino.h"

void Fillomino::getPartialSize(uint8_t& size, const uint8_t i, const uint8_t j, const uint8_t pre) const {
    if (isInBounds(i-1, j) && pre != 3 && board[i-1][j] == board[i][j]) {
        size++;
        getPartialSize(size++, i-1, j, 1);
    }
    if (isInBounds(i, j+1) && pre != 4 && board[i][j+1] == board[i][j]) {
        size++;
        getPartialSize(size++, i, j+1, 2);
    }
    if (isInBounds(i+1, j) && pre != 1 && board[i+1][j] == board[i][j]) {
        size++;
        getPartialSize(size++, i+1, j, 3);
    }
    if (isInBounds(i, j-1) && pre != 2 && board[i][j-1] == board[i][j]) {
        size++;
        getPartialSize(size++, i, j-1, 4);
    }
}

bool Fillomino::isInBounds(const uint8_t i, const uint8_t j) const {
    return i >= 0 && i < rows && j >= 0 && j < cols;
}