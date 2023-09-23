#include <iostream>
#include <vector>

struct progression {
    int base;
    int step;
};

int main() {
    int n;
    std::cin >> n;
    std::vector<int> a(n);
    for (auto &el : a) {
        std::cin >> el;
    }

    std::vector<progression> points(n + 1, {.base = 0, .step = 0});
    std::vector<int> additional_value_change(n + 1, 0);

    int q;
    std::cin >> q;
    for (int step = 0; step < q; ++step) {
        int l, r, b, d;
        std::cin >> l >> r >> b >> d;
        points[l].base += b;
        points[r + 1].base -= b;
        points[l].step += d;
        points[r + 1].step -= d;
        additional_value_change[r + 1] -= d * (r - l + 1);
        int val = 0;
        int cur_d = 0, cur_b = 0;
        for (int i = 0; i < n; ++i) {
            cur_d += points[i].step;
            cur_b += points[i].base;
            val += cur_d + additional_value_change[i];
            int cur = a[i] + val + cur_b;
            std::cout << cur << " ";
        }
        std::cout << std::endl;
    }
}