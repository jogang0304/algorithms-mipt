#include <deque>
#include <iostream>

void NormalizeHalves(std::deque<int>& first_half,
                     std::deque<int>& second_half) {
  if (first_half.size() < second_half.size()) {
    first_half.push_back(second_half.front());
    second_half.pop_front();
  }
  if (first_half.size() > second_half.size() + 1) {
    second_half.push_front(first_half.back());
    first_half.pop_back();
  }
}

int main() {
  size_t amount_of_actions;
  std::cin >> amount_of_actions;
  std::deque<int> first_half;
  std::deque<int> second_half;
  for (size_t step = 0; step < amount_of_actions; ++step) {
    std::string command;
    std::cin >> command;
    if (command == "-") {
      std::cout << first_half.front() << std::endl;
      first_half.pop_front();
    } else {
      int number;
      std::cin >> number;
      if (command == "+") {
        second_half.push_back(number);
      } else if (command == "*") {
        first_half.push_back(number);
      }
    }
    NormalizeHalves(first_half, second_half);
  }
}