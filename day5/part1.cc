#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

// To run build the code and then do the following command:
// ```
// cat ../day5/in.txt | day5/day5part1
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

void renderCrates(const std::vector<std::vector<char>>& crateStacks) {
  size_t maxStack = 0;
  for (int i = 0; i < crateStacks.size(); ++i) {
    maxStack = std::max(maxStack, crateStacks[i].size());
  }
  for (int y = maxStack - 1; y >= 0; --y) {
    for (int i = 0; i < crateStacks.size(); ++i) {
      if (y + 1 <= crateStacks[i].size()) {
        std::cout << "[" << crateStacks[i][y] << "]"
                  << " ";
      } else {
        std::cout << "    ";
      }
    }
    std::cout << std::endl;
  }
  for (int i = 0; i < crateStacks.size(); ++i) {
    std::cout << " " << i + 1 << "  ";
  }
  std::cout << std::endl;
}

int main(int argc, char** argv) {
  std::string line;
  std::vector<std::string> initialCrateStrings;
  std::vector<std::vector<char>> crateStacks;
  // Parse the initial crate configuration.
  while (std::getline(std::cin, line)) {
    if (line.find('[') == std::string::npos && line.find(']') == std::string::npos) {
      auto numbers = split(line, ' ');
      if (numbers.size() < 1) {
        throw std::runtime_error("Couldn't find any numbers after getting initial crate stack.");
      }
      int numCrates = std::stoi(numbers[numbers.size() - 1]);
      crateStacks.resize(numCrates);
      std::reverse(initialCrateStrings.begin(), initialCrateStrings.end());
      auto indexToX = [](const int index) { return 1 + index * 4; };
      for (const auto& crateLine : initialCrateStrings) {
        for (int i = 0; indexToX(i) < crateLine.size(); ++i) {
          char crate = crateLine[indexToX(i)];
          if (crate != ' ') {
            crateStacks[i].push_back(crate);
          }
        }
      }
      break;
    }
    initialCrateStrings.push_back(line);
  }

  renderCrates(crateStacks);
  // Clear the blank line.
  std::getline(std::cin, line);
  if (line != "") {
    throw std::runtime_error("Didn't encounter a blank line after the initial crate stack.");
  }
  // Parse the commands
  while (std::getline(std::cin, line)) {
    auto commands = split(line, ' ');
    if (commands.size() != 6) {
      throw std::runtime_error("Expected 6 commands.");
    }
    // Do the command N times.
    for (int i = 0; i < std::stoi(commands[1]); ++i) {
      int from = std::stoi(commands[3]) - 1;
      int to = std::stoi(commands[5]) - 1;
      if (crateStacks[from].size() < 1) {
        throw std::runtime_error("Moving from an empty stack.");
      }
      crateStacks[to].push_back(crateStacks[from].back());
      crateStacks[from].pop_back();
    }
    // renderCrates(crateStacks);
  }
  renderCrates(crateStacks);
  for (const auto& stack : crateStacks) {
    if (stack.size() == 0) {
      std::cout << " ";
    } else {
      std::cout << stack.back();
    }
  }
  std::cout << std::endl;

  return 0;
}
