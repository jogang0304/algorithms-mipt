#include <limits.h>

#include <iostream>
#include <vector>

unsigned char GetNByte(u_int64_t number, size_t byte_from_end) {
  auto shifted_number = number >> CHAR_BIT * byte_from_end;
  auto byte = static_cast<unsigned char>(shifted_number & UCHAR_MAX);
  return byte;
}

void LSDSort(std::vector<u_int64_t>& values) {
  for (size_t byte_from_end = 0; byte_from_end < sizeof(u_int64_t);
       ++byte_from_end) {
    std::vector<std::vector<u_int64_t>> groups(UCHAR_MAX + 1,
                                               std::vector<u_int64_t>(0));
    for (auto number : values) {
      auto number_n_char = GetNByte(number, byte_from_end);
      groups[number_n_char].push_back(number);
    }
    size_t values_index = 0;
    for (auto group : groups) {
      for (auto number : group) {
        values[values_index++] = number;
      }
    }
  }
}

int main() {
  int amount_of_elements;
  std::cin >> amount_of_elements;
  std::vector<u_int64_t> numbers(amount_of_elements);
  for (auto& number : numbers) {
    std::cin >> number;
  }
  LSDSort(numbers);
  for (auto& number : numbers) {
    std::cout << number << "\n";
  }
}