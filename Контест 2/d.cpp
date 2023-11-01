#include <algorithm>
#include <deque>
#include <iostream>
#include <queue>
#include <stack>
#include <vector>

const int kErrorCode = -2'000'000'000;

class QueueThroughStack {
 private:
  std::stack<int> stack1_;
  std::stack<int> stack2_;
  std::stack<int> stack1_min_;
  std::stack<int> stack2_min_;

  void MoveStack1ToStack2() {
    while (!stack1_.empty()) {
      stack2_.push(stack1_.top());
      if (!stack2_min_.empty()) {
        stack2_min_.push(std::min(stack2_min_.top(), stack1_.top()));
      } else {
        stack2_min_.push(stack1_.top());
      }
      stack1_.pop();
      stack1_min_.pop();
    }
  }

 public:
  QueueThroughStack() {}
  void Push(int number) {
    stack1_.push(number);
    if (!stack1_min_.empty()) {
      stack1_min_.push(std::min(stack1_min_.top(), number));
    } else {
      stack1_min_.push(number);
    }
  }

  int Pop() {
    if (stack2_.empty() && stack1_.empty()) {
      return kErrorCode;
    }
    if (stack2_.empty()) {
      MoveStack1ToStack2();
    }
    int answer = stack2_.top();
    stack2_.pop();
    stack2_min_.pop();
    return answer;
  }

  int Front() {
    if (stack2_.empty() && stack1_.empty()) {
      return kErrorCode;
    }
    if (stack2_.empty()) {
      MoveStack1ToStack2();
    }
    return stack2_.top();
  }

  int Size() { return stack1_.size() + stack2_.size(); }

  int Min() {
    if (stack1_min_.empty() && stack2_min_.empty()) {
      return kErrorCode;
    }
    if (stack1_min_.empty()) {
      return stack2_min_.top();
    }
    if (stack2_min_.empty()) {
      return stack1_min_.top();
    }
    return std::min(stack1_min_.top(), stack2_min_.top());
  }

  void Clear() {
    stack1_ = std::stack<int>();
    stack1_min_ = std::stack<int>();
    stack2_ = std::stack<int>();
    stack2_min_ = std::stack<int>();
  }
};

class Hat {
 private:
  QueueThroughStack queue_;

 public:
  Hat() {}

  void Enqueue(int number) {
    queue_.Push(number);
    std::cout << "ok" << std::endl;
  }

  int Dequeue() { return queue_.Pop(); }

  int Front() { return queue_.Front(); }

  int Size() { return queue_.Size(); }

  int Min() { return queue_.Min(); }

  void Clear() {
    queue_.Clear();
    std::cout << "ok" << std::endl;
  }
};

void Print(int answer) {
  std::cout << (answer == kErrorCode ? "error" : std::to_string(answer))
            << std::endl;
}

int main() {
  int amount_of_commands;
  std::cin >> amount_of_commands;
  Hat hat;
  for (int step = 0; step < amount_of_commands; ++step) {
    std::string command;
    std::cin >> command;
    if (command == "enqueue") {
      int number;
      std::cin >> number;
      hat.Enqueue(number);
    } else if (command == "dequeue") {
      Print(hat.Dequeue());
    } else if (command == "front") {
      Print(hat.Front());
    } else if (command == "size") {
      Print(hat.Size());
    } else if (command == "clear") {
      hat.Clear();
    } else if (command == "min") {
      Print(hat.Min());
    }
  }
}
