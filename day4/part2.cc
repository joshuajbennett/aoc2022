#include <iostream>
#include <string>

// To run build the code and then do the following command:
// ```
// cat ../day4/in.txt | day4/day4part2
// ```

int main(int argc, char** argv) {
  std::string line;
  int numPartialContains = 0;
  while (std::getline(std::cin, line)) {
    auto pos = line.find(",");
    if (pos == std::string::npos) {
      throw std::runtime_error("Couldn't find comma delimeter.");
    }
    std::string left = line.substr(0, pos);
    line.erase(0, pos + 1);

    auto dashPos = left.find("-");
    if (dashPos == std::string::npos) {
      throw std::runtime_error("Couldn't find dash delimeter in left.");
    }
    int leftFirstNum = std::stoi(left.substr(0, dashPos));
    left.erase(0, dashPos + 1);
    int leftSecondNum = std::stoi(left);

    dashPos = line.find("-");
    if (dashPos == std::string::npos) {
      throw std::runtime_error("Couldn't find dash delimeter in right.");
    }
    int rightFirstNum = std::stoi(line.substr(0, dashPos));
    line.erase(0, dashPos + 1);
    int rightSecondNum = std::stoi(line);

    if (leftFirstNum <= rightFirstNum && leftSecondNum >= rightSecondNum) {
      numPartialContains++; // Fully contained by left group
    } else if (rightFirstNum <= leftFirstNum && rightSecondNum >= leftSecondNum) {
      numPartialContains++; // Fully contained by right group
    } else if (rightFirstNum <= leftFirstNum && rightSecondNum >= leftFirstNum) {
      numPartialContains++; // Left first number partially contained by right group
    } else if (rightFirstNum <= leftSecondNum && rightSecondNum >= leftSecondNum) {
      numPartialContains++; // Left second number partially contained by right group
    } else if (leftFirstNum <= rightFirstNum && leftSecondNum >= rightFirstNum) {
      numPartialContains++; // Right first number partially contained by left group
    } else if (leftFirstNum <= rightSecondNum && leftSecondNum >= rightSecondNum) {
      numPartialContains++; // Right second number partially contained by left group
    }
  }
  std::cout << "Partial contains occurs " << numPartialContains << " times." << std::endl;

  return 0;
}
