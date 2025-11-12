#pragma once
#include <exception>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>
#include <type_traits>

template <typename T>
concept CharType = std::is_same_v<T, unsigned char> ||
                   std::is_same_v<T, signed char> || std::is_same_v<T, char>;
template <typename T>
concept ArithmeticInputType =
    std::is_arithmetic_v<T> && !CharType<T> && !std::is_same_v<T, bool>;

namespace InputValidator {

inline void error(std::istream &is, std::ostream &os,
                  const std::string &error) {
  is.clear();
  os << error;
}

template <ArithmeticInputType T>
inline std::istream &get_data(std::istream &is, T &value,
                              const std::string &prompt = "Enter a number: ",
                              const T min = std::numeric_limits<T>::lowest(),
                              const T max = std::numeric_limits<T>::max(),
                              std::ostream &os = std::cout) {
  os << prompt;
  std::string input;
  while (true) {
    if (!std::getline(is, input))
      return is;

    if (input.empty()) {
      error(is, os, "Empty input. Please, try again: ");
      continue;
    }

    try {
      size_t pos{};
      long double temp{};

      if constexpr (std::is_integral_v<T>) {
        if constexpr (std::is_unsigned_v<T>) {
          unsigned long long val = std::stoull(input, &pos);
          if (pos != input.size())
            throw std::invalid_argument{"Unexpected characters after number"};
          if (val < static_cast<unsigned long long>(min) ||
              val > static_cast<unsigned long long>(max))
            throw std::out_of_range{"Out of range"};
          value = static_cast<T>(val);
        } else {
          long long val = std::stoll(input, &pos);
          if (pos != input.size())
            throw std::invalid_argument{"Unexpected characters after number"};
          if (val < static_cast<long long>(min) ||
              val > static_cast<long long>(max))
            throw std::out_of_range{"Out of range"};
          value = static_cast<T>(val);
        }
      } else {
        temp = std::stold(input, &pos);
        if (pos != input.size())
          throw std::invalid_argument{"Unexpected characters after number"};
        if (temp < static_cast<long double>(min) ||
            temp > static_cast<long double>(max))

          throw std::out_of_range{"Out of range"};
        value = static_cast<T>(temp);
      }

      return is;
    } catch (const std::out_of_range &) {
      error(is, os, "Value out of range. Please, try again: ");
    } catch (const std::exception &) {
      error(is, os, "Invalid value. Please, try again: ");
    }
  }
}
inline std::istream &get_data(std::istream &is, std::string &value,
                              const std::string &prompt = "Enter a string: ",
                              std::ostream &os = std::cout) {
  os << prompt;
  std::string input;
  while (true) {
    if (!std::getline(is, input))
      return is;

    if (input.empty()) {
      error(is, os, "Empty input. Please, try again: ");
      continue;
    }
    value = input;
    return is;
  }
}
inline std::istream &
get_data(std::istream &is, bool &value,
         const std::string &prompt = "Enter 0 for false or 1 for true: ",
         std::ostream &os = std::cout) {
  os << prompt;
  std::string input;
  while (true) {
    if (!std::getline(is, input))
      return is;

    if (input.empty()) {
      error(is, os, "Empty input. Please, try again: ");
      continue;
    }

    if (input.length() != 1) {
      error(is, os, "Enter 0 for false or 1 for true: ");
      continue;
    }

    if (input == "1" || input == "0") {
      value = (input == "1");
      return is;
    }

    error(is, os, "Invalid value. Please, try again: ");
  }
}
template <CharType T>
inline std::istream &get_data(std::istream &is, T &value,
                              const std::string &prompt = "Enter a char: ",
                              std::ostream &os = std::cout) {
  os << prompt;
  std::string input;

  while (true) {
    if (!std::getline(is, input))
      return is;

    if (input.empty()) {
      error(is, os, "Empty input. Please, try again: ");
      continue;
    }

    if (input.length() != 1) {
      error(is, os, "Please, enter a char: ");
      continue;
    }

    value = static_cast<T>(input[0]);
    return is;
  }
}
}; // namespace InputValidator