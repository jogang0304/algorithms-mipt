#include <iostream>
#include <vector>

struct Action {
  int time;
  bool start;
};

struct Fragment {
  int start_time;
  int end_time;
};

bool SortCondition(Action& left_value, Action& right_value) {
  bool answer = false;
  if ((left_value.time < right_value.time) ||
      ((left_value.time == right_value.time) && left_value.start)) {
    answer = true;
  }
  return answer;
}

bool Equal(Action& left_value, Action& right_value) {
  bool answer = (left_value.time == right_value.time) &&
                (left_value.start == right_value.start);
  return answer;
}

int ChoosePivotIndex(int left, int right) { return (right + left) / 2; }

int Partition(std::vector<Action>& values, int left, int right) {
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

void QuickSort(std::vector<Action>& values, int left = 0, int right = -1) {
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
  int amount_of_fragments;
  std::cin >> amount_of_fragments;
  std::vector<Action> actions(0);
  for (int i = 0; i < amount_of_fragments; ++i) {
    int left;
    int right;
    std::cin >> left >> right;
    actions.push_back({.time = left, .start = true});
    actions.push_back({.time = right, .start = false});
  }
  QuickSort(actions);
  std::vector<Fragment> fragments(0);
  Fragment cur_fragment;
  int cur_sub_fragments = 0;
  for (auto& action : actions) {
    if (action.start) {
      ++cur_sub_fragments;
      if (cur_sub_fragments == 1) {
        cur_fragment.start_time = action.time;
      }
    } else {
      --cur_sub_fragments;
      if (cur_sub_fragments == 0) {
        cur_fragment.end_time = action.time;
        fragments.push_back(cur_fragment);
      }
    }
  }
  std::cout << fragments.size() << "\n";
  for (auto& fragment : fragments) {
    std::cout << fragment.start_time << " " << fragment.end_time << "\n";
  }
}