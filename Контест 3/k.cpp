#include <algorithm>
#include <iostream>
#include <set>
#include <vector>

const int64_t kInf = 2'000'000'000'000'000'000;

struct Node {
  int64_t minimum = 0;
  int64_t count = 0;
  Node(int64_t minimum, int64_t count) : minimum(minimum), count(count) {}
};

struct AddInfo {
  int64_t left;
  int64_t right;
  int64_t value;
  AddInfo(int64_t left, int64_t right, int64_t value)
      : left(left), right(right), value(value) {}
};

struct GetInfo {
  int64_t left;
  int64_t right;
  GetInfo(int64_t left, int64_t right) : left(left), right(right) {}
};

class MinTreeWithCount {
 public:
  MinTreeWithCount(const std::vector<int64_t>& coordinates)
      : array_size_(coordinates.size()),
        tree_(array_size_ * 4, {0, 0}),
        change_(array_size_ * 4, 0),
        coordinates_(coordinates) {
    BuildCount(0, 0, array_size_ - 1);
  }

  void AddRange(const AddInfo& info, int64_t tree_index = 0,
                int64_t tree_left = 0, int64_t tree_right = 0,
                int64_t saved_change = 0) {
    if (tree_right == 0) {
      tree_right = array_size_ - 1;
    }
    if (tree_left >= info.right || tree_right <= info.left) {
      return;
    }
    if (tree_left >= info.left && tree_right <= info.right) {
      change_[tree_index] += info.value;
      tree_[tree_index].minimum += info.value;
      return;
    }
    int64_t tree_middle = (tree_left + tree_right) / 2;
    AddRange(info, tree_index * 2 + 1, tree_left, tree_middle,
             saved_change + change_[tree_index]);
    AddRange(info, tree_index * 2 + 2, tree_middle, tree_right,
             saved_change + change_[tree_index]);
    auto left_child =
        GetMin({tree_left, tree_middle}, tree_index * 2 + 1, tree_left,
               tree_middle, saved_change + change_[tree_index]);
    auto right_child =
        GetMin({tree_middle, tree_right}, tree_index * 2 + 2, tree_middle,
               tree_right, saved_change + change_[tree_index]);
    if (left_child.minimum < right_child.minimum) {
      tree_[tree_index] = left_child;
    } else if (right_child.minimum < left_child.minimum) {
      tree_[tree_index] = right_child;
    } else {
      tree_[tree_index].minimum = left_child.minimum;
      tree_[tree_index].count = left_child.count + right_child.count;
    }
  }

  Node GetMin(const GetInfo& info, int64_t tree_index = 0,
              int64_t tree_left = 0, int64_t tree_right = 0,
              int64_t saved_change = 0) {
    if (tree_right == 0) {
      tree_right = array_size_ - 1;
    }
    if (tree_left >= info.right || tree_right <= info.left) {
      return {kInf, 0};
    }
    if (tree_left >= info.left && tree_right <= info.right) {
      return {static_cast<int64_t>(tree_[tree_index].minimum + saved_change +
                                   change_[tree_index]),
              tree_[tree_index].count};
    }
    int64_t middle = (tree_left + tree_right) / 2;
    Node left_node = GetMin(info, tree_index * 2 + 1, tree_left, middle,
                            saved_change + change_[tree_index]);
    Node right_node = GetMin(info, tree_index * 2 + 2, middle, tree_right,
                             saved_change + change_[tree_index]);
    if (left_node.minimum < right_node.minimum) {
      return left_node;
    }
    if (left_node.minimum > right_node.minimum) {
      return right_node;
    }
    return {left_node.minimum,
            static_cast<int64_t>(left_node.count + right_node.count)};
  }

 private:
  int array_size_;
  std::vector<Node> tree_;
  std::vector<int64_t> change_;
  std::vector<int64_t> coordinates_;

