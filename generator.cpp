#include "generator.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>
#include <utility>
#include <cstdint>

#include "utils.h"

void printAdj(const std::vector<std::vector<std::pair<uint8_t, char>>>& list) {
    for (size_t i=0; i < list.size(); i++) {
        if (list[i].size() > 0) std::cout << i << ':';
        for (const auto& [n,dir] : list[i])
            std::cout << static_cast<int>(n) << '(' << dir << "), ";
        if (list[i].size() > 0) std::cout << "\n";
    }
}

void Generator::generate() {
    std::vector<std::vector<int8_t>> board(N, std::vector<int8_t>(cols, -1));
    std::vector<std::pair<uint8_t, uint8_t>> border_cells;
    std::vector<std::vector<std::pair<uint8_t, char>>> adj_list;
    std::vector<bool> used(N*cols, false);

    uint8_t counter = 0;
    uint8_t max_used = 0;
    board[N-1][N-2] = counter++;  // 0
    used[0] = true;

    board[N-1][N-1] = counter++;  // 1
    board[N-2][N-2] = counter++;  // 2

    used[1] = true;
    border_cells.emplace_back(N-2, N-2);
    nextStep(board, border_cells, adj_list, used, N-1, N-1, counter, max_used);
    depth--;

    used[2] = true;
    used[1] = false;
    border_cells[0].first = N-1;
    border_cells[0].second = N-1;
    nextStep(board, border_cells, adj_list, used, N-2, N-2, counter, max_used);
    depth--;
}

void Generator::nextStep(const std::vector<std::vector<int8_t>>& old_board,
                         const std::vector<std::pair<uint8_t, uint8_t>>& available_cells,
                         const std::vector<std::vector<std::pair<uint8_t, char>>>& old_adj_list,
                         const std::vector<bool>& old_used, const uint8_t added_i, const uint8_t added_j,
                         const uint8_t old_counter, const uint8_t old_max_used) {
    std::vector<std::vector<int8_t>> board(old_board);
    std::vector<std::pair<uint8_t, uint8_t>> border_cells(available_cells);
    std::vector<std::vector<std::pair<uint8_t, char>>> adj_list(old_adj_list);
    std::vector<bool> used(old_used);
    uint8_t counter = old_counter;
    uint8_t max_used = old_max_used;
    depth++;

    chooseCell(adj_list, used, added_i, added_j, board);
    max_used = board[added_i][added_j];
    used[board[added_i][added_j]] = true;
    addNewBorderCells(board, border_cells, added_i, added_j, counter);
    updateAvailableBorderCells(border_cells, board, max_used);

    std::cout << "------------------------------\ndepth=" << static_cast<int>(depth) << std::endl;
    for (uint8_t i=0; i<N; i++) {
        for (uint8_t j=0; j<cols; j++)
            std::cout << std::setw(2) << static_cast<int>(board[i][j]) << ' ';
        std::cout << '\n';
    }
    std::cout << "used: ";
    for (size_t i = 0; i < used.size(); i++)
        if (used[i]) std::cout << i << ' ';
    std::cout << '\n';
    printAdj(adj_list);
    std::string code = "";
    generatePolyominoCodes(code, adj_list);

    if (depth < N) {  // <= ?
        for (const auto& [i, j] : border_cells) {
            nextStep(board, border_cells, adj_list, used, i, j, counter, max_used);
            depth--;
        }
    }
}

// kiedy robic update max_used??
// board[i][j] - new cell
void Generator::addNewBorderCells(std::vector<std::vector<int8_t>>& board,
                                  std::vector<std::pair<uint8_t, uint8_t>>& border_cells,
                                  const uint8_t i, const uint8_t j, uint8_t& counter) const {
    if (isInBounds(i+1,j) && board[i+1][j] == -1) {
        board[i+1][j] = counter++;
        border_cells.emplace_back(i+1,j);
    } // check
    if (isInBounds(i,j-1) && board[i][j-1] == -1) {
        board[i][j-1] = counter++;
        border_cells.emplace_back(i,j-1);
    }
    if (isInBounds(i,j+1) && board[i][j+1] == -1) {
        board[i][j+1] = counter++;
        border_cells.emplace_back(i,j+1);
    }
    if (isInBounds(i-1,j) && board[i-1][j] == -1) {
        board[i-1][j] = counter++;
        border_cells.emplace_back(i-1,j);
    }
}

