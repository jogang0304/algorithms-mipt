#include <iostream>
#include <vector>

int main() {
  // пусть это верно для j < n. проверим для j = n.
  // Будем хранить минимальную разницу s между (n-1 - i + 1 + x) и sum[a_i :
  // a_n-1]. Тогда условие верно для j = n, если a_n <= s + 1 Новое s1 = s + 1 -
  // a_n

  int n;
  std::cin >> n;
  std::vector<int> a(n + 1);  // нумерация с 1
  for (int i = 1; i <= n; ++i) {
    std::cin >> a[i];
  }
  int x;
  std::cin >> x;
  bool answer = true;
  int s = (1 - 1 + 1 + x) - a[1];
  for (int i = 2; i <= n; ++i) {
    if (!(a[i] <= s + 1)) {
      answer = false;
      break;
    }
    s = s + 1 - a[i];
  }
  std::cout << answer << "\n";
}