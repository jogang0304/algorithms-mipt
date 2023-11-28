#include <iostream>
#include <vector>

const long long kModule = 1'073'741'824;

class BinaryHeap {
 private:
  bool isMinHeap_;
  std::vector<int> elements_;
  void SiftUp(size_t index) {
    if (index <= 1) {
      return;
    }
    size_t parent_index = index / 2;
    if ((isMinHeap_ && elements_[index] < elements_[parent_index]) ||
        (!isMinHeap_ && elements_[index] > elements_[parent_index])) {
      std::swap(elements_[index], elements_[parent_index]);
      SiftUp(parent_index);
    }
  }
  void SiftDown(size_t index) {
    if (2 * index > elements_.size() - 1) {
      return;
    }
    size_t child_index = 2 * index;
    if (2 * index + 1 <= elements_.size() - 1 &&
        ((isMinHeap_ && elements_[2 * index + 1] < elements_[2 * index]) ||
         (!isMinHeap_ && elements_[2 * index + 1] > elements_[2 * index]))) {
      child_index = 2 * index + 1;
    }
    if ((isMinHeap_ && elements_[child_index] < elements_[index]) ||
        (!isMinHeap_ && elements_[child_index] > elements_[index])) {
      std::swap(elements_[child_index], elements_[index]);
      SiftDown(child_index);
    }
  }

 public:
  BinaryHeap(bool is_min_heap = true) {
    isMinHeap_ = is_min_heap;
    elements_.resize(1);
  }
  BinaryHeap(std::vector<int>& values, bool is_min_heap = true) {
    isMinHeap_ = is_min_heap;
    elements_ = values;
    for (int i = elements_.size() - 1; i >= 1; --i) {
      SiftDown(i);
    }
  }
  int GetRoot() {
    if (elements_.size() > 1) {
      return elements_[1];
    }
    return 0;
  }
  void ExtractRoot() {
    if (elements_.size() > 1) {
      std::swap(elements_[1], elements_[elements_.size() - 1]);
      elements_.pop_back();
      SiftDown(1);
    }
  }
  void Insert(int value) {
    elements_.push_back(value);
    SiftUp(elements_.size() - 1);
  }
  void DecreaseKey(int index, int delta) {
    if (delta <= 0 || index < 0 || elements_.size() <= 1) {
      return;
    }
    elements_[index + 1] -= delta;
    SiftUp(index + 1);
  }
  size_t Size() { return elements_.size() - 1; }

  int operator[](size_t index) {
    if (elements_.size() <= 1) {
      return 0;
    }
    return elements_[index + 1];
  }
};

void ChangeMaxElementToNew(BinaryHeap& max_heap, BinaryHeap& min_heap,
                           int new_a) {
  auto max_element_in_heap = max_heap.GetRoot();
  size_t index_in_min_heap = 0;
  while (min_heap[index_in_min_heap] != max_element_in_heap) {
    ++index_in_min_heap;
  }
  min_heap.DecreaseKey(index_in_min_heap, max_element_in_heap + 1);
  min_heap.ExtractRoot();
  max_heap.ExtractRoot();
  min_heap.Insert(new_a);
  max_heap.Insert(new_a);
}

int main() {
  size_t sequence_length;
  size_t needed_amount;
  std::cin >> sequence_length >> needed_amount;
  long long a_zero;
  long long x_variable;
  long long y_variable;
  std::cin >> a_zero >> x_variable >> y_variable;
  BinaryHeap min_heap;
  BinaryHeap max_heap(false);  // root - max{min_heap}

  long long last_a = a_zero;
  for (size_t i = 0; i < sequence_length; ++i) {
    int new_a = static_cast<int>((last_a * x_variable + y_variable) % kModule);
    if (min_heap.Size() < needed_amount) {
      min_heap.Insert(new_a);
      max_heap.Insert(new_a);
    } else if (new_a < max_heap.GetRoot()) {
      ChangeMaxElementToNew(max_heap, min_heap, new_a);
    }
    last_a = static_cast<long long>(new_a);
  }
  while (min_heap.Size() > 0) {
    std::cout << min_heap.GetRoot() << " ";
    min_heap.ExtractRoot();
  }
}