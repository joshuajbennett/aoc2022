#include <atomic>
#include <cmath>
#include <deque>
#include <functional>
#include <iostream>
#include <math.h>
#include <mutex>
#include <optional>
#include <queue>
#include <sstream>
#include <string>
#include <thread>
#include <unordered_map>
#include <unordered_set>
#include <vector>

long long snafuToDecimal(const std::string& input) {
  long long num = 0;
  for (int i = 0; i < input.size(); ++i) {
    char current = input[input.size() - 1 - i];
    long long val = pow(5, i);
    switch (current) {
    case '2':
      num += 2 * val;
      break;
    case '1':
      num += 1 * val;
      break;
    case '-':
      num -= 1 * val;
      break;
    case '=':
      num -= 2 * val;
      break;
    }
  }
  return num;
}

std::string decimalToSnafu(long long number) {
  std::string snafu{""};
  if (number == 0) {
    return "0";
  }
  int digit = 0;
  while (number != 0) {
    long long val = pow(5, digit);
    long long nextVal = pow(5, digit + 1);
    long long numToRemove = 0;
    auto remainder = (std::abs(number) % nextVal) / val;
    if (remainder < 3) {
      // snafu.insert(0, std::to_string(remainder));
      if (number > 0) {
        numToRemove = remainder;
      } else {
        numToRemove = -remainder;
      }
    } else if (remainder == 3) {
      // snafu.insert(0, "=");
      if (number > 0) {
        numToRemove = -2;
      } else {
        numToRemove = 2;
      }
    } else if (remainder == 4) {
      // snafu.insert(0, "-");
      if (number > 0) {
        numToRemove = -1;
      } else {
        numToRemove = 1;
      }
    } else {
      // std::cout << "Remainder: " << remainder << std::endl;
    }
    if (numToRemove >= 0 && numToRemove < 3) {
      snafu.insert(0, std::to_string(numToRemove));
    } else if (numToRemove == -1) {
      snafu.insert(0, "-");
    } else if (numToRemove == -2) {
      snafu.insert(0, "=");
    } else {
      // std::cout << "numToRemove: " << numToRemove << std::endl;
      throw std::runtime_error("Failure to remove.");
    }
    // std::cout << "Num to remove: " << numToRemove << " with val: " << val << std::endl;
    // std::cout << "Number: " << number << std::endl;
    number = number - numToRemove * val;
    // std::cout << "Number: " << number << std::endl;
    digit++;
    if (digit == 60000000) {
      throw std::runtime_error("Stuck");
    }
  }
  return snafu;
}

int main(int argc, char** argv) {
  for (int i = -50000; i < 50000; ++i) {
    auto exampleString = decimalToSnafu(i);
    int back = snafuToDecimal(exampleString);
    if (i != back) {
      std::cout << i << " : " << exampleString << " : " << back << std::endl;
      throw std::runtime_error("Failure.");
    }
  }
  std::string line;
  long long summed = 0;
  while (std::getline(std::cin, line)) {
    std::cout << line << std::endl;
    summed += snafuToDecimal(line);
  }
  std::cout << "Sum: " << decimalToSnafu(summed) << std::endl;
  return 0;
}
