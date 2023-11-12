#include <iostream>
#include <vector>

bool Equal(std::pair<int, int> left, std::pair<int, int> right) {
  return left.first == right.first;
}

bool SortCondition(std::pair<int, int> left, std::pair<int, int> right) {
  return left.first <= right.first;
}

std::vector<std::pair<int, int>> Merge(std::vector<std::pair<int, int>>& values,
                                       std::vector<int>& answer, size_t left,
                                       size_t right) {
  size_t medium = (left + right) / 2;
  size_t left_part_index = left;
  size_t right_part_index = medium;
  std::vector<std::pair<int, int>> sorted_array(0);
  while (left_part_index < medium || right_part_index < right) {
    if (left_part_index < medium && right_part_index < right) {
      if (SortCondition(values[left_part_index], values[right_part_index])) {
        answer[values[left_part_index].second] += (right_part_index - medium);
        sorted_array.push_back(values[left_part_index++]);
      } else {
        sorted_array.push_back(values[right_part_index++]);
      }
    } else if (left_part_index < medium) {
      answer[values[left_part_index].second] += (right_part_index - medium);
      sorted_array.push_back(values[left_part_index++]);
    } else {
      sorted_array.push_back(values[right_part_index++]);
    }
  }
  return sorted_array;
}

void MergeSort(std::vector<std::pair<int, int>>& values,
               std::vector<int>& answers, size_t left = 0, size_t right = 0) {
  if (right == 0) {
    right = values.size();
  }
  if (right - left == 2) {
    if (!SortCondition(values[left], values[right - 1])) {
      ++answers[values[left].second];
      std::swap(values[left], values[right - 1]);
    }
  } else if (right - left > 2) {
    size_t medium = (left + right) / 2;
    MergeSort(values, answers, left, medium);
    MergeSort(values, answers, medium, right);
    auto sorted_array = Merge(values, answers, left, right);
    for (size_t i = left; i < right; ++i) {
      values[i] = sorted_array[i - left];
    }
  }
}

int main() {
  int amount_of_elements;
  std::cin >> amount_of_elements;
  std::vector<std::pair<int, int>> values(amount_of_elements);
  int index = 0;
  for (auto& element : values) {
    std::cin >> element.first;
    element.second = index;
    ++index;
  }
  std::vector<int> answers(values.size());
  MergeSort(values, answers);
  for (auto element : answers) {
    std::cout << element << " ";
  }
}