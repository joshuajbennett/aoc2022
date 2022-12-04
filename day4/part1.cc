#include <iostream>
#include <string>

// To run build the code and then do the following command:
// ```
// cat ../day4/in.txt | day4/day4part1
// ```

int main(int argc, char** argv) {
  std::string line;
  int numFullyContains = 0;
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
      numFullyContains++;
    } else if (rightFirstNum <= leftFirstNum && rightSecondNum >= leftSecondNum) {
      numFullyContains++;
    }
  }
  std::cout << "Fully contains occurs " << numFullyContains << " times." << std::endl;

  return 0;
}
