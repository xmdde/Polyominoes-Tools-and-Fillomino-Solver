#include "fillomino.h"

#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <vector>

bool Fillomino::isInBounds(const uint8_t i, const uint8_t j) const {
    return i >= 0 && i < rows && j >= 0 && j < cols;
}

int Fillomino::getPartialSize(const uint8_t i, const uint8_t j, std::vector<std::pair<uint8_t, uint8_t>>& area) const {  // dodaj inne wspolrzedne?
    std::vector<std::vector<bool>> used(rows, std::vector<bool>(cols, false));
    used[i][j] = true;
    int size = 1;
    getPartialSizeHelp(board, size, used, i, j);
    for (uint8_t r = 0; r < rows; r++) {
        for (uint8_t c = 0; c < cols; c++) {
            if (used[r][c])
                area.emplace_back(r, c);
        }
    }
    return size;
}

int Fillomino::getPartialSize(const std::vector<std::vector<Cell>>& b, const uint8_t i, const uint8_t j) const {
    std::vector<std::vector<bool>> used(rows, std::vector<bool>(cols, false));
    used[i][j] = true;
    int size = 1;
    getPartialSizeHelp(b, size, used, i, j);
    return size;
}

void Fillomino::getPartialSizeHelp(const std::vector<std::vector<Cell>>& b, int& size, std::vector<std::vector<bool>>& used,
                                   const uint8_t i, const uint8_t j) const {
    if (isInBounds(i-1, j) && !used[i-1][j] && b[i-1][j] == b[i][j]) { // ^
        size++;
        used[i-1][j] = true;
        getPartialSizeHelp(b, size, used, i-1, j);
    }
    if (isInBounds(i, j+1) && !used[i][j+1] && b[i][j+1] == b[i][j]) { // >
        size++;
        used[i][j+1] = true;
        getPartialSizeHelp(b, size, used, i, j+1);
    }
    if (isInBounds(i+1, j) && !used[i+1][j] && b[i+1][j] == b[i][j]) { // d
        size++;
        used[i+1][j] = true;
        getPartialSizeHelp(b, size, used, i+1, j);
    }
    if (isInBounds(i, j-1) && !used[i][j-1] && b[i][j-1] == b[i][j]) { // <
        size++;
        used[i][j-1] = true;
        getPartialSizeHelp(b, size, used, i, j-1);
    }
}

void Fillomino::getBoardFromFile(const std::string& file) {
    std::ifstream f(file);
    if (!f.is_open()) {
        std::cerr << "Failed to open the file\n";
    }

    f >> rows;
    f >> cols;
    board.resize(rows);

    int n;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            f >> n;
            if (n < 0 || n > 9) {
                validFromFile = false;
            }
            board[i].push_back(Cell(n));
        }
    }
    f.close();

    std::vector<std::vector<bool>> counted(rows, std::vector<bool>(cols, false));
    std::vector<std::pair<uint8_t, uint8_t>> area;

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (board[i][j].getNum() != 0) {
                area.clear();
                int size = getPartialSize(i, j, area);
                for (const auto& [r,c] : area)
                    board[r][c].setGroupSize(size);
            }
        }
    }
}

bool Fillomino::areSizesValid() const {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (board[i][j].getNum() != 0) {
                if (getPartialSize(board, i, j) > board[i][j].getNum())
                    return false;
            }
        }
    }
    return true;
}

bool Fillomino::isValid() const { 
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (board[i][j].getNum() == 0) {
                if (isCellBlocked(board, i, j))
                    return false;
                if (getPartialSize(board,i,j) == 2 && invalidEmptyCells(board,i,j)) 
                    return false;
            } else {
                if (isInBounds(i,j+1) && board[i][j] == board[i][j+1] && board[i][j].finished != board[i][j+1].finished)
                    return false;
                if (isInBounds(i+1,j) && board[i][j] == board[i+1][j] && board[i][j].finished != board[i+1][j].finished)
                    return false;
            }
        }
    }
    return true;
}

