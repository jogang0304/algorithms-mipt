#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <iostream>
#include <string>
#include <vector>

const int kMaskBits = 50;

int64_t cur_clique = 0;
std::vector<int64_t> robot_friends;

bool PassesToClique(int robot) {
  return (robot_friends[robot] & cur_clique) == cur_clique;
}

int64_t GetAnswer() {
  int last_robot = 0;
  for (int i = 0; i < kMaskBits; ++i) {
    if (((cur_clique >> i) & 1) != 0) {
      last_robot = i;
    }
  }
  if ((robot_friends[last_robot] & ~((1 << last_robot) - 1)) ==
      (cur_clique & ~((1 << last_robot) - 1))) {
    return 0;
  }
  int64_t answer = 0;
  for (int16_t i = static_cast<int16_t>(static_cast<int>(last_robot) + 1);
       i < static_cast<int16_t>(robot_friends.size()); ++i) {
    if (((robot_friends[last_robot] >> i & 1) != 0) && PassesToClique(i)) {
      cur_clique |= 1 << i;
      answer += 1 + GetAnswer();
      cur_clique &= ~(1 << i);
    }
  }
  return answer;
}

int main() {
  int amount_of_robots_raw = 0;
  std::cin >> amount_of_robots_raw;
  int16_t amount_of_robots = static_cast<int16_t>(amount_of_robots_raw);
  robot_friends.resize(amount_of_robots, 0);
  for (int16_t i = 0; i < amount_of_robots; ++i) {
    std::string friends;
    std::cin >> friends;
    for (int16_t j = 0; j < amount_of_robots; ++j) {
      if (friends[j] == '1') {
        robot_friends[i] |= 1 << j;
      }
    }
  }
  int64_t answer = 1 + amount_of_robots;
  for (int16_t i = 0; i < amount_of_robots; ++i) {
    cur_clique = 1 << i;
    answer += GetAnswer();
  }
  std::cout << answer;
}