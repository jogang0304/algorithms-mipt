#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <iostream>
#include <string>
#include <vector>

const int64_t kMaskBits = 50;

int64_t cur_clique = 0;
std::vector<int64_t> robot_friends;
std::vector<int64_t> max_cliques(0);

int64_t Power(int64_t base, int64_t exponent) {
  if (exponent < 0) {
    return 0;
  }
  if (exponent == 0) {
    return 1;
  }
  if (exponent % 2 == 1) {
    return Power(base, exponent - 1) * base;
  }
  auto res = Power(base, exponent / 2);
  return res * res;
}

bool PassesToClique(int64_t robot) {
  return (robot_friends[robot] & cur_clique) == cur_clique;
}

bool IsInSomeClique(int64_t robot) {
  int64_t new_clique = cur_clique | 1 << robot;
  for (size_t i = 0; i < max_cliques.size(); ++i) {
    if ((max_cliques[i] & new_clique) == new_clique) {
      return true;
    }
  }
  return false;
}

bool IsInCurClique(int64_t robot) { return ((cur_clique >> robot) & 1) == 1; }

int64_t CountOnes(int64_t mask) {
  int64_t answer = 0;
  for (int i = 0; i < kMaskBits; ++i) {
    if (((mask >> i) & 1) == 1) {
      ++answer;
    }
  }
  return answer;
}

bool IsIn(int number, int64_t array) { return ((array >> number) & 1) == 1; }

bool PathExists(int first, int second) {
  return ((robot_friends[first] >> second) & 1) == 1;
}

int LastOnePosition(int64_t mask) {
  int answer = -1;
  for (int i = 0; i < kMaskBits; ++i) {
    if (((mask >> i) & 1) == 1) {
      answer = i;
    }
  }
  return answer;
}

void Dfs(const std::vector<int>& nodes, int start, std::vector<int>& colors) {
  for (int i = 0; i < static_cast<int>(nodes.size()); ++i) {
    if (PathExists(start, i) && colors[i] == -1) {
      colors[i] = colors[start];
      Dfs(nodes, i, colors);
    }
  }
}

std::vector<std::vector<int>> FindComponents(const std::vector<int>& nodes) {
  std::vector<int> colors(nodes.size(), -1);
  int color = 0;
  for (int i = 0; i < static_cast<int>(nodes.size()); ++i) {
    if (colors[i] == -1) {
      colors[i] = ++color;
      Dfs(nodes, i, colors);
    }
  }
  std::vector<std::vector<int>> answer(color);
  for (int i = 0; i < static_cast<int>(nodes.size()); ++i) {
    answer[colors[i] - 1].push_back(i);
  }
  return answer;
}

std::vector<int64_t> CliquesOnMask(int64_t mask) {
  std::vector<int64_t> answer(0);
  std::vector<int> nodes(0);
  for (int i = 0; i < kMaskBits; ++i) {
    if (((mask >> i) & 1) == 1) {
      nodes.push_back(i);
    }
  }
  auto res = FindComponents(nodes);
  for (auto comp : res) {
    int64_t cl = 0;
    for (int i = 0; i < static_cast<int>(comp.size()); ++i) {
      cl |= (1 << comp[i]);
    }
    answer.push_back(cl);
  }
  return answer;
}

int64_t Interceptions() {
  int64_t answer = 0;
  int64_t super_mask = 0;
  for (size_t i = 0; i < max_cliques.size(); ++i) {
    super_mask |= max_cliques[i];
  }
  auto components = CliquesOnMask(super_mask);
  for (auto comp : components) {
    int64_t same_part = comp & cur_clique;
    answer += Power(2, CountOnes(same_part)) - 1;
  }
  return answer;
}

int64_t GetAnswer(int64_t last_robot) {
  int64_t answer = 0;
  for (int64_t i = 0; i < static_cast<int64_t>(robot_friends.size()); ++i) {
    if (((robot_friends[last_robot] >> i & 1) != 0) && !IsInCurClique(i) &&
        PassesToClique(i) && !IsInSomeClique(i)) {
      cur_clique |= 1 << i;
      answer += GetAnswer(i);
      cur_clique &= ~(1 << i);
    }
  }
  if (answer == 0 && !IsInSomeClique(last_robot)) {
    auto interceptions = Interceptions();
    max_cliques.push_back(cur_clique);
    int cur_clique_size = 0;
    for (int i = 0; i < kMaskBits; ++i) {
      if (((cur_clique >> i) & 1) == 1) {
        ++cur_clique_size;
      }
    }
    return Power(2, cur_clique_size) - 1 - interceptions;
  }
  return answer;
}

int main() {
  int amount_of_robots = 0;
  std::cin >> amount_of_robots;
  robot_friends.resize(amount_of_robots, 0);
  for (int64_t i = 0; i < amount_of_robots; ++i) {
    std::string friends;
    std::cin >> friends;
    for (int64_t j = 0; j < amount_of_robots; ++j) {
      if (friends[j] == '1') {
        robot_friends[i] |= 1 << j;
      }
    }
  }
  int64_t answer = 1;
  for (int64_t i = 0; i < amount_of_robots; ++i) {
    cur_clique = 1 << i;
    answer += GetAnswer(i);
  }
  std::cout << answer;
}