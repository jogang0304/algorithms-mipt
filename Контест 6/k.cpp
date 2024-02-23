#include <iostream>
#include <stack>
#include <vector>

const int kInf = 1e9;

struct Cell {
  int max_price = -kInf;
  int taken_position = -kInf;
};

int FindMaxPriceResources(const std::vector<std::vector<Cell>>& dp) {
  int answer = 0;
  for (int i = 0; i < static_cast<int>(dp[dp.size() - 1].size()); ++i) {
    if (dp[dp.size() - 1][i].max_price > answer) {
      answer = i;
    }
  }
  return answer;
}

void PrintAnswer(const std::vector<std::vector<Cell>>& dp,
                 const std::vector<int>& task_resources) {
  int max_price_resources = FindMaxPriceResources(dp);
  int element_position = static_cast<int>(dp.size()) - 1;
  std::stack<int> answer_stack;
  while (element_position >= 1) {
    int taken_position =
        dp[element_position][max_price_resources].taken_position;
    if (taken_position != -kInf) {
      answer_stack.push(taken_position);
      max_price_resources -= task_resources[taken_position - 1];
    }
    --element_position;
  }
  while (!answer_stack.empty()) {
    std::cout << answer_stack.top() << "\n";
    answer_stack.pop();
  }
}

bool CanTakeElement(const std::vector<std::vector<Cell>>& dp,
                    const std::vector<int>& task_resources,
                    const std::vector<int>& task_price, int element_position,
                    int resources) {
  if (resources >= task_resources[element_position - 1]) {
    int possible_price = dp[element_position - 1]
                           [resources - task_resources[element_position - 1]]
                               .max_price +
                         task_price[element_position - 1];
    if (possible_price > dp[element_position - 1][resources].max_price) {
      return true;
    }
  }
  return false;
}

Cell MaxCell(const std::vector<std::vector<Cell>>& dp,
             const std::vector<int>& task_resources,
             const std::vector<int>& task_price, int element_position,
             int resources) {
  Cell answer;
  answer.max_price = dp[element_position - 1][resources].max_price;
  if (CanTakeElement(dp, task_resources, task_price, element_position,
                     resources)) {
    answer.max_price = dp[element_position - 1]
                         [resources - task_resources[element_position - 1]]
                             .max_price +
                       task_price[element_position - 1];
    answer.taken_position = element_position;
  }
  return answer;
}

struct Input {
  int amount_of_elements = 0;
  int max_resources = 0;
  std::vector<int> task_resources;
  std::vector<int> task_price;
};

Input GetInput() {
  Input answer;
  std::cin >> answer.amount_of_elements >> answer.max_resources;
  answer.task_resources.resize(answer.amount_of_elements);
  for (int i = 0; i < static_cast<int>(answer.task_resources.size()); ++i) {
    std::cin >> answer.task_resources[i];
  }
  answer.task_price.resize(answer.amount_of_elements);
  for (int i = 0; i < static_cast<int>(answer.task_price.size()); ++i) {
    std::cin >> answer.task_price[i];
  }
  return answer;
}

int main() {
  Input input = GetInput();
  std::vector<std::vector<Cell>> dp(input.amount_of_elements + 1,
                                    std::vector<Cell>(input.max_resources + 1));
  for (int i = 0; i < static_cast<int>(dp[0].size()); ++i) {
    dp[0][i].max_price = 0;
  }
  for (int position = 1; position < static_cast<int>(dp.size()); ++position) {
    for (int resources = 0; resources < static_cast<int>(dp[position].size()); ++resources) {
      dp[position][resources] = MaxCell(dp, input.task_resources, input.task_price, position, resources);
    }
  }

  PrintAnswer(dp, input.task_resources);
}