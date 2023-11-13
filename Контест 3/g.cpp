#include <algorithm>
#include <iostream>
#include <vector>

inline int AndHelper(int number) { return (number & (number + 1)); }
inline int OrHelper(int number) { return (number | (number + 1)); }

struct Point {
  int x;
  int y;
};

struct Rectangle {
  Point p1;
  Point p2;
  Rectangle(Point pp1, Point pp2)
      : p1{std::min(pp1.x, pp2.x), std::min(pp1.y, pp2.y)},
        p2{std::max(pp1.x, pp2.x), std::max(pp1.y, pp2.y)} {}
};

class DoubleFenwick {
 public:
  DoubleFenwick(size_t width, size_t height)
      : sums_(width + 1, std::vector<int>(height + 1, 0)) {}
  void Add(Point point, int value);
  int Get(Rectangle rect);

 private:
  std::vector<std::vector<int>> sums_;
};

void DoubleFenwick::Add(Point point, int value) {
  for (int i = point.x; i < static_cast<int>(sums_.size()); i = OrHelper(i)) {
    for (int j = point.y; j < static_cast<int>(sums_[i].size());
         j = OrHelper(j)) {
      sums_[i][j] += value;
    }
  }
}

int DoubleFenwick::Get(Rectangle rect) {
  int prefix22 = 0;
  for (int i = rect.p2.x; i >= 0; i = AndHelper(i) - 1) {
    for (int j = rect.p2.y; j >= 0; j = AndHelper(j) - 1) {
      prefix22 += sums_[i][j];
    }
  }
  int prefix12 = 0;
  for (int i = rect.p1.x - 1; i >= 0; i = AndHelper(i) - 1) {
    for (int j = rect.p2.y; j >= 0; j = AndHelper(j) - 1) {
      prefix12 += sums_[i][j];
    }
  }
  int prefix21 = 0;
  for (int i = rect.p2.x; i >= 0; i = AndHelper(i) - 1) {
    for (int j = rect.p1.y - 1; j >= 0; j = AndHelper(j) - 1) {
      prefix21 += sums_[i][j];
    }
  }
  int prefix11 = 0;
  for (int i = rect.p1.x - 1; i >= 0; i = AndHelper(i) - 1) {
    for (int j = rect.p1.y - 1; j >= 0; j = AndHelper(j) - 1) {
      prefix11 += sums_[i][j];
    }
  }
  return prefix22 - prefix12 - prefix21 + prefix11;
}

int main() {
  int board_size = 0;
  int amount_of_questions = 0;
  std::cin >> board_size >> amount_of_questions;
  DoubleFenwick tree(board_size, board_size);
  for (int step = 0; step < amount_of_questions; ++step) {
    std::string command;
    std::cin >> command;
    if (command == "ADD") {
      int x;
      int y;
      std::cin >> x >> y;
      --x;
      --y;
      tree.Add(Point{x, y}, 1);
    } else {
      int x1;
      int y1;
      int x2;
      int y2;
      std::cin >> x1 >> y1 >> x2 >> y2;
      --x1;
      --y1;
      --x2;
      --y2;
      std::cout << tree.Get(Rectangle{Point{x1, y1}, Point{x2, y2}})
                << std::endl;
    }
  }
}