  void BuildCount(int64_t tree_index, int64_t tree_left, int64_t tree_right) {
    if (tree_left >= tree_right) {
      return;
    }
    if (tree_left + 1 == tree_right) {
      tree_[tree_index].count =
          coordinates_[tree_right] - coordinates_[tree_left];
      return;
    }
    int64_t middle = (tree_left + tree_right) / 2;
    BuildCount(tree_index * 2 + 1, tree_left, middle);
    BuildCount(tree_index * 2 + 2, middle, tree_right);
    tree_[tree_index].count =
        (coordinates_[tree_right] - coordinates_[tree_left]);
  }
};

struct Rectangle {
  int64_t x1;
  int64_t y1;
  int64_t x2;
  int64_t y2;
  Rectangle(int64_t x1, int64_t y1, int64_t x2, int64_t y2)
      : x1(x1), y1(y1), x2(x2), y2(y2) {}
};

struct Event {
  int64_t x;
  int64_t y1;
  int64_t y2;
  int64_t type;  // 1 - start, -1 - end
  Event(int64_t x_coordinate, int64_t y1, int64_t y2, int64_t type)
      : x(x_coordinate), y1(y1), y2(y2), type(type) {}
};

int64_t GetShrinkedIndex(int64_t coordinate,
                         const std::vector<int64_t>& coordinates) {
  auto it =
      std::lower_bound(coordinates.begin(), coordinates.end(), coordinate);
  if (it != coordinates.end() && *it == coordinate) {
    return it - coordinates.begin();
  }
  if (it != coordinates.begin()) {
    --it;
    return it - coordinates.begin();
  }
  return 0;
}

std::vector<Event> GetEvents(std::vector<Rectangle>& rectangles) {
  std::vector<Event> events;
  for (auto& rect : rectangles) {
    events.push_back({rect.x1, rect.y1, rect.y2, 1});
    events.push_back({rect.x2, rect.y1, rect.y2, -1});
  }
  std::sort(events.begin(), events.end(),
            [](const Event& left, const Event& right) {
              return (left.x < right.x) ||
                     (left.x == right.x && left.type > right.type);
            });
  return events;
}

int main() {
  int amount_of_rectangles;
  std::cin >> amount_of_rectangles;
  if (amount_of_rectangles == 0) {
    std::cout << 0 << "\n";
    return 0;
  }
  std::vector<Rectangle> rectangles;
  std::set<int64_t> ys_set;
  std::vector<int64_t> ys(0);
  for (int i = 0; i < amount_of_rectangles; ++i) {
    int64_t x1;
    int64_t y1;
    int64_t x2;
    int64_t y2;
    std::cin >> x1 >> y1 >> x2 >> y2;
    if (x1 != x2 && y1 != y2) {
      ys_set.insert(y1);
      ys_set.insert(y2);
      rectangles.push_back({x1, y1, x2, y2});
    }
  }
  if (rectangles.empty() || ys_set.empty()) {
    std::cout << 0 << "\n";
    return 0;
  }
  for (const auto& y : ys_set) {
    ys.push_back(y);
  }
  ys_set.clear();
  int64_t max_y = ys[ys.size() - 1];
  std::sort(ys.begin(), ys.end());
  std::vector<Event> events = GetEvents(rectangles);

  MinTreeWithCount tree(ys);
  int64_t answer = 0;
  for (size_t i = 0; i < events.size(); ++i) {
    Event cur_event = events[i];
    if (i > 0) {
      Event prev_event = events[i - 1];
      Node min = tree.GetMin({0, GetShrinkedIndex(max_y, ys)});
      int64_t count_not_zero = max_y - ys[0];
      if (min.minimum == 0) {
        count_not_zero -= min.count;
      }
      answer += (cur_event.x - prev_event.x) * (count_not_zero);
    }

    tree.AddRange({GetShrinkedIndex(cur_event.y1, ys),
                   GetShrinkedIndex(cur_event.y2, ys), cur_event.type});
  }

  std::cout << answer << "\n";
}