bool Fillomino::isValid(const std::vector<std::vector<Cell>>& b) const {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (b[i][j].getNum() == 0) {
                if (isCellBlocked(b, i, j))
                    return false;
                if (getPartialSize(b,i,j) == 2 && invalidEmptyCells(b,i,j)) 
                    return false;
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

bool Fillomino::processCode(const std::string& code, const uint8_t i_idx, const uint8_t j_idx, std::vector<std::vector<Cell>>& b, const uint8_t number) const {
    std::vector<std::vector<Cell>> boardCopy = board;

    boardCopy[i_idx][j_idx].finished = true;
    const uint8_t n = (number == 33) ? board[i_idx][j_idx].getNum() : number;
    boardCopy[i_idx][j_idx].setNum(n); 
    const size_t pos = code.find('x');
    std::string sub1 = code.substr(0, pos);
    std::string sub2 = code.substr(pos + 1);

    uint8_t i = i_idx;
    uint8_t j = j_idx;
    uint8_t new_i;
    uint8_t new_j;
    for (int idx = sub1.size() - 1; idx >= 0; --idx) {
        getNewCoords(new_i, new_j, i, j, sub1[idx]);
        if (!isInBounds(new_i, new_j) || (board[new_i][new_j].getNum() != n && board[new_i][new_j].getNum() != 0)
            || (board[new_i][new_j].finished && !(new_i == i_idx && new_j == j_idx)))
            return false;

        boardCopy[new_i][new_j].setNum(n);
        boardCopy[new_i][new_j].finished = true;

        i = new_i;
        j = new_j;
    }

    i = i_idx;
    j = j_idx;

    for (const auto& c : sub2) {
        getNewCoords(new_i, new_j, i, j, c);

        if (!isInBounds(new_i, new_j) || (board[new_i][new_j].getNum() != n && board[new_i][new_j].getNum() != 0)
            || (board[new_i][new_j].finished && !(new_i == i_idx && new_j == j_idx)))
            return false;

        boardCopy[new_i][new_j].setNum(n);
        boardCopy[new_i][new_j].finished = true;

        i = new_i;
        j = new_j;
    }

    if (getPartialSize(boardCopy, i_idx, j_idx) > n)
        return false;

    bool v = isValid(boardCopy);
    if (v) {
        b = boardCopy;
    }
    return v;
}

// returns true if cell is unfinished and all neighbours are finished (+min. one neighbour is 1)
bool Fillomino::isCellBlocked(const std::vector<std::vector<Cell>>& b, const uint8_t i, const uint8_t j) const {
    if ((isInBounds(i+1, j) && !b[i+1][j].finished) || (isInBounds(i-1, j) && !b[i-1][j].finished)
        || (isInBounds(i, j+1) && !b[i][j+1].finished) || (isInBounds(i, j-1) && !b[i][j-1].finished))
        return false;
    if (opt)
        return true;
    else if ((isInBounds(i+1, j) && b[i+1][j].getNum() == 1) || (isInBounds(i-1, j) && b[i-1][j].getNum() == 1)
              || (isInBounds(i, j+1) && b[i][j+1].getNum() == 1) || (isInBounds(i, j-1) && b[i][j-1].getNum() == 1))
        return true;
    return false;
}

bool Fillomino::invalidEmptyCells(const std::vector<std::vector<Cell>>& b, const uint8_t i, const uint8_t j) const {
    std::vector<std::pair<uint8_t, uint8_t>> nbhs;
    if (isInBounds(i+1,j) && b[i+1][j].getNum() == 0) {
        getAllNbhs(i,j,i+1,j,nbhs);
    } else if (isInBounds(i-1,j) && b[i-1][j].getNum() == 0) {
        getAllNbhs(i,j,i-1,j,nbhs);
    } else if (isInBounds(i,j-1) && b[i][j-1].getNum() == 0) {
        getAllNbhs(i,j,i,j-1,nbhs);
    } else {
        getAllNbhs(i,j,i,j+1,nbhs);
    }

    for (const auto& [k,l] : nbhs) {
        if (!b[k][l].finished)
            return false;
    }

    //if (opt) return true;

    // skoro wszyscy sasiedzi sa finished, pytanie czy mozna wstawic 2
    for (const auto& [k,l] : nbhs) {
        if (b[k][l].getNum() == 2) // nie mozna, wiec invalid
            return true;
    }

    return false;
}

void Fillomino::getSurroundingCells(const std::vector<std::pair<int, int>>& polyomino, std::set<std::pair<int, int>>& surr) const {
    const std::vector<std::pair<int, int>>& directions = {
        {-1, 0}, {1, 0}, {0, -1}, {0, 1}
    };

    for (const auto& cell : polyomino) {
        for (const auto& direction : directions) {
            int newRow = cell.first + direction.first;
            int newCol = cell.second + direction.second;

            if (isInBounds(newRow, newCol)) {
                std::pair<int, int> surroundingCell = {newRow, newCol};

                if (std::find(polyomino.begin(), polyomino.end(), surroundingCell) == polyomino.end()) {
                    surr.insert(surroundingCell);
                }
            }
        }
    }
}

bool Fillomino::isCellAClue(const uint8_t i, const uint8_t j) const {
    return board[i][j].getNum() != 0 && !board[i][j].finished;
}

void Fillomino::completeOneOption() {
    uint8_t n;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (board[i][j].getNum() == 0 && oneOption(n,i,j)) {
                board[i][j].setNum(n);
                std::vector<std::pair<uint8_t, uint8_t>> area;
                int size = getPartialSize(i, j, area);
                for (const auto& [r,c] : area)
                    board[r][c].setGroupSize(size);
            }
        }
    }
}

bool Fillomino::oneOption(uint8_t& n, const uint8_t i, const uint8_t j) const {
    std::set<uint8_t> nbh;
    bool onePresent = false;
    const std::vector<std::pair<int, int>>& directions = {
        {-1, 0}, {1, 0}, {0, -1}, {0, 1}
    };

    for (const auto& dir : directions) {
        if (isInBounds(i+dir.first, j+dir.second)) {
            if (board[i + dir.first][j + dir.second].getNum() == 0)
                return false;
            if (board[i + dir.first][j + dir.second].getNum() == 1)
                onePresent = true;
            else if (!board[i + dir.first][j + dir.second].finished)
                nbh.insert(board[i + dir.first][j + dir.second].getNum());
        }
    }

    if (onePresent && nbh.size() == 1) {
        const uint8_t num = *nbh.begin();
        n = num;
        return true;
    }
    return false;
}

