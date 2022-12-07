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
  Directory* parentDirectory;
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

void parseCommand(Directory& dir, const std::string command) {
  std::string cmdName = command.substr(2, 2);
  std::string line;
  if (cmdName == "cd") {
    std::string dirName = command.substr(5);
    if (dirName == "..") {
      // Get the command line and parse the next command.
      if (std::getline(std::cin, line)) {
        if (line[0] != '$') {
          std::cerr << "Expected a command." << std::endl;
          return;
        }
        parseCommand(*dir.parentDirectory, line);
      }
      return;
    }
    for (auto& directory : dir.directories) {
      if (directory.name == dirName) {
        // Get the command line and parse the next command.
        if (std::getline(std::cin, line)) {
          if (line[0] != '$') {
            std::cerr << "Expected a command." << std::endl;
            return;
          }
          parseCommand(directory, line);
        }
        return;
      }
    }
  } else if (cmdName == "ls") {
    std::string line;
    while (std::getline(std::cin, line)) {
      if (line[0] == '$') {
        parseCommand(dir, line);
        return;
      } else {
        auto elements = split(line, ' ');
        if (elements[0] == "dir") {
          bool foundDir = false;
          std::string dirName = elements[1];
          for (const auto& directory : dir.directories) {
            if (directory.name == dirName) {
              foundDir = true;
            }
          }
          if (!foundDir) {
            dir.directories.push_back(Directory{dirName, {}, {}, 0, &dir});
          }
        } else {
          int size = std::stoi(elements[0]);
          bool foundFile = false;
          std::string fileName = elements[1];
          for (const auto& file : dir.files) {
            if (file.name == fileName) {
              foundFile = true;
            }
          }
          if (!foundFile) {
            dir.files.push_back(File{fileName, size});
          }
        }
      }
    }
  }
  std::cerr << "Unexpected command name: " << cmdName << std::endl;
  return;
}

std::string extractFile(const File& file, const int depth) {
  std::string currentLine(depth, ' ');
  currentLine += "- ";
  currentLine += file.name;
  currentLine += " (file, size=";
  currentLine += std::to_string(file.size);
  currentLine += ")";
  return currentLine;
}

std::vector<std::string> extractDirectory(const Directory& dir, const int depth) {
  std::string currentLine(depth, ' ');
  currentLine += "- ";
  currentLine += dir.name;
  currentLine += " (dir)";
  std::vector<std::string> lines{currentLine};
  for (const auto& directory : dir.directories) {
    auto newLines = extractDirectory(directory, depth + 2);
    for (const auto& newLine : newLines) {
      lines.push_back(newLine);
    }
  }
  for (const auto& file : dir.files) {
    lines.push_back(extractFile(file, depth + 2));
  }
  return lines;
}

int main(int argc, char** argv) {
  std::string line;
  Directory initial;
  initial.name = "/";
  std::getline(std::cin, line);
  if (line != "$ cd /") {
    std::cerr << "Didn't start at the root directory." << std::endl;
    return -1;
  }
  if (std::getline(std::cin, line)) {
    parseCommand(initial, line);
  }

  int depth = 0;
  auto lines = extractDirectory(initial, depth);

  std::cout << "Printing: " << std::endl;
  for (const auto& line : lines) {
    std::cout << line << std::endl;
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
