#include <iostream>
#include <sstream>
#include <string>
#include <vector>

// To run build the code and then do the following command:
// ```
// cat ../day7/in.txt | day7/day7part1
// ```

struct File {
  std::string name;
  int size;
};

struct Directory {
  std::string name;
  std::vector<File> files;
  std::vector<Directory> directories;
  int totalSize = 0;
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

Directory parseDirectory(const std::vector<std::string>& contents) {
  Directory currentDir;
  // Get the initial depth.
  size_t initialDepth = contents[0].find('-');
  // Extract the name.
  currentDir.name = split(contents[0], ' ')[1];
  // Loop through remaining elements.
  for (int i = 1; i < contents.size(); ++i) {
    auto lineElements = split(contents[i], ' ');
    if (lineElements[2] == "(dir)") {
      std::vector<std::string> dirContents{contents[i++]};
      for (; i < contents.size(); ++i) {
        size_t newDepth = contents[i].find('-');
        if (newDepth <= initialDepth + 2) {
          // Finished with the new directory, parse the lines we have accumulated.
          currentDir.directories.push_back(parseDirectory(dirContents));
          currentDir.totalSize += currentDir.directories.back().totalSize;
          // Decrement so we re-evaluate the current line.
          --i;
          break;
        }
        // Append to the current set of lines.
        dirContents.push_back(contents[i]);
      }
      if (i == contents.size()) {
        currentDir.directories.push_back(parseDirectory(dirContents));
        currentDir.totalSize += currentDir.directories.back().totalSize;
      }
    } else {
      auto fileLine = split(contents[i], ' ');
      std::string fileName = fileLine[1];
      auto sizeLine = split(fileLine[3], '=');
      auto size = std::stoi(split(sizeLine[1], ')')[0]);
      currentDir.totalSize += size;
      currentDir.files.push_back(File{fileName, size});
    }
  }
  return currentDir;
}

int countSizesBelowValue(const Directory& dir, const int value) {
  int totalSize = 0;
  for (const auto& directory : dir.directories) {
    if (directory.totalSize <= value) {
      totalSize += directory.totalSize;
    }
    totalSize += countSizesBelowValue(directory, value);
  }
  return totalSize;
}

int main(int argc, char** argv) {
  std::vector<std::string> lines;
  std::string line;
  while (std::getline(std::cin, line)) {
    auto findDash = line.find('-');
    if (findDash == std::string::npos) {
      std::cerr << "Encountered a line without a dash." << std::endl;
      return -1;
    }
    lines.push_back(line);
  }
  auto root = parseDirectory(lines);

  int maxSize = 100000;
  int totalSize = countSizesBelowValue(root, maxSize);
  if (root.totalSize <= maxSize) {
    totalSize += root.totalSize;
  }
  std::cout << "Total size: " << totalSize << std::endl;
  return 0;
}
