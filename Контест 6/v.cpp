#include <algorithm>
#include <iostream>
#include <vector>

void CountForPrev(
    std::vector<std::vector<std::vector<std::vector<std::vector<bool>>>>>& dp,
    std::vector<std::vector<bool>>& prev, int x_coordinate, int y_coordinate,
    int length) {
  int board_height = static_cast<int>(dp[length][x_coordinate].size());
  for (auto& var : prev) {
    if (!var[x_coordinate * board_height + y_coordinate]) {
      dp[length][x_coordinate][y_coordinate].push_back(var);
      dp[length][x_coordinate][y_coordinate]
        [dp[length][x_coordinate][y_coordinate].size() - 1]
        [x_coordinate * board_height + y_coordinate] = true;
    }
  }
}

void CountDpCell(
    std::vector<std::vector<std::vector<std::vector<std::vector<bool>>>>>& dp,
    int x_coordinate, int y_coordinate, int length) {
  int board_width = static_cast<int>(dp[length].size());
  int board_height = static_cast<int>(dp[length][x_coordinate].size());
  if (x_coordinate > 0) {
    CountForPrev(dp, dp[length - 1][x_coordinate - 1][y_coordinate],
                 x_coordinate, y_coordinate, length);
  }
  if (y_coordinate > 0) {
    CountForPrev(dp, dp[length - 1][x_coordinate][y_coordinate - 1],
                 x_coordinate, y_coordinate, length);
  }
  if (x_coordinate < board_width - 1) {
    CountForPrev(dp, dp[length - 1][x_coordinate + 1][y_coordinate],
                 x_coordinate, y_coordinate, length);
  }
  if (y_coordinate < board_height - 1) {
    CountForPrev(dp, dp[length - 1][x_coordinate][y_coordinate + 1],
                 x_coordinate, y_coordinate, length);
  }
}

int main() {
  int board_width;
  int board_height;
  std::cin >> board_width >> board_height;
  std::vector<std::vector<std::vector<std::vector<std::vector<bool>>>>> dp(
      board_width * board_height,
      std::vector<std::vector<std::vector<std::vector<bool>>>>(
          board_width,
          std::vector<std::vector<std::vector<bool>>>(
              board_height,
              std::vector<std::vector<bool>>(
                  0, std::vector<bool>(board_width * board_height, false)))));
  // [path length][coordinate x][coordinate y][vector of masks]
  // mask: [x][y] -> x*m + y, 0 <= x < n, 0 <= y < m
  std::vector<bool> default_mask(board_width * board_height, false);
  default_mask[0] = true;
  dp[0][0][0] = {default_mask};
  for (int length = 1; length < board_width * board_height; ++length) {
    for (int x_coordinate = 0; x_coordinate < board_width; ++x_coordinate) {
      for (int y_coordinate = (x_coordinate + length) % 2;
           y_coordinate < board_height; y_coordinate += 2) {
        CountDpCell(dp, x_coordinate, y_coordinate, length);
      }
    }
  }
  std::cout << (dp[board_width * board_height - 1][0][1].size() +
                dp[board_width * board_height - 1][1][0].size()) /
                   2;
}