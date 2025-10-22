#include <iostream>
#include <limits>
#include <string>

bool getBoolValue();
char getCharValue();
float getFloatValue();
short getShortValue();

void error() {
  std::cin.clear();
  std::cout << "Invalid value. Please, try again: ";
}

int main() {
  bool boolValue = getBoolValue();
  std::cout << "Successfully received boolean value : "
            << (boolValue ? "true" : "false") << std::endl;
  char charValue = getCharValue();
  std::cout << "Successfully received char value : " << charValue << std::endl;
  float floatValue = getFloatValue();
  std::cout << "Successfully received float value : " << floatValue
            << std::endl;
  short shortValue = getShortValue();
  std::cout << "Successfully received short value : " << shortValue
            << std::endl;
  return 0;
}

bool getBoolValue() {
  std::cout << "Enter the bool value (0 or 1): ";
  std::string value;
  while (true) {
    std::getline(std::cin, value);
    if (value == "1" || value == "0")
      return (value == "1" ? true : false);
    error();

    continue;
  }
}

char getCharValue() {
  const std::string valid = "abcdef";
  std::cout << "Enter a letter from the Latin alphabet (from a to f): ";
  std::string value;
  while (true) {
    std::getline(std::cin, value);

    if (value.length() != 1) {
      std::cout << "Please enter only one letter: ";
      continue;
    }

    char letter = value[0];

    for (char c : valid) {
      if (letter == c) {
        return letter;
      }
    }
    error();
  }
}

float getFloatValue() {
  std::cout << "Enter a float value: ";
  float value;
  while (true) {
    if (std::cin >> value)
      return value;

    error();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  }
}

short getShortValue() {
  std::cout << "Enter a short value: ";
  short value;
  while (true) {
    if (std::cin >> value)
      return value;
    error();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  }
}