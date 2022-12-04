#include <iostream>
#include <string>
#include <unordered_set>

// To run build the code and then do the following command:
// ```
// cat ../day3/in.txt | day3/day3part1
// ```

int getPriority(const char& character) {
  int characterNum = static_cast<int>(character);
  int a = static_cast<int>('a');
  int z = static_cast<int>('z');
  int capitalA = static_cast<int>('A');
  int capitalZ = static_cast<int>('Z');
  if (characterNum >= a && characterNum <= z) {
    return characterNum - a + 1;
  }
  if (characterNum >= capitalA && characterNum <= capitalZ) {
    return characterNum - capitalA + 27;
  }
  throw std::runtime_error("Invalid rucksack character.");
}

int main(int argc, char** argv) {
  int totalPriority = 0;
  std::string line;
  while (std::getline(std::cin, line)) {
    std::string firstCompartment = line.substr(0, line.size() / 2);
    std::string secondCompartment = line.substr(line.size() / 2, line.size() / 2);
    std::unordered_set<char> firstSet;
    std::unordered_set<char> secondSet;
    for (int i = 0; i < firstCompartment.size(); ++i) {
      firstSet.insert(firstCompartment[i]);
    }
    for (int i = 0; i < secondCompartment.size(); ++i) {
      if (firstSet.find(secondCompartment[i]) != firstSet.end()) {
        totalPriority += getPriority(secondCompartment[i]);
        break;
      }
    }
  }

  std::cout << "Total priority: " << totalPriority << std::endl;

  return 0;
}
