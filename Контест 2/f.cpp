#include <iostream>
#include <map>
#include <vector>

const size_t kMaxElementsInHeap = 100'001;
const size_t kMaxAmountOfQuestions = 1'000'001;

class BinaryHeap {
 public:
  BinaryHeap() {
    map_index_to_step_.resize(kMaxElementsInHeap);
    map_step_to_index_.resize(kMaxAmountOfQuestions);
    elements_.resize(1);
  }
  long long GetMin() {
    if (elements_.size() > 1) {
      return elements_[1];
    }
    return 0;
  }
  void ExtractMin() {
    if (elements_.size() > 1) {
      std::swap(elements_[1], elements_[elements_.size() - 1]);
      SwapMaps(1, elements_.size() - 1);
      elements_.pop_back();
      SiftDown(1);
    }
  }
  void Insert(long long value, size_t step) {
    elements_.push_back(value);
    map_step_to_index_[step] = elements_.size() - 1;
    map_index_to_step_[elements_.size() - 1] = step;
    SiftUp(elements_.size() - 1);
  }
  void DecreaseKey(size_t step, long long delta) {
    if (delta <= 0 || step < 1 || elements_.size() <= 1) {
      return;
    }
    elements_[map_step_to_index_[step]] -= delta;
    SiftUp(map_step_to_index_[step]);
  }

 private:
  void SwapMaps(size_t left_index, size_t right_index) {
    std::swap(map_step_to_index_[map_index_to_step_[right_index]],
              map_step_to_index_[map_index_to_step_[left_index]]);
    std::swap(map_index_to_step_[right_index], map_index_to_step_[left_index]);
  }
  void SiftUp(size_t index) {
    if (index <= 1) {
      return;
    }
    long long parent_index = index / 2;
    if (elements_[index] < elements_[parent_index]) {
      std::swap(elements_[index], elements_[parent_index]);
      SwapMaps(index, parent_index);
      SiftUp(parent_index);
    }
  }
  void SiftDown(size_t index) {
    if (2 * index > elements_.size() - 1) {
      return;
    }
    long long child_index = 2 * index;
    if (2 * index + 1 <= elements_.size() - 1 &&
        elements_[2 * index + 1] < elements_[2 * index]) {
      child_index = 2 * index + 1;
    }
    if (elements_[child_index] < elements_[index]) {
      std::swap(elements_[child_index], elements_[index]);
      SwapMaps(child_index, index);
      SiftDown(child_index);
    }
  }

 private:
  std::vector<long long> elements_;
  std::vector<size_t> map_step_to_index_;
  std::vector<size_t> map_index_to_step_;
};

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(NULL);
  size_t amount_of_questions;
  std::cin >> amount_of_questions;
  BinaryHeap heap;
  for (size_t step = 0; step < amount_of_questions; ++step) {
    std::string command;
    std::cin >> command;
    if (command == "insert") {
      long long value;
      std::cin >> value;
      heap.Insert(value, step + 1);
    } else if (command == "decreaseKey") {
      size_t decreasing_step;
      long long delta;
      std::cin >> decreasing_step >> delta;
      heap.DecreaseKey(decreasing_step, delta);
    } else if (command == "getMin") {
      std::cout << heap.GetMin() << std::endl;
    } else if (command == "extractMin") {
      heap.ExtractMin();
    }
  }
}
