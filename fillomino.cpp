#include "fillomino.h"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

uint8_t Fillomino::getPartialSize(const uint8_t i, const uint8_t j) const { // dodaj inne wspolrzedne?
    std::vector<std::vector<bool>> used(rows, std::vector<bool>(cols, false));
    used[i][j] = true;
    uint8_t size = 1;
    getPartialSizeHelp(size, used, i, j);
    return size;
}

void Fillomino::getPartialSizeHelp(uint8_t& size, std::vector<std::vector<bool>>& used, const uint8_t i,
                                   const uint8_t j) const {
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

// to check + puste/unfinished pole odciete
bool Fillomino::isValid() const { 
    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j++) {
            if (board[i][j].getNum() == 0) {
                if (isCellBlocked(board, i, j))
                    return false;
                break;
            }
            if (isInBounds(i,j+1) && board[i][j] == board[i][j+1] && board[i][j].finished != board[i][j+1].finished)
                return false;
            if (isInBounds(i+1,j) && board[i][j] == board[i+1][j] && board[i][j].finished != board[i+1][j].finished)
                return false;
        }
    }
    return true;
}

bool Fillomino::isValid(const std::vector<std::vector<Cell>>& b) const {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (b[i][j].getNum() == 0) {
                if (isCellBlocked(b, i, j)) {
                    return false;
                }
            } else {
                if (isInBounds(i,j+1) && b[i][j] == b[i][j+1] && b[i][j].finished != b[i][j+1].finished)
                    return false;
                if (isInBounds(i+1,j) && b[i][j] == b[i+1][j] && b[i][j].finished != b[i+1][j].finished)
                    return false;
            }
        }
    }
    return true;
}

bool Fillomino::processCode(const std::string& code, uint8_t i_idx, uint8_t j_idx) const {  // +vec
    std::vector<std::vector<Cell>> boardCopy = board;
    boardCopy[i_idx][j_idx].finished = true;
    const uint8_t n = board[i_idx][j_idx].getNum();
    const size_t pos = code.find('x');
    std::string sub1 = code.substr(0, pos);
    std::string sub2 = code.substr(pos + 1);

    uint8_t i = i_idx;
    uint8_t j = j_idx;
    uint8_t new_i;
    uint8_t new_j;
    for (int idx = sub1.size() - 1; idx >= 0; --idx) {
        getNewCoords(new_i, new_j, i, j, sub1[idx]);
        // std::cout << '(' << static_cast<int>(new_i) << ',' << static_cast<int>(new_j) << "), ";
        // moze zmiana warunku po prostu?
        if (!isInBounds(new_i, new_j) || (board[new_i][new_j].getNum() != n && board[new_i][new_j].getNum() != 0) || board[new_i][new_j].finished) { // || empty
            return false;
        }

        boardCopy[new_i][new_j].setNum(n);
        boardCopy[new_i][new_j].finished = true;

        i = new_i;
        j = new_j;
    }

    i = i_idx;
    j = j_idx;

    for (const auto& c : sub2) {
        getNewCoords(new_i, new_j, i, j, c);
        // std::cout << '(' << new_i << ',' << new_j << "), ";

        if (!isInBounds(new_i, new_j) || (board[new_i][new_j].getNum() != n && board[new_i][new_j].getNum() != 0) || board[new_i][new_j].finished)
            return false;

        boardCopy[new_i][new_j].setNum(n);
        boardCopy[new_i][new_j].finished = true;

        i = new_i;
        j = new_j;
    }
    return isValid(boardCopy);
}

// returns true if cell is unfinished and all neighbours are finished (+min. one neighbour is 1)
bool Fillomino::isCellBlocked(const std::vector<std::vector<Cell>>& b, const uint8_t i, const uint8_t j) const {
    if ((isInBounds(i+1, j) && !b[i+1][j].finished) || (isInBounds(i-1, j) && !b[i-1][j].finished)
        || (isInBounds(i, j+1) && !b[i][j+1].finished) || (isInBounds(i, j-1) && !b[i][j-1].finished))
        return false;
    if ((isInBounds(i+1, j) && b[i+1][j].getNum() == 1) || (isInBounds(i-1, j) && b[i-1][j].getNum() == 1)
        || (isInBounds(i, j+1) && b[i][j+1].getNum() == 1) || (isInBounds(i, j-1) && b[i][j-1].getNum() == 1))
        return true;
    return false;
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
