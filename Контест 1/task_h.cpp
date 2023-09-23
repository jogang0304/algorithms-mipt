#include <iostream>
#include <stack>
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

std::pair<size_t, size_t> Partition(std::vector<int>& values, size_t left,
                                    size_t right, int pivot) {
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
  while (left_pointer + 1 < values.size() && values[left_pointer] <= pivot) {
    ++left_pointer;
  }
  while (right_pointer >= 1 && values[right_pointer] >= pivot) {
    --right_pointer;
  }
  // values[right_pointer] - последний элемент, < pivot
  // values[left_pointer] - первый элемент, > pivot
  return {right_pointer, left_pointer};
}

const int kMaxArraySizeForMedian = 10200;
int Median(std::vector<int>& values) {
  int sorted_values[kMaxArraySizeForMedian * 2];
  sorted_values[kMaxArraySizeForMedian] = values[0];
  int maximum_index = kMaxArraySizeForMedian;
  int minimum_index = kMaxArraySizeForMedian;
  for (size_t i = 1; i < values.size(); ++i) {
    if (values[i] >= sorted_values[maximum_index]) {
      ++maximum_index;
      sorted_values[maximum_index] = values[i];
    } else {
      --minimum_index;
      sorted_values[minimum_index] = values[i];
    }
  }
  return sorted_values[(maximum_index + minimum_index) / 2 +
                       ((maximum_index + minimum_index) % 2)];
}
// position - индекс (нумерация с 0)
int QuickSelect(std::vector<int>& values, size_t position) {
  int answer;
  auto left = 0;
  auto right = values.size();
  std::vector<int> medians(0);
  std::vector<int> five_numbers(0);
  while (true) {
    medians.resize(0);
    medians.reserve((right - left) / 5);
    for (size_t i = left; i < right; i += 5) {
      five_numbers.resize(0);
      five_numbers.reserve(5);
      for (size_t j = i; j < i + 5 && j < right; ++j) {
        five_numbers.push_back(values[j]);
      }
      medians.push_back(Median(five_numbers));
    }
    int pivot;
    if (medians.size() <= kMaxArraySizeForMedian) {
      pivot = Median(medians);
    } else {
      pivot = QuickSelect(medians, medians.size() / 2);
    }

    auto part = Partition(values, left, right, pivot);
    if (position <= part.first) {
      right = part.first + 1;
    } else if (position > part.first && position < part.second) {
      // между part.first и part.second все элементы одинаковые и равные pivot
      answer = values[position];
      break;
    } else {
      left = part.second;
    }
  }
  return answer;
}

void QuickSort(std::vector<int>& values) {
  std::stack<std::pair<size_t, size_t>> sort_stack;
  sort_stack.push({0, values.size()});
  while (!sort_stack.empty()) {
    auto left = sort_stack.top().first;
    auto right = sort_stack.top().second;
    sort_stack.pop();
    auto pivot = QuickSelect(values, (left + right) / 2);
    auto part = Partition(values, left, right, pivot);
    if (part.first + 1 - left > 1) {
      sort_stack.push({left, part.first + 1});
    }
    if (right - part.second > 1) {
      sort_stack.push({part.second, right});
    }
  }
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