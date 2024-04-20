#include <algorithm>
#include <iostream>
#include <set>
#include <vector>

const int64_t cInf = 1'000'000'000'000;

struct Path {
  int64_t destination;
  int64_t price;
  int64_t time;
  bool operator<(const Path& other) const {
    if (destination != other.destination) {
      return destination < other.destination;
    }
    if (price != other.price) {
      return price < other.price;
    }
    return time < other.time;
  }
};

struct Vertex {
  int64_t price = cInf;
  int64_t time = cInf;
  int64_t came_from = cInf;

  bool operator<(const Vertex& other) const {
    if (price != other.price) {
      return price < other.price;
    }
    if (time != other.time) {
      return time < other.time;
    }
    return came_from < other.came_from;
  }
};

int64_t GetIndexByEnds(const std::vector<std::vector<Path>>& graph,
                       int64_t from, int64_t to) {
  int64_t ans =
      std::lower_bound(graph[from].begin(), graph[from].end(), Path{to, 0, 0}) -
      graph[from].begin();
  return ans;
}

std::vector<int64_t> GetMinPricePath(
    const std::vector<std::vector<Path>>& graph,
    const std::vector<std::vector<Vertex>>& vertices, int64_t t) {
  Vertex best_end;
  for (int64_t i = 0; i <= t; ++i) {
    if (vertices[i].back().price < best_end.price) {
      best_end = vertices[i].back();
    }
  }
  std::vector<int64_t> ans(0);
  if (best_end.time > t) {
    return ans;
  }
  int64_t current_index = static_cast<int64_t>(graph.size()) - 1;
  while (current_index >= 0) {
    ans.push_back(current_index);
    current_index = best_end.came_from;
    if (current_index < 0) {
      break;
    }
    auto index = GetIndexByEnds(graph, best_end.came_from, ans.back());
    best_end = vertices[best_end.time - graph[best_end.came_from][index].time]
                       [best_end.came_from];
  }
  return ans;
}

std::vector<int64_t> Dijkstra(const std::vector<std::vector<Path>>& graph,
                              int64_t t) {
  std::vector<std::vector<Vertex>> vertices(
      t + 1,
      std::vector<Vertex>(graph.size()));  // v[i][j] - min price if vertex j
                                           // is visited at time i.
  vertices[0][0] = {0, 0, -1};
  std::set<std::pair<Vertex, int64_t>> queue;
  queue.insert({vertices[0][0], 0});
  while (!queue.empty()) {
    auto current = *queue.begin();
    queue.erase(queue.begin());
    auto current_index = current.second;
    for (auto path : graph[current_index]) {
      int64_t new_time = current.first.time + path.time;
      int64_t new_price = current.first.price + path.price;
      if (new_time > t) {
        continue;
      }
      if (new_price < vertices[new_time][path.destination].price) {
        queue.erase({vertices[new_time][path.destination], path.destination});
        vertices[new_time][path.destination].price = new_price;
        vertices[new_time][path.destination].time = new_time;
        vertices[new_time][path.destination].came_from = current_index;
        queue.insert({vertices[new_time][path.destination], path.destination});
      }
    }
  }
  auto ans = GetMinPricePath(graph, vertices, t);
  return ans;
}

int main() {
  int64_t n;
  int64_t m;
  int64_t t;
  std::cin >> n >> m >> t;
  std::vector<std::vector<Path>> graph(n, std::vector<Path>(0));
  for (int64_t i = 0; i < m; ++i) {
    int64_t start;
    int64_t end;
    int64_t price;
    int64_t time;
    std::cin >> start >> end >> price >> time;
    graph[start - 1].push_back({end - 1, price, time});
    graph[end - 1].push_back({start - 1, price, time});
  }
  auto res = Dijkstra(graph, t);
  if (res.empty()) {
    std::cout << -1;
    return 0;
  }
  std::reverse(res.begin(), res.end());
  int64_t price = 0;
  for (size_t i = 0; i + 1 < res.size(); ++i) {
    auto index = GetIndexByEnds(graph, res[i], res[i + 1]);
    price += graph[res[i]][index].price;
  }
  std::cout << price << "\n" << res.size() << "\n";
  for (auto vertex : res) {
    std::cout << vertex + 1 << " ";
  }
}