#include <algorithm>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

struct Input {
  int matrix_size;
  std::vector<std::vector<bool>> communication_matrix;
};

const Input* ReadInput() {
  int matrix_size;
  std::cin >> matrix_size;
  Input* answer = new Input;
  answer->matrix_size = matrix_size;
  answer->communication_matrix.resize(matrix_size,
                                      std::vector<bool>(matrix_size, false));
  for (int i = 0; i < matrix_size; ++i) {
    std::string one_line;
    std::cin >> one_line;
    for (int j = 0; j < static_cast<int>(one_line.size()); ++j) {
      if (one_line[j] == 'Y') {
        answer->communication_matrix[i][j] = true;
      }
    }
  }
  return answer;
}

int CalculateWithComputersFromN(
    const std::vector<std::vector<bool>>& communication_matrix,
    std::vector<bool>& false_rows, std::vector<bool>& false_columns,
    int starting_computer_index) {
  if (starting_computer_index ==
      static_cast<int>(communication_matrix.size()) - 1) {
    return 0;
  }
  bool prev_row = false_rows[starting_computer_index];
  bool prev_column = false_columns[starting_computer_index];
  false_rows[starting_computer_index] = true;
  false_columns[starting_computer_index] = true;
  int answer =
      CalculateWithComputersFromN(communication_matrix, false_rows,
                                  false_columns, starting_computer_index + 1);
  false_rows[starting_computer_index] = prev_row;
  false_columns[starting_computer_index] = prev_column;
  if (!false_rows[starting_computer_index]) {
    for (int i = starting_computer_index;
         i < static_cast<int>(communication_matrix.size()); ++i) {
      if (!false_columns[i] &&
          communication_matrix[starting_computer_index][i]) {
        bool prev_i_row = false_rows[i];
        bool prev_i_column = false_columns[i];
        bool prev_s_row = false_rows[starting_computer_index];
        bool prev_s_column = false_columns[starting_computer_index];
        false_rows[i] = true;
        false_columns[i] = true;
        false_rows[starting_computer_index] = true;
        false_columns[starting_computer_index] = true;
        int possible_answer =
            2 + CalculateWithComputersFromN(communication_matrix, false_rows,
                                            false_columns,
                                            starting_computer_index + 1);
        false_rows[i] = prev_i_row;
        false_columns[i] = prev_i_column;
        false_rows[starting_computer_index] = prev_s_row;
        false_columns[starting_computer_index] = prev_s_column;
        answer = std::max(answer, possible_answer);
      }
    }
  }
  return answer;
}

int main() {
  const Input* input = ReadInput();
  std::vector<bool> false_rows(input->matrix_size, false);
  std::vector<bool> false_columns(input->matrix_size, false);
  std::cout << "started" << std::endl;
  int answer = CalculateWithComputersFromN(input->communication_matrix,
                                           false_rows, false_columns, 0);
  std::cout << answer;
  delete input;
}