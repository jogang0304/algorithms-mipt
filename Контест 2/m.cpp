#include <algorithm>
#include <iostream>
#include <vector>

const int64_t kInf = 2'000'000'000;

void CountSums(std::vector<int64_t>& weights, std::vector<int64_t>& sums,
               int64_t points_for_second) {
  sums[0] = 0;
  for (int64_t i = 0; i < points_for_second; ++i) {
    sums[0] += weights[i];
  }
  for (size_t i = 1; i < sums.size() - points_for_second; ++i) {
    sums[i] = sums[i - 1] + weights[i + points_for_second - 1] - weights[i - 1];
  }
}

class MaxTree {
 private:
  std::vector<int64_t> tree_;
  int64_t array_size_;

 public:
  MaxTree(std::vector<int64_t>& array);
  void Update(int64_t pos, int64_t value, int64_t tree_index = 0,
              int64_t node_left = 0, int64_t node_right = -1);
  int64_t GetMax(int64_t left, int64_t right, int64_t tree_index = 0,
                 int64_t node_left = 0, int64_t node_right = -1);
};

MaxTree::MaxTree(std::vector<int64_t>& array)
    : tree_(array.size() * 4), array_size_(array.size()) {
  for (int64_t i = 0; i < static_cast<int64_t>(array.size()); ++i) {
    Update(i, array[i]);
  }
}

void MaxTree::Update(int64_t pos, int64_t value, int64_t tree_index,
                     int64_t node_left, int64_t node_right) {
  if (node_right == -1) {
    node_right = array_size_;
  }
  if (node_left >= node_right) {
    return;
  }
  if (node_left + 1 == node_right) {
    tree_[tree_index] = value;
    return;
  }
  int64_t node_middle = (node_left + node_right) / 2;
  if (pos < node_middle) {
    Update(pos, value, tree_index * 2 + 1, node_left, node_middle);
  } else {
    Update(pos, value, tree_index * 2 + 2, node_middle, node_right);
  }
  tree_[tree_index] =
      std::max(tree_[tree_index * 2 + 1], tree_[tree_index * 2 + 2]);
}

int64_t MaxTree::GetMax(int64_t left, int64_t right, int64_t tree_index,
                        int64_t node_left, int64_t node_right) {
  if (node_right == -1) {
    node_right = array_size_;
  }
  if (left == node_left && right == node_right) {
    return tree_[tree_index];
  }
  int64_t node_middle = (node_left + node_right) / 2;
  int64_t ans = -kInf;
  if (left < node_middle) {
    ans = std::max(ans, GetMax(left, std::min(node_middle, right),
                               tree_index * 2 + 1, node_left, node_middle));
  }
  if (right > node_middle) {
    ans = std::max(ans, GetMax(std::max(node_middle, left), right,
                               tree_index * 2 + 2, node_middle, node_right));
  }
  return ans;
}

// class MaxSparseTable {
//  private:
//   std::vector<std::vector<int64_t>> subarrays_;
//   int64_t array_size_;
//  public:
//   MaxSparseTable(std::vector<int64_t>& array): subarrays_(0),
//   array_size_(array.size()) {

//   }
// };

int64_t GetSecondResultByFirstIndex(int64_t first_answer_index,
                                    std::vector<int64_t>& sums,
                                    int64_t amount_of_points,
                                    int64_t points_for_second, MaxTree& maxs) {
  int64_t second_answer = maxs.GetMax(
      first_answer_index + 1,
      std::min(first_answer_index + amount_of_points - points_for_second + 1,
               static_cast<int64_t>(sums.size())));
  if (first_answer_index + amount_of_points + 1 <
      static_cast<int64_t>(sums.size())) {
    second_answer = std::max(
        second_answer,
        maxs.GetMax(first_answer_index + amount_of_points + 1, sums.size()));
  }
  if (first_answer_index >= points_for_second) {
    second_answer =
        std::max(second_answer,
                 maxs.GetMax(0, first_answer_index - points_for_second + 1));
  }
  return second_answer;
}

int main() {
  std::cin.tie(0);
  std::cout.tie(0);
  std::ios_base::sync_with_stdio(false);

  int64_t amount_of_points;
  std::cin >> amount_of_points;
  std::vector<int64_t> weights_cycle(amount_of_points * 2);
  for (int64_t i = 0; i < amount_of_points; ++i) {
    std::cin >> weights_cycle[i];
  }
  for (int64_t i = amount_of_points; i < amount_of_points * 2; ++i) {
    weights_cycle[i] = weights_cycle[i - amount_of_points];
  }
  int64_t double_sum = 0;
  for (auto element : weights_cycle) {
    double_sum += element;
  }
  int64_t points_for_second = amount_of_points / 2;

  // сумма на отрезке [i:i + points_for_second)
  std::vector<int64_t> sums(amount_of_points * 2 - points_for_second);
  CountSums(weights_cycle, sums, points_for_second);

  MaxTree maxs(sums);

  // какой счёт будет у первого игрока, если первым ходом он возьмёт вершину i
  std::vector<int64_t> first_result(amount_of_points);
  for (int64_t i = 0; i < amount_of_points; ++i) {
    int64_t second_result = GetSecondResultByFirstIndex(
        i, sums, amount_of_points, points_for_second, maxs);
    first_result[i] = (double_sum / 2) - second_result;
  }

  int64_t first_answer = 0;
  int64_t first_answer_index = 0;
  for (int64_t i = 0; i < amount_of_points; ++i) {
    if (first_result[i] > first_answer) {
      first_answer = first_result[i];
      first_answer_index = i;
    }
  }
  int64_t second_answer = GetSecondResultByFirstIndex(
      first_answer_index, sums, amount_of_points, points_for_second, maxs);
  std::cout << first_answer << " " << second_answer;
}