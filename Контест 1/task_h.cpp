#include <iostream>
#include <vector>

bool SortCondition(int left_value, int right_value) {
  return left_value <= right_value;
}

bool Equal(int left_value, int right_value) {
  return left_value == right_value;
}

int ChoosePivotIndex(int left, int right) {
  // сделать выбор медианы через deterministic quick select
  return (right + left) / 2;
}

int Partition(std::vector<int>& values, int left, int right) {
  auto pivot = values[ChoosePivotIndex(left, right)];
  auto left_pointer = left;
  auto right_pointer = right - 1;
  while (left_pointer <= right_pointer) {
    while (!Equal(values[left_pointer], pivot) &&
           SortCondition(values[left_pointer], pivot)) {
      ++left_pointer;
    }
    while (!Equal(values[right_pointer], pivot) &&
           SortCondition(pivot, values[right_pointer])) {
      --right_pointer;
    }
    if (left_pointer <= right_pointer) {
      std::swap(values[left_pointer], values[right_pointer]);
      ++left_pointer;
      --right_pointer;
    }
  }
  return right_pointer;
}

void QuickSort(std::vector<int>& values, int left = 0, int right = -1) {
  if (right == -1) {
    right = values.size();
  }
  if (right - left <= 1) {
    return;
  }
  auto part = Partition(values, left, right);
  QuickSort(values, left, part + 1);
  QuickSort(values, part + 1, right);
}

int main() {
  int count_elements;
  std::cin >> count_elements;
  std::vector<int> values(count_elements);
  for (auto& element : values) {
    std::cin >> element;
  }

  QuickSort(values);
  for (auto x : values) {
    std::cout << x << " ";
  }
}