#include <iostream>
#include <stack>
#include <vector>

const int kInf = __INT32_MAX__;

int FindIndexOfFirstInf(const std::vector<int>& numbers) {
  int left = 0;
  int right = static_cast<int>(numbers.size()) - 1;
  int middle = 0;
  while (right - left > 1) {
    middle = (left + right) / 2;
    if (numbers[middle] == kInf) {
      right = middle;
    } else {
      left = middle;
    }
  }
  return right;
}

int GetLengthOfMaxSubsequence(
    const std::vector<int>& elements,
    const std::vector<int>& least_number_index_in_subsequence_of_length_i,
    int current_index) {
  int left = 0;
  int right = static_cast<int>(elements.size()) + 1;
  int middle = 0;
  while (right - left > 1) {
    middle = (left + right) / 2;
    if (least_number_index_in_subsequence_of_length_i[middle] != kInf &&
        elements[least_number_index_in_subsequence_of_length_i[middle]] >=
            elements[current_index]) {
      left = middle;
    } else {
      right = middle;
    }
  }
  return left;
}

int PrintAndReturnAnswerLength(
    const std::vector<int>& least_number_index_in_subsequence_of_length_i) {
  int answer =
      FindIndexOfFirstInf(least_number_index_in_subsequence_of_length_i) - 1;
  std::cout << answer << "\n";
  return answer;
}

void PrintAnswerIndexes(
    const std::vector<int>& least_number_index_in_subsequence_of_length_i,
    const std::vector<int>& prev_index, int answer) {
  std::stack<int> answer_indexes_stack;
  answer_indexes_stack.push(
      least_number_index_in_subsequence_of_length_i[answer] + 1);
  answer = prev_index[least_number_index_in_subsequence_of_length_i[answer]];
  while (answer != kInf) {
    answer_indexes_stack.push(answer + 1);
    answer = prev_index[answer];
  }
  while (!answer_indexes_stack.empty()) {
    std::cout << answer_indexes_stack.top() << " ";
    answer_indexes_stack.pop();
  }
}

void PrintAnswer(
    const std::vector<int>& least_number_index_in_subsequence_of_length_i,
    const std::vector<int>& prev_index) {
  PrintAnswerIndexes(least_number_index_in_subsequence_of_length_i, prev_index,
                     PrintAndReturnAnswerLength(
                         least_number_index_in_subsequence_of_length_i));
}

int main() {
  int amount_of_elements = 0;
  std::cin >> amount_of_elements;
  std::vector<int> elements(amount_of_elements);
  for (auto& element : elements) {
    std::cin >> element;
  }
  std::vector<int> least_number_index_in_subsequence_of_length_i(
      amount_of_elements + 1, kInf);
  std::vector<int> prev_index(amount_of_elements, kInf);
  for (int i = 0; i < static_cast<int>(elements.size()); ++i) {
    int length_of_max_subsequence = GetLengthOfMaxSubsequence(
        elements, least_number_index_in_subsequence_of_length_i, i);
    if (least_number_index_in_subsequence_of_length_i
                [length_of_max_subsequence + 1] != kInf &&
        elements[least_number_index_in_subsequence_of_length_i
                     [length_of_max_subsequence + 1]] >= elements[i]) {
      continue;
    }
    least_number_index_in_subsequence_of_length_i[length_of_max_subsequence +
                                                  1] = i;
    prev_index[i] = least_number_index_in_subsequence_of_length_i
        [length_of_max_subsequence];
  }
  PrintAnswer(least_number_index_in_subsequence_of_length_i, prev_index);
}