void Fillomino::certainCells(const std::vector<std::vector<std::string>>& codes) {
    std::vector<std::vector<bool>> checked(rows, std::vector<bool>(cols, false));
    for (uint8_t i = 0; i < rows; i++)
        for (uint8_t j = 0; j < cols; j++)
            if (!checked[i][j] && isCellAClue(i,j)) {
                checked[i][j] = true;
                crossSection(checked,codes,i,j);
            }
}

void Fillomino::crossSection(std::vector<std::vector<bool>>& done, const std::vector<std::vector<std::string>>& codes, const uint8_t i, const uint8_t j) {
    std::vector<std::vector<uint8_t>> checked(rows, std::vector<uint8_t>(cols, 0));
    int cnt = 0;

    for (const auto& code : codes[board[i][j].getNum()]) {
        std::set<std::pair<uint8_t,uint8_t>> s;
        if (processCodeCrossSection(code, i, j, s)) {
            cnt++;
            for (const auto& p : s) {
                checked[p.first][p.second]++;
            }
        }
    }

    if (cnt == 0)
        return;

    for (int k = 0; k < rows; k++) {
        for (int l = 0; l < cols; l++) {
            if (checked[k][l] == cnt) {
                board[k][l] = board[i][j].getNum();
                done[k][l] = true;
            }
        }
    }

    std::vector<std::pair<uint8_t, uint8_t>> area;
    int size = getPartialSize(i, j, area);
    for (const auto& [r,c] : area) {
        board[r][c].setGroupSize(size);
        done[r][c] = true;
    }
}

bool Fillomino::processCodeCrossSection(const std::string& code, const uint8_t i_idx, const uint8_t j_idx, std::set<std::pair<uint8_t,uint8_t>>& s) const {
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

        if (!isInBounds(new_i, new_j) || (board[new_i][new_j].getNum() != n && board[new_i][new_j].getNum() != 0)
            || (board[new_i][new_j].finished && !(new_i == i_idx && new_j == j_idx)))
            return false;

        boardCopy[new_i][new_j].setNum(n);
        boardCopy[new_i][new_j].finished = true;
        s.insert({new_i, new_j});

        i = new_i;
        j = new_j;
    }

    i = i_idx;
    j = j_idx;

    for (const auto& c : sub2) {
        getNewCoords(new_i, new_j, i, j, c);

        if (!isInBounds(new_i, new_j) || (board[new_i][new_j].getNum() != n && board[new_i][new_j].getNum() != 0)
            || (board[new_i][new_j].finished && !(new_i == i_idx && new_j == j_idx)))
            return false;

        boardCopy[new_i][new_j].setNum(n);
        boardCopy[new_i][new_j].finished = true;
        s.insert({new_i, new_j});

        i = new_i;
        j = new_j;
    }

    if (getPartialSize(boardCopy, i_idx, j_idx) > n)
        return false;

    bool v = isValid(boardCopy);
    return v;
}

bool Fillomino::isSolved() const {
    for (uint8_t i = 0; i < rows; i++) {
        for (uint8_t j = 0; j < cols; j++)
            if (!board[i][j].finished)
                return false;
    }
    return true;
}

uint8_t Fillomino::getNum(const uint8_t i, const uint8_t j) const {
    return board[i][j].getNum();
}

// fills vector with all adjecent cells of (i1,j1), (i2,j2)
void Fillomino::getAllNbhs(const uint8_t i1, const uint8_t j1, const uint8_t i2, const uint8_t j2,
                           std::vector<std::pair<uint8_t, uint8_t>>& nbhs) const {
    if (isInBounds(i1+1, j1) && !(i1+1 == i2 && j1 == j2))
        nbhs.emplace_back(i1+1, j1);
    if (isInBounds(i1-1, j1) && !(i1-1 == i2 && j1 == j2))
        nbhs.emplace_back(i1-1, j1);
    if (isInBounds(i1, j1+1) && !(i1 == i2 && j1+1 == j2))
        nbhs.emplace_back(i1, j1+1);
    if (isInBounds(i1, j1-1) && !(i1 == i2 && j1-1 == j2))
        nbhs.emplace_back(i1, j1-1);

    if (isInBounds(i2+1, j2) && !(i1 == i2+1 && j1 == j2))
        nbhs.emplace_back(i2+1, j2);
    if (isInBounds(i2-1, j2) && !(i1 == i2-1 && j1 == j2))
        nbhs.emplace_back(i2-1, j2);
    if (isInBounds(i2, j2+1) && !(i1 == i2 && j1 == j2+1))
        nbhs.emplace_back(i2, j2+1);
    if (isInBounds(i2, j2-1) && !(i1 == i2 && j1 == j2-1))
        nbhs.emplace_back(i2, j2-1);
}

void Fillomino::print() const {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            std::cout << static_cast<int>(board[i][j].getNum()) << ' ';
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}
