#ifndef POLYOMINOES_UTILS_H
#define POLYOMINOES_UTILS_H

#include <vector>
#include <utility>
#include <cstdint>

void resizeList(std::vector<std::vector<std::pair<uint8_t, char>>>& list, const uint8_t idx);
void getNewCoords(uint8_t& new_i, uint8_t& new_j, const uint8_t i, const uint8_t j, const char dir);
char getOppositeDir(const char dir);

#endif  // POLYOMINOES_UTILS_H
