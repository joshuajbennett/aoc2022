#include <cmath>
#include <deque>
#include <functional>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

// To run build the code and then do the following command:
// ```
// cat ../day9/in.txt | day9/day9part1
// ```

struct Monkey {
  int number;
  std::deque<int> items;
  std::function<int(const int)> operation;
  int divisibleBy;
  int trueAction;  // Who to throw to if true.
  int falseAction; // Who to throw to if false.
  int numInspections;
};

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

int main(int argc, char** argv) {
  std::string line;
  std::vector<Monkey> monkeys;
  while (std::getline(std::cin, line)) {
    auto elem = split(line, ' ');
    if (elem[0] != "Monkey") {
      std::cout << "No more monkeys." << std::endl;
      return 0;
    }
    int number = std::stoi(elem[1]);
    std::cout << "Reading in monkey " << number << std::endl;
    // Read starting items
    std::getline(std::cin, line);
    auto starting = split(line, ' ');
    std::deque<int> items;
    for (int i = 2; i < starting.size(); ++i) {
      if (i == starting.size() - 1) {
        items.push_back(std::stoi(starting[i]));
      } else {
        items.push_back(std::stoi(starting[i].substr(0, starting[i].size() - 1)));
      }
    }
    // Read the operation line.
    std::getline(std::cin, line);
    auto opElem = split(line, ' ');
    if (opElem[0] != "Operation:") {
      throw std::runtime_error("Operation not parsed.");
    }

    std::function<int(const int)> operation = [&]() -> std::function<int(const int)> {
      if (opElem[4] == "+") {
        if (opElem[5] == "old") {
          return [](const int old) -> int { return old + old; };
        } else {
          int amount = std::stoi(opElem[5]);
          return [=](const int old) -> int { return old + amount; };
        }
      } else if (opElem[4] == "*") {
        if (opElem[5] == "old") {
          return [](const int old) -> int { return old * old; };
        } else {
          int amount = std::stoi(opElem[5]);
          return [=](const int old) -> int { return old * amount; };
        }
      }
      throw std::runtime_error("Unsupported operation type.");
      return [](const int old) -> int { return old; };
    }();

    // Read the divisible line.
    std::getline(std::cin, line);
    auto divisibleElem = split(line, ' ');
    int divisibleBy = std::stoi(divisibleElem[3]);

    // Read the true line.
    std::getline(std::cin, line);
    auto trueElem = split(line, ' ');
    int trueAction = std::stoi(trueElem[5]);

    // Read the false line.
    std::getline(std::cin, line);
    auto falseElem = split(line, ' ');
    int falseAction = std::stoi(falseElem[5]);

    monkeys.push_back(Monkey{number, items, operation, divisibleBy, trueAction, falseAction, 0});
    // Extra line.
    std::getline(std::cin, line);
  }

  auto postInspectionRelief = [](const int old) -> int {
    return static_cast<int>(std::floor(static_cast<double>(old) / 3.0));
  };

  int numRounds = 20;

  for (int round = 0; round < numRounds; ++round) {
    for (auto& monkey : monkeys) {
      while (monkey.items.size() > 0) {
        int item = monkey.items.front();
        monkey.items.pop_front();
        monkey.numInspections++;
        int postInspection = monkey.operation(item);
        int relief = postInspectionRelief(postInspection);
        if (relief % monkey.divisibleBy == 0) {
          monkeys[monkey.trueAction].items.push_back(relief);
        } else {
          monkeys[monkey.falseAction].items.push_back(relief);
        }
      }
    }
  }

  std::vector<int> inspections;
  for (auto& monkey : monkeys) {
    std::cout << "Monkey " << monkey.number << " inspected items " << monkey.numInspections << " times." << std::endl;
    inspections.push_back(monkey.numInspections);
  }
  std::sort(inspections.begin(), inspections.end(), std::greater<int>());

  std::cout << "Monkey business: " << inspections[0] * inspections[1] << std::endl;

  return 0;
}

// struct Monkey {
//   int number;
//   std::deque<int> items;
//   std::function<int(const int)> operation;
//   int divisibleBy;
//   int trueAction;  // Who to throw to if true.
//   int falseAction; // Who to throw to if false.
// };
