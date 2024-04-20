#include <algorithm>
#include <iostream>
#include <set>
#include <vector>

const int cInf = 2'000'000'000;

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
  int prev_path_time = cInf;

  bool operator<(const Vertex& other) const {
    // if (time != other.time) {
    return time < other.time;
    // }
    // if (price != other.price) {
    //   return price < other.price;
    // }
    // return came_from < other.came_from;
  }
};

std::vector<int> GetMinTimes(const std::vector<std::vector<Path>>& graph,
                             int start) {
  std::vector<int> times(graph.size(), cInf);
  times[start] = 0;
  std::set<std::pair<int, int>> queue;
  queue.insert({0, start});
  while (!queue.empty()) {
    auto b = *queue.begin();
    queue.erase(queue.begin());
    int v = b.second;
    for (auto u : graph[v]) {
      if (times[u.destination] > times[v] + u.time) {
        queue.erase({times[u.destination], u.destination});
        times[u.destination] = times[v] + u.time;
        queue.insert({times[u.destination], u.destination});
      }
    }
  }
  return times;
}

std::pair<int, std::vector<size_t>> GetMinPricePath(
    const std::vector<std::vector<Path>>& graph,
    const std::vector<std::set<Vertex>>& vertices) {
  Vertex best_end = *(--(vertices[graph.size() - 1].end()));
  int ans_sum = best_end.price;
  std::vector<size_t> ans_vertices(0);
  if (best_end.price >= cInf) {
    return {-1, ans_vertices};
  }
  int current_index = graph.size() - 1;
  while (current_index != 0) {
    ans_vertices.push_back(current_index);
    current_index = best_end.came_from;
    best_end = *(--(vertices[best_end.came_from].upper_bound(
        {cInf, best_end.time - best_end.prev_path_time, cInf, cInf})));
  }
  ans_vertices.push_back(0);
  return {ans_sum, ans_vertices};
}

std::pair<int, std::vector<size_t>> Dijkstra(
    const std::vector<std::vector<Path>>& graph, int t) {
  std::vector<std::set<Vertex>> vertices(
      graph.size(),
      std::set<Vertex>());  // v[i] - set of starting times, from which vertex i
                            // can be reached with price
  auto min_times_till_end =
      GetMinTimes(graph, static_cast<int>(graph.size() - 1));
  vertices[0].insert({0, 0, 0, 0});
  for (size_t i = 1; i < vertices.size(); ++i) {
    vertices[i].insert({cInf, 0, cInf, cInf});
  }
  std::set<std::pair<Vertex, int>> queue;
  queue.insert({*vertices[0].begin(), 0});
  while (!queue.empty()) {
    auto current = *queue.begin();
    queue.erase(queue.begin());
    int current_index = current.second;
    for (auto path : graph[current_index]) {
      int new_time = current.first.time + path.time;
      int new_price = current.first.price + path.price;
      if (new_time + min_times_till_end[path.destination] > t) {
        continue;
      }
      Vertex new_vertex = {new_price, new_time,
                           static_cast<size_t>(current_index), path.time};
      auto pv = --(vertices[path.destination].upper_bound(new_vertex));
      if (new_price < pv->price) {
        if (pv->time >= new_time) {
          queue.erase({*pv, path.destination});
          vertices[path.destination].erase(pv);
        }
        vertices[path.destination].insert(new_vertex);
        queue.insert({new_vertex, path.destination});
      }
    }
  }
  return GetMinPricePath(graph, vertices);
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
  auto [sum, res] = Dijkstra(graph, t);
  if (res.empty()) {
    std::cout << -1;
    return 0;
  }
  std::reverse(res.begin(), res.end());
  std::cout << sum << "\n" << res.size() << "\n";
  for (auto vertex : res) {
    std::cout << vertex + 1 << " ";
  }
}