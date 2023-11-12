#include <iostream>
#include <stack>
#include <vector>

int64_t CountMaxArea(std::vector<int64_t>& heights) {
  std::stack<int64_t> stack;
  stack.push(0);
  int64_t max_area = 0;
  int64_t last_deleted_index = -1;
  for (int64_t i = 1; i < heights.size(); ++i) {
    if (heights[i] >= heights[stack.top()]) {
      stack.push(i);
      last_deleted_index = -1;
    } else {
      while (stack.size() > 1 && heights[i] < heights[stack.top()]) {
        int64_t cur_rectangle = heights[stack.top()] * (i - stack.top());
        max_area = std::max(max_area, cur_rectangle);
        last_deleted_index = stack.top();
        stack.pop();
      }
      heights[last_deleted_index] =
          std::min(heights[last_deleted_index], heights[i]);
      stack.push(last_deleted_index);
    }
  }
  return max_area;
}

int main() {
  int64_t amount_of_heights;
  std::cin >> amount_of_heights;
  std::vector<int64_t> heights(amount_of_heights + 2);
  heights[0] = -1;
  heights[amount_of_heights + 1] = 0;
  for (int64_t i = 1; i < amount_of_heights + 1; ++i) {
    std::cin >> heights[i];
  }
  int64_t max_area = CountMaxArea(heights);
  std::cout << max_area;
}