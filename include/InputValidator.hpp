#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>
#include <type_traits>
#pragma once

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
                                std::ostream &os = std::cout)
    requires std::is_arithmetic_v<T> && (!std::is_same_v<T, char>) &&
             (!std::is_same_v<T, bool>)
  {
    std::string input;
    while (true) {
      if (!std::getline(is, input))
        return is;

      if (input.empty()) {
        error(is, os, "Empty input. Please, try again: ");
        continue;
      }

      try {
        size_t pos;

        if constexpr (std::is_integral_v<T>) {
          if constexpr (std::is_unsigned_v<T>) {
            unsigned long long temp = std::stoull(input, &pos);

            if (pos != input.size())
              throw std::invalid_argument{"Extra characters"};

            if (temp > std::numeric_limits<T>::max())
              throw std::out_of_range{"Out of range"};

            value = static_cast<T>(temp);
          } else {
            long long temp = std::stoll(input, &pos);

            if (pos != input.size())
              throw std::invalid_argument{"Extra characters"};

            if (temp < std::numeric_limits<T>::min() ||
                temp > std::numeric_limits<T>::max())
              throw std::out_of_range{"Out of range"};

            value = static_cast<T>(temp);
          }
        } else if constexpr (std::is_floating_point_v<T>) {
          long double temp = std::stold(input, &pos);

          if (pos != input.size())
            throw std::invalid_argument{"Extra characters"};

          if (temp < std::numeric_limits<T>::lowest() ||
              temp > std::numeric_limits<T>::max())
            throw std::out_of_range{"Out of range"};

          value = static_cast<T>(temp);
        }

        return is;
      } catch (const std::invalid_argument &) {
        error(is, os, "Invalid value. Please, try again: ");
      } catch (const std::out_of_range &) {
        error(is, os, "Value out of range. Please, try again: ");
      }
    }
  }

  static std::istream &get_data(std::istream &is, bool &value,
                                std::ostream &os = std::cout) {
    std::string input;
    while (true) {
      if (!std::getline(is, input))
        return is;

      if (input.empty()) {
        error(is, os, "Empty input. Please, try again: ");
        continue;
      }

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

  static std::istream &get_data(std::istream &is, char &value,
                                std::ostream &os = std::cout) {
    const std::string valid = "abcdef";
    std::string input;

    while (true) {
      if (!std::getline(is, input))
        return is;

      if (input.empty()) {
        error(is, os, "Empty input. Please, try again: ");
        continue;
      }

      if (input.length() != 1) {
        error(is, os, "Please, enter one letter (a-f): ");
        continue;
      }

      if (valid.find(input[0]) != std::string::npos) {
        value = input[0];
        return is;
      }

      error(is, os, "Invalid value. Please, try again: ");
    }
  }
};