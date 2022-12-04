#include <iostream>
#include <string>
#include <unordered_set>

// To run build the code and then do the following command:
// ```
// cat ../day3/in.txt | day3/day3part2
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
  int groupMember = 0;
  std::unordered_set<char> firstSet;
  std::unordered_set<char> secondSet;
  while (std::getline(std::cin, line)) {
    if (groupMember == 0) {
      for (int i = 0; i < line.size(); ++i) {
        firstSet.insert(line[i]);
      }
    } else if (groupMember == 1) {
      for (int i = 0; i < line.size(); ++i) {
        secondSet.insert(line[i]);
      }
    } else {
      for (int i = 0; i < line.size(); ++i) {
        if (firstSet.find(line[i]) != firstSet.end() && secondSet.find(line[i]) != secondSet.end()) {
          totalPriority += getPriority(line[i]);
          break;
        }
      }
    }
    groupMember++;
    if (groupMember == 3) {
      groupMember = 0;
      firstSet.clear();
      secondSet.clear();
    }
  }

  std::cout << "Total priority: " << totalPriority << std::endl;

  return 0;
}
