#ifndef POLYOMINOES_CELL_H
#define POLYOMINOES_CELL_H

#include <cstdint>

class Cell {
    uint8_t num = 0;
    uint8_t group_size = 0;
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

    void setGroupSize(const uint8_t size) { // pomysl co z 0
        group_size = size;
        if (group_size == num)
            finished = true;
    }

    uint8_t getGroupSize() const {
        return group_size;
    }

    void updateGroupSize(const uint8_t delta) {
        group_size += delta;
        if (group_size == num)
            finished = true;
    }
};

/*
to raczej odpada
struct Compare {
    bool operator()(const Cell& c1, const Cell& c2) {
        return c1.value < c2.value;
    } // zmiana na std::pair<> ? bez cell -> move to fillomino 
};

std::priority_queue<A, std::vector<A>, Compare> pq; -> std::pair, std::vector<std::pair<uint8_t, uint8_t>>
*/

#endif  // POLYOMINOES_CELL_H