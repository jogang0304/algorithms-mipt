#include <algorithm>
#include <iostream>
#include <vector>

inline int64_t AndHelper(int64_t number) { return (number & (number + 1)); }
inline int64_t OrHelper(int64_t number) { return (number | (number + 1)); }

struct Point {
  int64_t x = 0;
  int64_t y = 0;
  int64_t weight = 0;
};

struct Rectangle {
  Point p1;
  Point p2;
  Rectangle(Point pp1, Point pp2)
      : p1{std::min(pp1.x, pp2.x), std::min(pp1.y, pp2.y), 0},
        p2{std::max(pp1.x, pp2.x), std::max(pp1.y, pp2.y), 0} {}
};

struct CoordinateInfo {
  std::vector<int64_t> map_to_shrinked;
  std::vector<int64_t> coordinates;
};

class DoubleFenwick {
 public:
  DoubleFenwick(size_t width, size_t height)
      : sums_(width + 2, std::vector<int64_t>(height + 2, 0)) {}
  void Add(Point point, int64_t value);
  int64_t Get(Rectangle rect);

 private:
  std::vector<std::vector<int64_t>> sums_;
};

void DoubleFenwick::Add(Point point, int64_t value) {
  for (int64_t i = point.x; i < static_cast<int64_t>(sums_.size()) - 1;
       i = OrHelper(i)) {
    for (int64_t j = point.y; j < static_cast<int64_t>(sums_[i].size()) - 1;
         j = OrHelper(j)) {
      sums_[i][j] += value;
    }
  }
}

int64_t DoubleFenwick::Get(Rectangle rect) {
  int64_t prefix22 = 0;
  for (int64_t i = rect.p2.x; i >= 0; i = AndHelper(i) - 1) {
    for (int64_t j = rect.p2.y; j >= 0; j = AndHelper(j) - 1) {
      prefix22 += sums_[i][j];
    }
  }
  int64_t prefix12 = 0;
  for (int64_t i = rect.p1.x - 1; i >= 0; i = AndHelper(i) - 1) {
    for (int64_t j = rect.p2.y; j >= 0; j = AndHelper(j) - 1) {
      prefix12 += sums_[i][j];
    }
  }
  int64_t prefix21 = 0;
  for (int64_t i = rect.p2.x; i >= 0; i = AndHelper(i) - 1) {
    for (int64_t j = rect.p1.y - 1; j >= 0; j = AndHelper(j) - 1) {
      prefix21 += sums_[i][j];
    }
  }
  int64_t prefix11 = 0;
  for (int64_t i = rect.p1.x - 1; i >= 0; i = AndHelper(i) - 1) {
    for (int64_t j = rect.p1.y - 1; j >= 0; j = AndHelper(j) - 1) {
      prefix11 += sums_[i][j];
    }
  }
  return prefix22 - prefix12 - prefix21 + prefix11;
}

CoordinateInfo GetShrinkedX(int64_t max_x, std::vector<Point>& point) {
  int64_t amount_of_points = static_cast<int64_t>(point.size());
  std::vector<int64_t> map_x_to_shrinked_x(max_x + 2);
  std::vector<int64_t> xs(amount_of_points + 1);
  std::sort(point.begin(), point.end(),
            [](const Point& p1, const Point& p2) { return p1.x < p2.x; });
  for (int64_t i = 0; i < amount_of_points; ++i) {
    map_x_to_shrinked_x[point[i].x] = i;
    xs[i] = point[i].x;
  }
  xs[amount_of_points] = max_x + 1;
  map_x_to_shrinked_x[max_x + 1] = amount_of_points;
  return {map_x_to_shrinked_x, xs};
}

