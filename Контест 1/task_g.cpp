#include <limits.h>

#include <iostream>
#include <vector>

// byte_from_end начинается с 0
unsigned char GetNByte(unsigned long long number, size_t byte_from_end) {
  // CHAR_BIT = 8 - количесво бит в байте
  // нужный байт поддвигается к концу числа, чтобы его можно было легко взять
  auto shifted_number = number >> CHAR_BIT * byte_from_end;
  // UCHAR_MAX = 0xFF - один байт
  auto byte = static_cast<unsigned char>(shifted_number & UCHAR_MAX);
  return byte;
}

void LSDSort(std::vector<unsigned long long>& values) {
  for (size_t byte_from_end = 0; byte_from_end < sizeof(unsigned long long);
       ++byte_from_end) {
    std::vector<std::vector<unsigned long long>> groups(
        UCHAR_MAX + 1, std::vector<unsigned long long>(0));
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
  std::vector<unsigned long long> numbers(amount_of_elements);
  for (auto& number : numbers) {
    std::cin >> number;
  }
  LSDSort(numbers);
  for (auto& number : numbers) {
    std::cout << number << "\n";
  }
}