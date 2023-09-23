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

std::pair<size_t, size_t> Partition(std::vector<int>& values, size_t left,
                                    size_t right) {
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

// position - индекс (нумерация с 0)
int QuickSelect(std::vector<int>& values, size_t position, size_t left = 0,
                size_t right = 0) {
  if (right == 0) {
    right = values.size();
  }
  auto part = Partition(values, left, right);
  int answer = 0;
  if (position <= part.first) {
    answer = QuickSelect(values, position, left, part.first + 1);
  } else if (position > part.first && position < part.second) {
    // между part.first и part.second все элементы одинаковые и равные pivot
    return values[position];
  } else {
    answer = QuickSelect(values, position, part.second, right);
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