CoordinateInfo GetShrinkedY(int64_t max_y, std::vector<Point>& point) {
  int64_t amount_of_points = static_cast<int64_t>(point.size());
  std::vector<int64_t> map_y_to_shrinked_y(max_y + 2);
  std::vector<int64_t> ys(amount_of_points + 1);
  std::sort(point.begin(), point.end(),
            [](const Point& p1, const Point& p2) { return p1.y < p2.y; });
  for (int64_t i = 0; i < amount_of_points; ++i) {
    map_y_to_shrinked_y[point[i].y] = i;
    ys[i] = point[i].y;
  }
  ys[amount_of_points] = max_y + 1;
  map_y_to_shrinked_y[max_y + 1] = amount_of_points;
  return {map_y_to_shrinked_y, ys};
}

Point ShrinkPoint(Point& point64_t, CoordinateInfo& x_info,
                  CoordinateInfo& y_info) {
  return {x_info.map_to_shrinked[point64_t.x],
          y_info.map_to_shrinked[point64_t.y], point64_t.weight};
}

std::pair<std::vector<int64_t>::iterator, std::vector<int64_t>::iterator>
GetSearchPair(CoordinateInfo& x_info, CoordinateInfo& y_info, int64_t rx,
              int64_t ry) {
  auto search_x = std::lower_bound(x_info.coordinates.begin(),
                                   x_info.coordinates.end(), rx);
  auto search_y = std::lower_bound(y_info.coordinates.begin(),
                                   y_info.coordinates.end(), ry);
  if (search_x == x_info.coordinates.end() ||
      (*search_x != rx && search_x != x_info.coordinates.begin())) {
    search_x--;
  }
  if (search_y == y_info.coordinates.end() ||
      (*search_y != ry && search_y != y_info.coordinates.begin())) {
    search_y--;
  }
  return {search_x, search_y};
}

int64_t GetAnswer(CoordinateInfo& x_info, CoordinateInfo& y_info, int64_t rx,
                  int64_t ry, DoubleFenwick& tree) {
  auto search = GetSearchPair(x_info, y_info, rx, ry);

  if (*search.first > rx || *search.second > ry) {
    return 0;
  }

  int64_t answer = tree.Get({{0, 0, 0},
                             {x_info.map_to_shrinked[*search.first],
                              y_info.map_to_shrinked[*search.second], 0}});
  return answer;
}

int main() {
  int64_t amount_of_points;
  std::cin >> amount_of_points;
  std::vector<Point> initial_points(amount_of_points);
  int64_t max_x = 0;
  int64_t max_y = 0;
  for (int64_t i = 0; i < amount_of_points; ++i) {
    int64_t x;
    int64_t y;
    int64_t weight;
    std::cin >> x >> y >> weight;
    initial_points[i] = {x, y, weight};
    max_x = std::max(max_x, x);
    max_y = std::max(max_y, y);
  }
  std::vector<Point> point = initial_points;

  auto x_info = GetShrinkedX(max_x, point);
  auto y_info = GetShrinkedY(max_y, point);

  DoubleFenwick tree(amount_of_points, amount_of_points);
  std::vector<Point> initial_shrinked(amount_of_points);
  for (int64_t i = 0; i < amount_of_points; ++i) {
    initial_shrinked[i] = ShrinkPoint(initial_points[i], x_info, y_info);
    tree.Add(initial_shrinked[i], initial_points[i].weight);
  }
  tree.Add({amount_of_points + 1, amount_of_points + 1, 0}, 0);

  int64_t amount_of_questions;
  std::cin >> amount_of_questions;
  for (int64_t step = 0; step < amount_of_questions; ++step) {
    std::string command;
    std::cin >> command;
    if (command == "get") {
      int64_t rx;
      int64_t ry;
      std::cin >> rx >> ry;
      auto answer = GetAnswer(x_info, y_info, rx, ry, tree);
      std::cout << answer << "\n";
    } else if (command == "change") {
      int64_t index;
      int64_t new_weight;
      std::cin >> index >> new_weight;
      int64_t delta = new_weight - initial_points[index - 1].weight;
      tree.Add(initial_shrinked[index - 1], delta);
      initial_points[index - 1].weight = new_weight;
      initial_shrinked[index - 1].weight = new_weight;
    }
  }
}