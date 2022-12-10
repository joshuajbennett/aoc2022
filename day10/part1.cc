#include <deque>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

// To run build the code and then do the following command:
// ```
// cat ../day9/in.txt | day9/day9part1
// ```

std::vector<std::string> split(const std::string& input, const char delim) {
  std::stringstream sstr(input);
  std::string item;
  std::vector<std::string> elements;
  while (std::getline(sstr, item, delim)) {
    if (item != "") {
      elements.push_back(item);
    }
  }
  return elements;
}
int popCycleAndSum(int sumSignalStrength, int cycleNumber, int X, std::deque<int>& cycles) {
  if (cycleNumber == cycles.front()) {
    // std::cout << "Hit cycle " << cycleNumber << " with X " << X << " and strength " << cycleNumber * X << std::endl;
    cycles.pop_front();
    return sumSignalStrength + cycleNumber * X;
  }
  return sumSignalStrength;
}

int main(int argc, char** argv) {
  int X = 1;
  int cycleNumber = 0;
  int sumSignalStrength = 0;
  std::deque<int> cycles{20, 60, 100, 140, 180, 220};

  std::vector<bool> row(40, false);

  std::string line;
  while (std::getline(std::cin, line)) {
    if (line == "noop") {
      cycleNumber++;
      sumSignalStrength = popCycleAndSum(sumSignalStrength, cycleNumber, X, cycles);
    } else {
      cycleNumber++;
      // std::cout << "First" << std::endl;
      sumSignalStrength = popCycleAndSum(sumSignalStrength, cycleNumber, X, cycles);
      cycleNumber++;
      auto elements = split(line, ' ');
      auto addNum = std::stoi(elements[1]);
      X += addNum;
      // std::cout << "Second" << std::endl;
      sumSignalStrength = popCycleAndSum(sumSignalStrength, cycleNumber, X, cycles);
      // std::cout << "AddNum: " << addNum << std::endl;
    }
  }

  std::cout << "Sum of signal strength: " << sumSignalStrength << std::endl;

  return 0;
}
