#include <iostream>
#include <vector>

bool Equal(int left, int right) { return left == right; }

bool SortCondition(int left, int right) { return left <= right; }

std::vector<int> Merge(std::vector<int>& values, long long& answer, size_t left,
                       size_t right) {
  size_t medium = (left + right) / 2;
  size_t left_part_index = left;
  size_t right_part_index = medium;
  std::vector<int> sorted_array(0);
  while (left_part_index < medium || right_part_index < right) {
    if (left_part_index < medium && right_part_index < right) {
      if (SortCondition(values[left_part_index], values[right_part_index])) {
        sorted_array.push_back(values[left_part_index++]);
      } else {
        sorted_array.push_back(values[right_part_index++]);
        answer += (medium - left_part_index);
      }
    } else if (left_part_index < medium) {
      sorted_array.push_back(values[left_part_index++]);
    } else {
      sorted_array.push_back(values[right_part_index++]);
    }
  }
  return sorted_array;
}

// сортирует отрезок с индексами [left, right)
long long MergeSort(std::vector<int>& values, size_t left = 0,
                    size_t right = 0) {
  if (right == 0) {
    right = values.size();
  }
  long long answer = 0;
  if (right - left == 2) {
    if (!SortCondition(values[left], values[right - 1])) {
      ++answer;
      std::swap(values[left], values[right - 1]);
    }
  } else if (right - left > 2) {
    size_t medium = (left + right) / 2;
    answer += MergeSort(values, left, medium);
    answer += MergeSort(values, medium, right);
    auto sorted_array = Merge(values, answer, left, right);
    for (size_t i = left; i < right; ++i) {
      values[i] = sorted_array[i - left];
    }
  }
  return answer;
}

int main() {
  int amount_of_elements;
  std::cin >> amount_of_elements;
  std::vector<int> values(amount_of_elements);
  for (auto& element : values) {
    std::cin >> element;
  }
  std::cout << MergeSort(values);
}
