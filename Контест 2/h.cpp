#include <iostream>
#include <vector>

namespace QuickSelect {
bool SortCondition(int left_value, int right_value) {
  return left_value <= right_value;
}

bool Equal(int left_value, int right_value) {
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

// position - индекс (нумерация с 0)
std::pair<int, size_t> QuickSelect(std::vector<int>& values, size_t position) {
  int answer;
  size_t answer_index;
  auto left = 0;
  auto right = values.size();
  while (true) {
    auto part = Partition(values, left, right, values[(left + right) / 2]);
    if (position <= part.first) {
      right = part.first + 1;
    } else if (position > part.first && position < part.second) {
      // между part.first и part.second все элементы одинаковые и равные pivot
      answer = values[position];
      answer_index = position;
      break;
    } else {
      left = part.second;
    }
  }
  return {answer, answer_index};
}
}  // namespace QuickSelect

namespace Heap {
class Heap {
  /*
    Все левые дети - меньше, все правые - больше или равны
  */
 private:
  size_t SiftUp(size_t index);
  void SiftDown(size_t index);

 public:
  std::vector<int> elements;
  Heap();
  void Insert(int value);
  int GetMin();
  int ExtractMin();
  int GetMax();
  int ExtractMax();
  size_t Size();
  void Clear();
};

void Heap::SiftDown(size_t index) {
  size_t left_child_index = index * 2 + 1;
  size_t right_child_index = index * 2 + 2;
  if (left_child_index >= elements.size()) {
    return;
  } else if (right_child_index >= elements.size()) {
    if (elements[index] < elements[left_child_index]) {
      std::swap(elements[left_child_index], elements[index]);
      SiftDown(left_child_index);
    }
  } else {
    if (elements[index] > elements[right_child_index]) {
      std::swap(elements[index], elements[right_child_index]);
      SiftDown(right_child_index);
    } else if (elements[index] < elements[left_child_index]) {
      std::swap(elements[index], elements[left_child_index]);
      SiftDown(left_child_index);
    }
    SiftDown(index);
  }
}

size_t Heap::SiftUp(size_t index) {
  if (index == 0) {
    return index;
  }
  size_t parent_index = (index - 1) / 2;
  bool is_left_child = true;
  if (parent_index * 2 + 1 != index) {
    is_left_child = false;
  }
  if ((is_left_child && elements[index] > elements[parent_index]) ||
      (!is_left_child && elements[index] < elements[parent_index])) {
    std::swap(elements[index], elements[parent_index]);
    return SiftUp(parent_index);
  }
  return index;
}

size_t Heap::Size() { return elements.size(); }

void Heap::Insert(int value) {
  elements.push_back(value);
  size_t stopped_at = SiftUp(elements.size() - 1);
  SiftDown(stopped_at);
}

Heap::Heap() { elements.resize(0); }

Heap Make_Heap(std::vector<int>& vec) {
  size_t root_index = QuickSelect::QuickSelect(vec, vec.size() / 2).second;
  Heap answer;
  answer.Insert(vec[root_index]);
  for (size_t i = 0; i < vec.size(); ++i) {
    if (i != root_index) {
      answer.Insert(vec[i]);
    }
  }
  return answer;
}

int Heap::GetMin() {
  size_t cur_index = 0;
  size_t left_child_index = cur_index * 2 + 1;
  while (left_child_index < elements.size()) {
    cur_index = left_child_index;
    left_child_index = cur_index * 2 + 1;
  }
  return elements[left_child_index];
}

int Heap::GetMax() {
  size_t cur_index = 0;
  size_t right_child_index = cur_index * 2 + 2;
  while (right_child_index < elements.size()) {
    cur_index = right_child_index;
    right_child_index = cur_index * 2 + 1;
  }
  return elements[right_child_index];
}





}  // namespace Heap

int main() {
  
}





