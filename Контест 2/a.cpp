#include <iostream>
#include <stack>

bool Corresponds(char first, char second) {
  return ((first == '(' && second == ')') || (first == '[' && second == ']') ||
          (first == '{' && second == '}'));
}

int main() {
  std::stack<char> bracket_stack;
  std::string input;
  std::cin >> input;
  bool correct = true;
  for (auto bracket : input) {
    if (bracket == ')' || bracket == ']' || bracket == '}') {
      if (bracket_stack.empty() || !Corresponds(bracket_stack.top(), bracket)) {
        correct = false;
        break;
      }
      bracket_stack.pop();
    } else {
      bracket_stack.push(bracket);
    }
  }
  if (!bracket_stack.empty()) {
    correct = false;
  }
  std::cout << (correct ? "YES" : "NO") << std::endl;
}