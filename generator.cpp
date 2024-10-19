#include "generator.h"

#include <algorithm>
#include <vector>
#include <utility>
#include <cstdint>

void resizeList(std::vector<std::vector<std::pair<uint8_t, char>>>& adj_list, uint8_t idx) {
    while (idx >= adj_list.size()) {
        adj_list.push_back(std::vector<std::pair<uint8_t, char>>());
    }
}

void Generator::generate() {
    std::vector<std::vector<int8_t>> board(N, std::vector<int8_t>(cols, -1));
    std::vector<std::pair<uint8_t, uint8_t>> border_cells;
    std::vector<std::vector<std::pair<uint8_t, char>>> adj_list; // jak z rozmiarem?
    std::vector<bool> used(N*cols, false);

    uint8_t counter = 0;
    uint8_t max_used = 0;
    board[N-1][N-2] = counter++;  // 0
    used[0] = true;

    board[N-1][N-1] = counter++;  // 1
    board[N-2][N-2] = counter++;  // 2

    used[1] = true;
    nextStep(board, border_cells, adj_list, used, N-1, N-1, counter, max_used);
    depth--;

    used[2] = true;
    used[1] = false;
    nextStep(board, border_cells, adj_list, used, N-2, N-2, counter, max_used);
    depth--;
}

void Generator::nextStep(const std::vector<std::vector<int8_t>>& old_board,
                         const std::vector<std::pair<uint8_t, uint8_t>>& available_cells,
                         const std::vector<std::vector<std::pair<uint8_t, char>>>& old_adj_list,
                         const std::vector<bool>& old_used,
                         const uint8_t added_i, const uint8_t added_j, const uint8_t old_counter, const uint8_t old_max_used) {
    std::vector<std::vector<int8_t>> board(old_board);
    std::vector<std::pair<uint8_t, uint8_t>> border_cells(available_cells);
    std::vector<std::vector<std::pair<uint8_t, char>>> adj_list(old_adj_list);
    std::vector<bool> used(old_used);
    uint8_t counter = old_counter;
    uint8_t max_used = old_max_used;
    depth++;
    std::cout << "------------------------------\ndepth=" << static_cast<int>(depth) << ", cnt=" << static_cast<int>(counter) << std::endl;

    //cos nie dziala - powtorki
    chooseCell(adj_list, used, added_i, added_j, board);
    max_used = board[added_i][added_j];
    used[board[added_i][added_j]] = true;
    addNewBorderCells(board, border_cells, added_i, added_j, counter);
    updateAvailableBorderCells(border_cells, board, max_used);

    for (uint8_t i=0; i<N; i++) {
        for (uint8_t j=0; j<cols; j++) 
            std::cout << static_cast<int>(board[i][j]) << ' ';
        std::cout << std::endl;
    }

    std::cout << "used: ";
    for (size_t i = 0; i < used.size(); i++)
        if (used[i]) std::cout << i << ' ';
    std::cout << "\nmax used=" << static_cast<int>(max_used) << std::endl;

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
                                  const uint8_t i, const uint8_t j, uint8_t& counter) {
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
                                           const std::vector<std::vector<int8_t>>& board, const uint8_t max_used) {
    cells.erase(std::remove_if(cells.begin(), cells.end(), [&](const std::pair<int, int>& p) {
        return board[p.first][p.second] < max_used;
    }), cells.end());
}

void Generator::chooseCell(std::vector<std::vector<std::pair<uint8_t, char>>>& adj_list, const std::vector<bool>& used,
                           const uint8_t i, const uint8_t j, const std::vector<std::vector<int8_t>>& board) {
    // find used nbh and add to the list
    std::cout << "chooseCell | i=" << static_cast<int>(i) << " j=" << static_cast<int>(j) << std::endl;
    
    if (isInBounds(i,j-1) && board[i][j-1] != -1 && used[board[i][j-1]]) {  // ->
        if (board[i][j-1] >= adj_list.size())
            resizeList(adj_list, board[i][j-1]);
        adj_list[board[i][j-1]].emplace_back(board[i][j], '0');
    } else if (isInBounds(i,j+1) && board[i][j+1] != -1 && used[board[i][j+1]]) {  // <-
        if (board[i][j+1] >= adj_list.size())
            resizeList(adj_list, board[i][j+1]);
        adj_list[board[i][j+1]].emplace_back(board[i][j], '2');
    } else {  // ^
        if (board[i+1][j] >= adj_list.size())
            resizeList(adj_list, board[i+1][j]);
        adj_list[board[i+1][j]].emplace_back(board[i][j], '1');
    }
}

bool Generator::isInBounds(const uint8_t i, const uint8_t j) const {
    return i >= 0 && i < N && j >= 0 && j < cols;
}
