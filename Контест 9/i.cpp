#include <iostream>
#include <vector>

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(NULL);
  std::cout.tie(NULL);
  short m;
  short n;
  std::cin >> m >> n;
  std::vector<std::vector<short>> graph(
      m + n);  // 0 - m-1 - first part, m - m+n-1 - second part
  for (short i = 0; i < m; ++i) {
    short k;
    std::cin >> k;
    graph[i].reserve(k);
    for (int j = 0; j < k; ++j) {
      short cur;
      std::cin >> cur;
      graph[i].push_back(m + cur - 1);
    }
  }
  short min_control_set_size = 0;
  std::vector<bool> is_visited(m + n, false);
  std::vector<short> queue(0);
  queue.reserve(m + n);
  for (short i = 0; i < m; ++i) {
    short cur;
    std::cin >> cur;
    if (cur == 0) {
      is_visited[i] = true;
      queue.push_back(i);
      continue;
    }
    min_control_set_size++;
    graph[m + cur - 1].push_back(i);
  }
  short left_visited_count = 0;
  short right_visited_count = 0;
  while (!queue.empty()) {
    auto cur = queue.back();
    queue.pop_back();
    if (cur < m) {
      ++left_visited_count;
    } else {
      ++right_visited_count;
    }
    for (auto next : graph[cur]) {
      if (!is_visited[next]) {
        queue.push_back(next);
        is_visited[next] = true;
      }
    }
  }
  std::cout << min_control_set_size << "\n" << m - left_visited_count << " ";
  for (short i = 0; i < m; ++i) {
    if (!is_visited[i]) {
      std::cout << i + 1 << " ";
    }
  }
  std::cout << "\n" << right_visited_count << " ";
  for (short i = m; i < m + n; ++i) {
    if (is_visited[i]) {
      std::cout << i - m + 1 << " ";
    }
  }
}