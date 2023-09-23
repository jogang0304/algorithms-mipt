#include <algorithm>
#include <iostream>
#include <vector>

int main() {
    int n;
    std::cin >> n;
    std::vector<int> a(n), b(n);
    for (auto &x : a) {
        std::cin >> x;
    }
    for (auto &x : b) {
        std::cin >> x;
    }
    int max_a = -1e9;
    int ans = -1e9;
    for (int i = 0; i < n; ++i) {
        max_a = std::max(max_a, a[i]);
        ans = std::max(ans, max_a + b[i]);
    }
    std::cout << ans << "\n";
}