void Generator::updateAvailableBorderCells(std::vector<std::pair<uint8_t, uint8_t>>& cells, 
                                           const std::vector<std::vector<int8_t>>& board, const uint8_t max_used) const {
    cells.erase(std::remove_if(cells.begin(), cells.end(), [&](const std::pair<int, int>& p) {
        return board[p.first][p.second] <= max_used;
    }), cells.end());
}

void Generator::chooseCell(std::vector<std::vector<std::pair<uint8_t, char>>>& adj_list, const std::vector<bool>& used,
                           const uint8_t i, const uint8_t j, const std::vector<std::vector<int8_t>>& board) {
    // find used nbh and add to the list
    if (isInBounds(i,j-1) && board[i][j-1] != -1 && used[board[i][j-1]]) {  // ->
        if (static_cast<size_t>(board[i][j-1]) >= adj_list.size())
            resizeList(adj_list, board[i][j-1]);
        adj_list[board[i][j-1]].emplace_back(board[i][j], '0');
    } else if (isInBounds(i,j+1) && board[i][j+1] != -1 && used[board[i][j+1]]) {  // <-
        if (static_cast<size_t>(board[i][j+1]) >= adj_list.size())
            resizeList(adj_list, board[i][j+1]);
        adj_list[board[i][j+1]].emplace_back(board[i][j], '2');
    } else if (isInBounds(i+1,j) && board[i+1][j] != -1 && used[board[i+1][j]]) {  // ^
        if (static_cast<size_t>(board[i+1][j]) >= adj_list.size())
            resizeList(adj_list, board[i+1][j]);
        adj_list[board[i+1][j]].emplace_back(board[i][j], '1');
    } else { // down
        if (static_cast<size_t>(board[i-1][j]) >= adj_list.size())
            resizeList(adj_list, board[i-1][j]);
        adj_list[board[i-1][j]].emplace_back(board[i][j], '3');
    }
}

void Generator::dfs(std::string& polyomino, std::vector<bool>& used, size_t& visited,
                    const std::vector<std::vector<std::pair<uint8_t, char>>>& adj_list, const uint8_t curr) const {
    /*
    std::cout << "polyomino: " << &polyomino << '\n';
    std::cout << "used: " << &used << '\n';
    std::cout << "visited: " << &visited << '\n';
    std::cout << "adj_list: " << &adj_list << '\n';
    std::cout << "depth: " << &depth << '\n';
    std::cout << "this: " << this << '\n';
    std::cout << "curr: " << static_cast<int>(curr) << ", adj_list.size(): " << adj_list.size() << '\n';
    */
    if (curr >= adj_list.size()) return;

    for (const auto& [n,dir] : adj_list[curr]) {
        if (!used[n] && visited < depth - 1) {  // ? is the 1. necessary
            polyomino += dir;
            used[n] = true;
            visited++;
            dfs(polyomino, used, visited, adj_list, n);
            if (visited < depth - 1) {
                polyomino += getOppositeDir(dir);
            }
        }
    }
}

void Generator::generatePolyominoCodes(std::string& polyomino, const std::vector<std::vector<std::pair<uint8_t, char>>>& adj_list) {
    size_t visited = 0;
    std::vector<bool> used(adj_list.size(), false);
    used[0] = true;
    polyomino = "x";
    dfs(polyomino, used, visited, adj_list, 0);
    generateAllOptions(polyomino);
}

void Generator::generateAllOptions(const std::string& code) {
    std::vector<std::string> polyominoes;
    polyominoes.push_back(code);
    std::string s1 = "";
    std::string s2 = code.substr(1);

    while (s2 != "") {
        s1 += getOppositeDir(s2[0]);
        s2 = s2.substr(1);
        if (s1 == s2.substr(0, s1.length())) {
            s1 = "";
            polyominoes.clear();
        }
        polyominoes.push_back(s1 + "x" + s2);
    }

    for (const auto& p : polyominoes) {
        std::cout << p << '\n';
        file << static_cast<int>(depth) << ' ' << p << '\n';
    }
}

bool Generator::isInBounds(const int8_t i, const int8_t j) const {
    return i >= 0 && i < N && j >= 0 && j < cols;
}
