#include <algorithm>
#include <iostream>
#include <queue>
#include <set>
#include <vector>

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(NULL);
  std::cout.tie(NULL);
  int m;
  int n;
  std::cin >> m >> n;
  std::vector<std::vector<int>> graph(
      m + n);  // 0 - m-1 - first part, m - m+n-1 - second part
  for (int i = 0; i < m; ++i) {
    int k;
    std::cin >> k;
    for (int j = 0; j < k; ++j) {
      int cur;
      std::cin >> cur;
      graph[i].push_back(m + cur - 1);
    }
  }
  int min_control_set_size = 0;
  std::vector<bool> left_is_saturated(m, true);
  for (int i = 0; i < m; ++i) {
    int cur;
    std::cin >> cur;
    if (cur == 0) {
      left_is_saturated[i] = false;
      continue;
    }
    min_control_set_size++;
    graph[m + cur - 1].push_back(i);
  }
  std::set<int> left_not_visited;
  for (int i = 0; i < m; ++i) {
    left_not_visited.insert(i);
  }
  std::set<int> right_visited;
  std::vector<bool> global_is_visited(m + n, false);
  std::queue<int> queue;
  for (int i = 0; i < m; ++i) {
    if (!left_is_saturated[i]) {
      queue.push(i);
    }
  }
  while (!queue.empty()) {
    int cur = queue.front();
    queue.pop();
    global_is_visited[cur] = true;
    if (cur < m) {
      left_not_visited.erase(cur);
    } else {
      right_visited.insert(cur);
    }
    for (int next : graph[cur]) {
      if (!global_is_visited[next]) {
        queue.push(next);
      }
    }
  }
  std::cout << min_control_set_size << "\n" << left_not_visited.size() << " ";
  for (int x : left_not_visited) {
    std::cout << x + 1 << " ";
  }
  std::cout << "\n" << right_visited.size() << " ";
  for (int x : right_visited) {
    std::cout << x - m + 1 << " ";
  }
}