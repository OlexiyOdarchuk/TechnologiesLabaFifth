#include "InputValidator.hpp"
#include <iostream>

int main() {
  bool boolValue;
  std::cout << "Enter a boolean value: ";

  InputValidator::get_data(std::cin, boolValue);
  std::cout << "Successfully received boolean value : "
            << (boolValue ? "true" : "false") << std::endl;

  char charValue;
  std::cout << "Enter a char value: ";

  InputValidator::get_data(std::cin, charValue);
  std::cout << "Successfully received char value : " << charValue << std::endl;

  double doubleValue;
  std::cout << "Enter a double value: ";

  InputValidator::get_data(std::cin, doubleValue);
  std::cout << "Successfully received double value : " << doubleValue
            << std::endl;

  short shortValue;
  std::cout << "Enter a short value: ";

  InputValidator::get_data(std::cin, shortValue);
  std::cout << "Successfully received short value : " << shortValue
            << std::endl;
  return 0;
}
