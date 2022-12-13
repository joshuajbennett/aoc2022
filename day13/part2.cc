#include <cmath>
#include <deque>
#include <functional>
#include <iostream>
#include <optional>
#include <queue>
#include <sstream>
#include <string>
#include <vector>

// To run build the code and then do the following command:
// ```
// cat ../day13/in.txt | day13/day13part2
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

enum class EntryType { List, Value };

struct Entry {
  std::string data;
  EntryType type;
};

std::vector<Entry> getEntries(const std::string& element) {
  std::vector<Entry> entries;
  int previousEntry = 0;
  for (int i = 0; i < element.size(); ++i) {
    if (element[i] == '[') {
      std::string data;
      int numBrackets = 1;
      int origin = i;
      for (int j = origin + 1; j < element.size(); ++j) {
        if (element[j] == '[') {
          numBrackets++;
        }
        if (element[j] == ']') {
          numBrackets--;
        }
        if (numBrackets == 0) {
          entries.push_back(Entry{element.substr(origin, j - origin + 1), EntryType::List});
          i = j + 1;
          previousEntry = j + 2;
          break;
        }
      }
    } else if (element[i] == ',') {
      entries.push_back(Entry{element.substr(previousEntry, i - previousEntry), EntryType::Value});
      previousEntry = i + 1;
    }
  }
  if (previousEntry < element.size()) {
    entries.push_back(Entry{element.substr(previousEntry), EntryType::Value});
  }
  return entries;
}

void renderEntries(const std::vector<Entry>& entries) {
  for (const auto& entry : entries) {
    std::cout << "Type: " << (entry.type == EntryType::List ? "List " : "Value ") << entry.data << std::endl;
  }
  std::cout << std::endl;
}

std::optional<bool> compareEntries(const std::string left, const std::string right) {
  // std::cout << std::endl;
  // std::cout << "Left: " << left << " Right: " << right << std::endl;
  if (left.size() == 0 && right.size() == 0) {
    return std::nullopt;
  }
  if (left.size() == 0) {
    return true;
  }
  if (right.size() == 0) {
    return false;
  }
  auto leftEntries = getEntries(left);
  auto rightEntries = getEntries(right);
  // std::cout << "NumLeftEntries: " << leftEntries.size() << " NumRightEntries: " << rightEntries.size() << std::endl;
  for (int i = 0; i < leftEntries.size() && i < rightEntries.size(); ++i) {
    auto leftEntry = leftEntries[i];
    auto rightEntry = rightEntries[i];
    // std::cout << "Left Entry: " << leftEntry << " Right Entry: " << rightEntry << std::endl;
    if (leftEntry.type == EntryType::Value && rightEntry.type == EntryType::Value) {
      int leftVal = std::stoi(leftEntry.data);
      int rightVal = std::stoi(rightEntry.data);
      if (leftVal < rightVal) {
        return true;
      } else if (leftVal > rightVal) {
        return false;
      }
      // Equal, continue checking.
    } else if (leftEntry.type == EntryType::List && rightEntry.type == EntryType::List) {
      auto compare = compareEntries(leftEntry.data.substr(1, leftEntry.data.size() - 2),
                                    rightEntry.data.substr(1, rightEntry.data.size() - 2));
      if (compare.has_value()) {
        return compare;
      }
    } else if (leftEntry.type == EntryType::Value) {
      auto compare = compareEntries(leftEntry.data, rightEntry.data.substr(1, rightEntry.data.size() - 2));
      if (compare.has_value()) {
        return compare;
      }
    } else if (rightEntry.type == EntryType::Value) {
      auto compare = compareEntries(leftEntry.data.substr(1, leftEntry.data.size() - 2), rightEntry.data);
      if (compare.has_value()) {
        return compare;
      }
    }
  }
  if (leftEntries.size() < rightEntries.size()) {
    return true;
  }
  if (leftEntries.size() > rightEntries.size()) {
    return false;
  }
  return std::nullopt;
}

// enum class EntryType { List, Value };
//
// struct Entry {
//   std::string data;
//   EntryType type;
// };

int main(int argc, char** argv) {
  std::string line;
  std::vector<std::string> lines;
  while (std::getline(std::cin, line)) {
    if (line != "") {
      lines.push_back(line);
    }
  }

  lines.push_back("[[2]]");
  lines.push_back("[[6]]");

  std::sort(lines.begin(), lines.end(), [](const std::string& leftLine, const std::string& rightLine) -> bool {
    auto compare = compareEntries(leftLine.substr(1, leftLine.size() - 2), rightLine.substr(1, rightLine.size() - 2));
    if (compare.has_value()) {
      return compare.value();
    }
    return false;
  });
  for (const auto& line : lines) {
    std::cout << line << std::endl;
  }
  int firstIndex = 1;
  bool foundFirst = false;
  int secondIndex = 1;
  for (const auto& line : lines) {
    if (line == "[[2]]") {
      foundFirst = true;
    }
    if (foundFirst && line == "[[6]]") {
      break;
    }

    secondIndex++;
    if (!foundFirst) {
      firstIndex++;
    }
  }
  std::cout << "Multiplied indices = " << secondIndex * firstIndex << std::endl;
  return 0;
}
