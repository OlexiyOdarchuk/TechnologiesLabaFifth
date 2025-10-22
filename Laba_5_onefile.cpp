#include <iostream>
#include <limits>
#include <ostream>

class InputValidator {
private:
  static void error(std::istream &is, std::ostream &os,
                    const std::string &error) {
    is.clear();
    os << error;
  }

public:
  template <typename T>
  static std::istream &get_data(std::istream &is, T &value,
                                std::ostream &os = std::cout) {
    while (true) {
      if (is >> value)
        return is;
      error(is, os, "Invalid value. Please, try again: ");
      is.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
  }
};
template <>
inline std::istream &InputValidator::get_data<char>(std::istream &is,
                                                    char &value,
                                                    std::ostream &os) {
  const std::string valid = "abcdef";
  std::string input;
  while (true) {
    std::getline(is, input);

    if (input.length() != 1) {
      error(is, os, "Please, enter one letter (a-f):");
      continue;
    }

    char letter = input[0];

    for (char c : valid) {
      if (letter == c) {
        value = letter;
        return is;
      }
    }
    error(is, os, "Invalid value. Please, try again: ");
  }
}

template <>
inline std::istream &InputValidator::get_data<bool>(std::istream &is,
                                                    bool &value,
                                                    std::ostream &os) {
  std::string input;
  while (true) {
    std::getline(is, input);

    if (input.length() != 1) {
      error(is, os, "Please, enter one digit (0-1): ");
      continue;
    }

    if (input == "1" || input == "0") {
      value = (input == "1");
      return is;
    }
    error(is, os, "Invalid value. Please, try again: ");
  }
}

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
