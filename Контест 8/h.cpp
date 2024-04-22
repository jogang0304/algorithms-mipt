#include <algorithm>
#include <array>
#include <iostream>
#include <map>
#include <set>
#include <stack>
#include <vector>

const int cInf = 2'000'000'000;
const int cFieldSize = 3;

struct Field {
  std::array<std::array<int, cFieldSize>, cFieldSize> nums;
  int changes = cInf;
  std::pair<int, int> position_of_zero = {cInf, cInf};
  char last_move = ' ';

  Field() = default;

  Field(std::array<int, cFieldSize * cFieldSize> a) : changes(0) {
    for (int i = 0; i < cFieldSize; ++i) {
      for (int j = 0; j < cFieldSize; ++j) {
        nums[i][j] = a[i * cFieldSize + j];
        if (nums[i][j] == 0) {
          position_of_zero = {i, j};
        }
      }
    }
  }

  bool operator<(const Field& other) const {
    for (int i = 0; i < cFieldSize; ++i) {
      for (int j = 0; j < cFieldSize; ++j) {
        if (nums[i][j] != other.nums[i][j]) {
          return nums[i][j] < other.nums[i][j];
        }
      }
    }
    return false;
  }
};

const Field cFinalField({1, 2, 3, 4, 5, 6, 7, 8, 0});

std::vector<Field> GetNextFields(const Field& f) {
  std::vector<Field> next_fields(0);
  if (f.position_of_zero.first > 0) {
    Field next = f;
    std::swap(
        next.nums[f.position_of_zero.first][f.position_of_zero.second],
        next.nums[f.position_of_zero.first - 1][f.position_of_zero.second]);
    next.position_of_zero = {f.position_of_zero.first - 1,
                             f.position_of_zero.second};
    next.last_move = 'U';
    next.changes = f.changes + 1;
    next_fields.push_back(next);
  }
  if (f.position_of_zero.first < cFieldSize - 1) {
    Field next = f;
    std::swap(
        next.nums[f.position_of_zero.first][f.position_of_zero.second],
        next.nums[f.position_of_zero.first + 1][f.position_of_zero.second]);
    next.position_of_zero = {f.position_of_zero.first + 1,
                             f.position_of_zero.second};
    next.last_move = 'D';
    next.changes = f.changes + 1;
    next_fields.push_back(next);
  }
  if (f.position_of_zero.second > 0) {
    Field next = f;
    std::swap(
        next.nums[f.position_of_zero.first][f.position_of_zero.second],
        next.nums[f.position_of_zero.first][f.position_of_zero.second - 1]);
    next.position_of_zero = {f.position_of_zero.first,
                             f.position_of_zero.second - 1};
    next.last_move = 'L';
    next.changes = f.changes + 1;
    next_fields.push_back(next);
  }
  if (f.position_of_zero.second < cFieldSize - 1) {
    Field next = f;
    std::swap(
        next.nums[f.position_of_zero.first][f.position_of_zero.second],
        next.nums[f.position_of_zero.first][f.position_of_zero.second + 1]);
    next.position_of_zero = {f.position_of_zero.first,
                             f.position_of_zero.second + 1};
    next.last_move = 'R';
    next.changes = f.changes + 1;
    next_fields.push_back(next);
  }
  return next_fields;
}

int main() {
  std::array<int, cFieldSize * cFieldSize> a;
  for (int i = 0; i < cFieldSize * cFieldSize; ++i) {
    std::cin >> a[i];
  }
  Field f(a);
  std::set<Field> fields;
  fields.insert(f);

  auto cmp = [](const Field& left, const Field& right) {
    if (left.changes != right.changes) {
      return left.changes < right.changes;
    }
    return left < right;
  };

  std::set<Field, decltype(cmp)> queue(cmp);
  queue.insert(f);
  while (!queue.empty()) {
    Field cur = *queue.begin();
    queue.erase(queue.begin());
    auto found_cur = *fields.find(cur);
    for (auto& next : GetNextFields(cur)) {
      auto found_next = fields.find(next);
      if ((found_next == fields.end()) ||
          ((*found_next).changes > found_cur.changes + 1)) {
        queue.erase(next);
        fields.erase(*found_next);
        // next.changes = found_cur.changes + 1;
        fields.insert(next);
        queue.insert(next);
      }
    }
  }
  if (fields.find(cFinalField) == fields.end()) {
    std::cout << -1 << "\n";
  } else {
    auto found_final = fields.find(cFinalField);
    std::cout << (*found_final).changes << "\n";
    std::stack<char> moves;
    while ((*found_final).changes > 0) {
      moves.push((*found_final).last_move);
      Field next = *found_final;
      next.changes--;
      if (next.last_move == 'U') {
        std::swap(next.nums[next.position_of_zero.first]
                           [next.position_of_zero.second],
                  next.nums[next.position_of_zero.first + 1]
                           [next.position_of_zero.second]);
        next.position_of_zero = {next.position_of_zero.first + 1,
                                 next.position_of_zero.second};
      } else if (next.last_move == 'D') {
        std::swap(next.nums[next.position_of_zero.first]
                           [next.position_of_zero.second],
                  next.nums[next.position_of_zero.first - 1]
                           [next.position_of_zero.second]);
        next.position_of_zero = {next.position_of_zero.first - 1,
                                 next.position_of_zero.second};
      } else if (next.last_move == 'L') {
        std::swap(next.nums[next.position_of_zero.first]
                           [next.position_of_zero.second],
                  next.nums[next.position_of_zero.first]
                           [next.position_of_zero.second + 1]);
        next.position_of_zero = {next.position_of_zero.first,
                                 next.position_of_zero.second + 1};
      } else if (next.last_move == 'R') {
        std::swap(next.nums[next.position_of_zero.first]
                           [next.position_of_zero.second],
                  next.nums[next.position_of_zero.first]
                           [next.position_of_zero.second - 1]);
        next.position_of_zero = {next.position_of_zero.first,
                                 next.position_of_zero.second - 1};
      }
      found_final = fields.find(next);
    }
    while (!moves.empty()) {
      std::cout << moves.top();
      moves.pop();
    }
  }
}