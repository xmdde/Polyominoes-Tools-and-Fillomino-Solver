#ifndef POLYOMINOES_UTILS_H
#define POLYOMINOES_UTILS_H

#include <vector>
#include <utility>
#include <cstdint>

void resizeList(std::vector<std::vector<std::pair<uint8_t, char>>>& list, const uint8_t idx) {
    while (idx >= list.size()) {
        list.push_back(std::vector<std::pair<uint8_t, char>>());
    }
}

void getNewCoords(uint8_t& new_i, uint8_t& new_j, const uint8_t i, const uint8_t j, const char dir) {
    switch (dir) {
        case '0':
            new_i = i;
            new_j = j+1;
            break;
        case '1':
            new_i = i-1;
            new_j = j;
            break;
        case '2':
            new_i = i;
            new_j = j-1;
            break;
        case '3':
            new_i = i+1;
            new_j = j;
            break;
    }
    
}

char getOppositeDir(const char dir) {
    if (dir == '0')
        return '2';
    else if (dir == '1')
        return '3';
    else if (dir == '2')
        return '0';
    else return '1';
}

#endif  // POLYOMINOES_UTILS_H
