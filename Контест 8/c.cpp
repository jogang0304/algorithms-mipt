#include <algorithm>
#include <iostream>
#include <set>
#include <vector>

const int cInf = 1'000'000'000;

struct Path {
  int destination;
  int price;
  int time;
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
  int price = cInf;
  int time = cInf;
  size_t came_from = cInf;

  bool operator<(const Vertex& other) const {
    if (price != other.price) {
      return price < other.price;
    }
    return time < other.time;
  }
};

int GetIndexByEnds(const std::vector<std::vector<Path>>& graph, int from,
                   int to) {
  int ans =
      std::lower_bound(graph[from].begin(), graph[from].end(), Path{to, 0, 0}) -
      graph[from].begin();
  return ans;
}

std::vector<size_t> GetMinPricePath(
    const std::vector<std::vector<Path>>& graph,
    const std::vector<std::vector<Vertex>>& vertices, int t) {
  Vertex best_end;
  for (int i = 0; i <= t; ++i) {
    if (vertices[i].back().price < best_end.price) {
      best_end = vertices[i].back();
    }
  }
  std::vector<size_t> ans(0);
  if (best_end.time > t) {
    return ans;
  }
  int current_index = graph.size() - 1;
  while (current_index != 0) {
    ans.push_back(current_index);
    int index = GetIndexByEnds(graph, best_end.came_from, current_index);
    current_index = best_end.came_from;
    best_end = vertices[best_end.time - graph[best_end.came_from][index].time]
                       [best_end.came_from];
  }
  ans.push_back(0);
  return ans;
}

std::vector<size_t> Dijkstra(const std::vector<std::vector<Path>>& graph,
                             int t) {
  std::vector<std::vector<Vertex>> vertices(
      t + 1,
      std::vector<Vertex>(graph.size()));  // v[i][j] - min price if vertex j
                                           // is visited at time i.
  vertices[0][0] = {0, 0, 0};
  std::set<std::pair<Vertex, int>> queue;
  queue.insert({vertices[0][0], 0});
  while (!queue.empty()) {
    auto current = *queue.begin();
    queue.erase(queue.begin());
    int current_index = current.second;
    for (auto path : graph[current_index]) {
      int new_time = current.first.time + path.time;
      int new_price = current.first.price + path.price;
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
  return GetMinPricePath(graph, vertices, t);
}

int main() {
  int n;
  int m;
  int t;
  std::cin >> n >> m >> t;
  std::vector<std::vector<Path>> graph(n, std::vector<Path>(0));
  for (int i = 0; i < m; ++i) {
    int start;
    int end;
    int price;
    int time;
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
  int price = 0;
  for (size_t i = 0; i + 1 < res.size(); ++i) {
    int index = GetIndexByEnds(graph, res[i], res[i + 1]);
    price += graph[res[i]][index].price;
  }
  std::cout << price << "\n" << res.size() << "\n";
  for (auto vertex : res) {
    std::cout << vertex + 1 << " ";
  }
}