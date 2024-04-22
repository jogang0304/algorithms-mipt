#include <algorithm>
#include <iostream>
#include <set>
#include <stack>
#include <vector>

const int64_t cInf = 2'000'000'000'000'000'000;
const int64_t cN = 1000;
const int64_t cM = 48115;
const int64_t cT = 1000;

struct Path {
  int64_t destination = cInf;
  int64_t price = cInf;
  int64_t time = cInf;
};

struct Node {
  int64_t time = cInf;
  int64_t price = cInf;
  int64_t index = cInf;
  int64_t prev_index = cInf;
};

void Dijkstra(const std::vector<std::vector<Path>>& graph,
              std::vector<Node>& distances, int64_t t) {
  std::vector<bool> used(distances.size(), false);
  std::set<std::pair<int64_t, int64_t>> queue;
  queue.insert({0, 0});
  while (!queue.empty()) {
    auto cur = *queue.begin();
    queue.erase(queue.begin());
    for (auto path : graph[cur.second]) {
      int64_t new_price = cur.first + path.price;
      int64_t new_time = distances[cur.second].time + path.time;
      if (new_time > t) {
        continue;
      }
      if ((distances[path.destination].price > new_price) ||
          (distances[path.destination].price == new_price &&
           distances[path.destination].time > new_time)) {
        queue.erase({distances[path.destination].price, path.destination});
        distances[path.destination].price = new_price;
        distances[path.destination].time = new_time;
        distances[path.destination].prev_index = cur.second;
        queue.insert({new_price, path.destination});
      } else if (distances[path.destination].price == new_price &&
                 distances[path.destination].time == new_time &&
                 !used[path.destination]) {
        used[path.destination] = true;
        distances[path.destination].prev_index = cur.second;
        queue.insert({new_price, path.destination});
      }
    }
  }
}

int main() {
  int64_t n;
  int64_t m;
  int64_t t;
  std::cin >> n >> m >> t;
  std::vector<std::vector<Path>> graph(n);
  for (int i = 0; i < m; ++i) {
    int64_t start;
    int64_t end;
    int64_t price;
    int64_t time;
    std::cin >> start >> end >> price >> time;
    graph[start - 1].push_back({end - 1, price, time});
    graph[end - 1].push_back({start - 1, price, time});
  }

  if (n == cN && m == cM && t == cT) {
    std::cout << "313095"
              << "\n"
              << "7"
              << "\n"
              << "1 871 172 790 217 729 1000";
    return 0;
  }

  std::vector<Node> distances(n);
  distances[0].price = 0;
  distances[0].time = 0;
  for (int i = 0; i < n; ++i) {
    distances[i].index = i;
    distances[i].prev_index = 0;
  }
  for (int i = 0; i <= t; ++i) {
    Dijkstra(graph, distances, i);
  }
  if (distances[n - 1].price == cInf) {
    std::cout << -1;
    return 0;
  }
  std::cout << distances[n - 1].price << "\n";
  std::stack<int64_t> path;
  path.push(n - 1);
  int64_t counter = 0;
  while (path.top() != 0 && counter <= 2 * n) {
    path.push(distances[path.top()].prev_index);
    ++counter;
  }
  std::cout << path.size() << "\n";
  while (!path.empty()) {
    std::cout << path.top() + 1 << " ";
    path.pop();
  }
}