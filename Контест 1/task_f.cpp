#include <iostream>
#include <vector>

inline bool SortCondition(int left_value, int right_value) {
  return left_value <= right_value;
}

inline bool Equal(int left_value, int right_value) {
  return left_value == right_value;
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

int main() {
  int amount_of_values;
  int needed_element;
  int a0;
  int a1;
  std::cin >> amount_of_values >> needed_element >> a0 >> a1;

  if (amount_of_values == 1) {
    std::cout << a0;
    return 0;
  }

  std::vector<int> values(amount_of_values);
  values[0] = a0;
  values[1] = a1;

  const int kPrevRatio = 123;
  const int kPrev2Ratio = 45;
  const int kModule = 10'004'321;
  for (size_t i = 2; i < values.size(); ++i) {
    values[i] =
        (values[i - 1] * kPrevRatio + values[i - 2] * kPrev2Ratio) % kModule;
  }

  std::cout << QuickSelect(values, needed_element - 1);
}