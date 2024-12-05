#ifndef POLYOMINOES_CELL_H
#define POLYOMINOES_CELL_H

#include <cstdint>

class Cell {
    uint8_t num = 0;
    int group_size = 0;
public:
    bool finished = false;

    Cell(const uint8_t n) : num(n) {}

    bool operator==(const Cell& other) const {
        return this->num == other.num;
    }

    void setNum(const uint8_t n) {
        num = n;
    }

    uint8_t getNum() const {
        return num;
    }

    void setGroupSize(const int size) {
        group_size = size;
        if (group_size == num && num != 0)
            finished = true;
    }

    int getGroupSize() const {
        return group_size;
    }
};

#endif  // POLYOMINOES_CELL_H