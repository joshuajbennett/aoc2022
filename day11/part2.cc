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
  std::deque<long long> items;
  std::function<long long(const long long)> operation;
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
    std::deque<long long> items;
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

    std::function<long long(const long long)> operation = [&]() -> std::function<long long(const long long)> {
      if (opElem[4] == "+") {
        if (opElem[5] == "old") {
          return [](const long long old) -> long long { return old + old; };
        } else {
          long long amount = std::stoi(opElem[5]);
          return [=](const long long old) -> long long { return old + amount; };
        }
      } else if (opElem[4] == "*") {
        if (opElem[5] == "old") {
          return [](const long long old) -> long long { return old * old; };
        } else {
          long long amount = std::stoi(opElem[5]);
          return [=](const long long old) -> long long { return old * amount; };
        }
      }
      throw std::runtime_error("Unsupported operation type.");
      return [](const long long old) -> long long { return old; };
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

  int leastCommonDenominator = 1;
  for (auto& monkey : monkeys) {
    leastCommonDenominator = leastCommonDenominator * monkey.divisibleBy;
  }

  auto postInspectionRelief = [=](const long long old) -> long long {
    // By calculating the least common denominator of all the divisibleBy checks we have provided a way
    // to continually reduce the size of the worry levels. The reason for this is that all operations are
    // addition or multiplication, so the only term that continues to matter in the number is the
    // remainder of the least common denominator.
    return old % leastCommonDenominator;
  };

  int numRounds = 10000;

  for (int round = 0; round < numRounds; ++round) {
    for (auto& monkey : monkeys) {
      while (monkey.items.size() > 0) {
        long long item = monkey.items.front();
        monkey.items.pop_front();
        monkey.numInspections++;
        long long postInspection = monkey.operation(item);
        long long relief = postInspectionRelief(postInspection);
        if (relief % monkey.divisibleBy == 0) {
          monkeys[monkey.trueAction].items.push_back(relief);
        } else {
          monkeys[monkey.falseAction].items.push_back(relief);
        }
      }
    }
  }

  std::vector<long long> inspections;
  for (auto& monkey : monkeys) {
    std::cout << "Monkey " << monkey.number << " inspected items " << monkey.numInspections << " times." << std::endl;
    inspections.push_back(monkey.numInspections);
  }
  std::sort(inspections.begin(), inspections.end(), std::greater<long long>());

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
