#include <iostream>
#include <stack>
#include <vector>

int main() {
  int n;
  std::cin >> n;
  std::vector<int> v(n);
  for (auto& x : v) {
    std::cin >> x;
  }
  // пункт а
  std::vector<int> f(n, -1);  // answers
  std::stack<int> s;
  s.push(n - 1);
  for (int i = n - 2; i >= 0; i--) {
    if (v[i] < v[s.top()]) {
      f[i] = s.top();
    } else {
      while (!s.empty() && !(v[i] < v[s.top()])) {
        s.pop();
      }
      if (s.empty()) {
        f[i] = -1;
      } else {
        f[i] = s.top();
      }
    }
    s.push(i);
  }
  for (auto& x : f) {
    std::cout << x << " ";
  }
  std::cout << "\n";
}
