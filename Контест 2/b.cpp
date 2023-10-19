#include <iostream>
#include <stack>
#include <vector>

int main() {
  long long amount_of_heights;
  std::cin >> amount_of_heights;
  std::vector<long long> heights(amount_of_heights + 2);
  heights[0] = -1;
  heights[amount_of_heights + 1] = 0;
  for (long long i = 1; i < amount_of_heights + 1; ++i) {
    std::cin >> heights[i];
  }
  std::stack<long long> stack;
  stack.push(0);
  long long max_area = 0;
  long long last_deleted_index = -1;
  for (long long i = 1; i < amount_of_heights + 2; ++i) {
    if (heights[i] >= heights[stack.top()]) {
      stack.push(i);
      last_deleted_index = -1;
    } else {
      while (stack.size() > 1 && heights[i] < heights[stack.top()]) {
        long long cur_rectangle = heights[stack.top()] * (i - stack.top());
        max_area = std::max(max_area, cur_rectangle);
        last_deleted_index = stack.top();
        stack.pop();
      }
      heights[last_deleted_index] =
          std::min(heights[last_deleted_index], heights[i]);
      stack.push(last_deleted_index);
    }
  }
  std::cout << max_area;
}