#include <iostream>
#include <set>
#include <vector>

const long long kErrorCode = -10;

class BinaryHeap {
 private:
  bool isMinHeap_;
  std::vector<std::pair<long long, size_t>> elements_;
  std::set<size_t> deleted_indexes_;
  inline static bool CompareForMinHeap(std::pair<long long, size_t>& left,
                                       std::pair<long long, size_t>& right) {
    return left.first < right.first;
  }
  void SiftUp(size_t index) {
    if (index <= 1) {
      return;
    }
    size_t parent_index = index / 2;
    if (isMinHeap_ ==
        CompareForMinHeap(elements_[index], elements_[parent_index])) {
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
        (isMinHeap_ ==
         CompareForMinHeap(elements_[2 * index + 1], elements_[2 * index]))) {
      child_index = 2 * index + 1;
    }
    if (isMinHeap_ ==
        CompareForMinHeap(elements_[child_index], elements_[index])) {
      std::swap(elements_[child_index], elements_[index]);
      SiftDown(child_index);
    }
  }
  void ExtractDeletedRoots() {
    while (true) {
      if (deleted_indexes_.find(elements_[1].second) ==
          deleted_indexes_.end()) {
        break;
      }
      deleted_indexes_.erase(elements_[1].second);
      this->ExtractRoot();
    }
  }

 public:
  BinaryHeap(bool is_min_heap = true) {
    isMinHeap_ = is_min_heap;
    elements_.resize(1);
    deleted_indexes_.clear();
  }
  BinaryHeap(std::vector<std::pair<long long, size_t>>& values,
             bool is_min_heap = true) {
    isMinHeap_ = is_min_heap;
    elements_ = values;
    for (long long i = elements_.size() - 1; i >= 1; --i) {
      SiftDown(i);
    }
  }
  std::pair<long long, size_t> GetRoot() {
    ExtractDeletedRoots();
    if (elements_.size() > 1) {
      return elements_[1];
    }
    return {kErrorCode, -1};
  }
  void ExtractRoot() {
    ExtractDeletedRoots();
    if (elements_.size() > 1) {
      std::swap(elements_[1], elements_[elements_.size() - 1]);
      elements_.pop_back();
      SiftDown(1);
    }
  }
  void Insert(std::pair<long long, size_t>& value) {
    elements_.push_back(value);
    SiftUp(elements_.size() - 1);
  }
  size_t Size() {
    ExtractDeletedRoots();
    return elements_.size() - 1 - deleted_indexes_.size();
  }
  void Clear() {
    elements_.resize(1);
    deleted_indexes_.clear();
  }
  void AddDeletedIndex(size_t index) { deleted_indexes_.insert(index); }
};

class MinMaxHeap {
 private:
  BinaryHeap MinHeap_;
  BinaryHeap MaxHeap_;
  size_t index_;

 public:
  MinMaxHeap() {
    MinHeap_ = BinaryHeap();
    MaxHeap_ = BinaryHeap(false);
    index_ = 0;
  }
  void Insert(long long value) {
    std::pair<long long, size_t> to_insert = {value, ++index_};
    MinHeap_.Insert(to_insert);
    MaxHeap_.Insert(to_insert);
  }

  long long ExtractMin() {
    auto answer = MinHeap_.GetRoot();
    if (answer.first != kErrorCode) {
      MinHeap_.ExtractRoot();
      MaxHeap_.AddDeletedIndex(answer.second);
    }
    return answer.first;
  }
  long long GetMin() {
    auto answer = MinHeap_.GetRoot();
    return answer.first;
  }
  long long ExtractMax() {
    auto answer = MaxHeap_.GetRoot();
    if (answer.first != kErrorCode) {
      MaxHeap_.ExtractRoot();
      MinHeap_.AddDeletedIndex(answer.second);
    }
    return answer.first;
  }
  long long GetMax() {
    auto answer = MaxHeap_.GetRoot();
    return answer.first;
  }
  long long Size() { return MinHeap_.Size(); }
  void Clear() {
    MinHeap_.Clear();
    MaxHeap_.Clear();
  }
};

void PrintAnswer(long long ans) {
  if (ans == kErrorCode) {
    std::cout << "error" << std::endl;
  } else {
    std::cout << ans << std::endl;
  }
}

int main() {
  size_t amount_of_questions;
  std::cin >> amount_of_questions;
  MinMaxHeap heap;
  for (size_t step = 0; step < amount_of_questions; ++step) {
    std::string command;
    std::cin >> command;
    if (command == "insert") {
      long long value;
      std::cin >> value;
      heap.Insert(value);
      std::cout << "ok" << std::endl;
    } else if (command == "extract_min") {
      auto ans = heap.ExtractMin();
      PrintAnswer(ans);
    } else if (command == "get_min") {
      auto ans = heap.GetMin();
      PrintAnswer(ans);
    } else if (command == "extract_max") {
      auto ans = heap.ExtractMax();
      PrintAnswer(ans);
    } else if (command == "get_max") {
      auto ans = heap.GetMax();
      PrintAnswer(ans);
    } else if (command == "size") {
      auto ans = heap.Size();
      PrintAnswer(ans);
    } else if (command == "clear") {
      heap.Clear();
      std::cout << "ok" << std::endl;
    }
  }
}
