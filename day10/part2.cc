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
void popCycle(int cycleNumber, int X, std::deque<int>& cycles) {
  if (cycleNumber == cycles.front()) {
    cycles.pop_front();
  }

  int rowWidth = 40;
  int locInRow = cycleNumber % rowWidth;

  if (locInRow == X || locInRow == X + 1 || locInRow == X - 1) {
    std::cout << "#";
  } else {
    std::cout << ".";
  }

  if (locInRow == 0) {
    std::cout << std::endl;
  }
}

int main(int argc, char** argv) {
  int X = 1;
  int cycleNumber = 0;
  std::deque<int> cycles{20, 60, 100, 140, 180, 220};

  std::string line;
  while (std::getline(std::cin, line)) {
    if (line == "noop") {
      cycleNumber++;
      popCycle(cycleNumber, X, cycles);
    } else {
      cycleNumber++;
      // std::cout << "First" << std::endl;
      popCycle(cycleNumber, X, cycles);
      cycleNumber++;
      auto elements = split(line, ' ');
      auto addNum = std::stoi(elements[1]);
      X += addNum;
      // std::cout << "Second" << std::endl;
      popCycle(cycleNumber, X, cycles);
      // std::cout << "AddNum: " << addNum << std::endl;
    }
  }

  return 0;
}
