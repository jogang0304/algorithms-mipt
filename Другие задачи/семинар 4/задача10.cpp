#include <iostream>
#include <stack>
#include <vector>

int main() {
  int n;
  std::cin >> n;
  std::vector<std::string> v(n);
  for (auto& x : v) {
    std::cin >> x;
  }
  std::stack<int> s;
  for (auto x : v) {
    int number;
    bool isNumber = false;
    if (x != "+" && x != "-" && x != "*" && x != "/") {
      number = std::stoi(x);
      isNumber = true;
    }
    if (isNumber) {
      s.push(number);
    } else {
      int el2 = s.top();
      s.pop();
      int el1 = s.top();
      s.pop();
      int result;
      if (x == "+")
        result = el1 + el2;
      else if (x == "-")
        result = el1 - el2;
      else if (x == "*")
        result = el1 * el2;
      else if (x == "/")
        result = el1 / el2;
      s.push(result);
    }
  }
  std::cout << s.top() << "\n";
}