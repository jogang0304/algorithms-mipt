#include <algorithm>
#include <iostream>
#include <vector>

struct Cell {
  int answer = 1;
};

struct AnswerCell {
  int value = 1;
  int index = 0;
  bool is_bigger = true;
};

std::vector<int> GetAnswer(const std::vector<Cell>& dp_last_is_bigger,
                           const std::vector<Cell>& dp_last_is_smaller,
                           const std::vector<int>& regions, AnswerCell data) {
  std::vector<int> answer(0);
  int prev_index = data.index - 1;
  for (; prev_index >= 0; --prev_index) {
    if (data.is_bigger) {
      if (regions[prev_index] < regions[data.index] &&
          dp_last_is_smaller[prev_index].answer == data.value - 1) {
        break;
      }
    } else {
      if (regions[prev_index] > regions[data.index] &&
          dp_last_is_bigger[prev_index].answer == data.value - 1) {
        break;
      }
    }
  }
  if (prev_index >= 0) {
    answer = GetAnswer(dp_last_is_bigger, dp_last_is_smaller, regions,
                       {data.value - 1, prev_index, !data.is_bigger});
  }
  answer.push_back(data.index);
  return answer;
}

int main() {
  int amount_of_regions;
  std::cin >> amount_of_regions;
  std::vector<int> regions(amount_of_regions);
  for (int i = 0; i < amount_of_regions; ++i) {
    std::cin >> regions[i];
  }
  std::vector<Cell> dp_last_is_bigger(amount_of_regions);
  std::vector<Cell> dp_last_is_smaller(amount_of_regions);
  int best_answer = 1;
  int best_answer_index = 0;
  bool best_answer_is_bigger = false;
  for (int i = 1; i < amount_of_regions; ++i) {
    for (int j = 0; j < i; ++j) {
      if (regions[i] > regions[j]) {
        if (dp_last_is_smaller[j].answer + 1 > dp_last_is_bigger[i].answer) {
          dp_last_is_bigger[i].answer = dp_last_is_smaller[j].answer + 1;
        }
      } else if (regions[i] < regions[j]) {
        if (dp_last_is_bigger[j].answer + 1 > dp_last_is_smaller[i].answer) {
          dp_last_is_smaller[i].answer = dp_last_is_bigger[j].answer + 1;
        }
      }
      if (dp_last_is_bigger[i].answer > best_answer) {
        best_answer = dp_last_is_bigger[i].answer;
        best_answer_index = i;
        best_answer_is_bigger = true;
      }
      if (dp_last_is_smaller[i].answer > best_answer) {
        best_answer = dp_last_is_smaller[i].answer;
        best_answer_index = i;
        best_answer_is_bigger = false;
      }
    }
  }
  std::cout << best_answer << "\n";
  auto answer_list =
      GetAnswer(dp_last_is_bigger, dp_last_is_smaller, regions,
                {best_answer, best_answer_index, best_answer_is_bigger});
  for (auto element : answer_list) {
    std::cout << regions[element] << " ";
  }
}