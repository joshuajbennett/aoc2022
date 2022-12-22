#include <atomic>
#include <cmath>
#include <deque>
#include <functional>
#include <iostream>
#include <mutex>
#include <optional>
#include <queue>
#include <sstream>
#include <string>
#include <thread>
#include <unordered_map>
#include <unordered_set>
#include <vector>

enum class Direction { Up, Right, Down, Left };

struct Loc {
  int row = 0;
  int column = 0;
};

struct Node {
  Node* up = nullptr;
  Node* right = nullptr;
  Node* down = nullptr;
  Node* left = nullptr;
  bool isWall = false;
  Loc location;
};

enum class Rotate { Left, Right };

struct Instruction {
  bool isMove = false;
  int moveDistance = 0;
  Rotate rotate;
};

int main(int argc, char** argv) {
  std::vector<std::string> lines;
  std::string line;
  int maxLineSize = 0;
  while (std::getline(std::cin, line)) {
    if (line == "") {
      break;
    }
    lines.push_back(line);
    if (line.size() > maxLineSize) {
      maxLineSize = line.size();
    }
  }

  // Parse the grid.
  std::vector<std::vector<std::unique_ptr<Node>>> grid;
  int rowNum = 0;
  for (const auto& row : lines) {
    std::vector<std::unique_ptr<Node>> rowContents;
    rowContents.resize(maxLineSize);
    for (int i = 0; i < row.size(); ++i) {
      if (row[i] == ' ') {
      } else {
        rowContents[i] = std::make_unique<Node>();
        auto node = rowContents[i].get();
        node->location.row = rowNum;
        node->location.column = i;
        if (row[i] == '#') {
          node->isWall = true;
        } else if (row[i] == '.') {
          node->isWall = false;
        } else {
          throw std::runtime_error("Unrecognized");
        }
      }
    }
    rowNum++;
    grid.push_back(std::move(rowContents));
  }

  Node* current;
  bool findCurrent = true;
  for (int y = 0; y < grid.size(); y++) {
    Node* lastRightSweepNode;
    bool foundRightSweepNode = false;
    // Search left to get rightmost node.
    for (int x = maxLineSize - 1; x >= 0; x--) {
      if (grid[y][x]) {
        lastRightSweepNode = grid[y][x].get();
        foundRightSweepNode = true;
        break;
      }
    }
    if (foundRightSweepNode) {
      Node* lastLeftSweepNode;
      bool foundLeftSweepNode = false;
      for (int x = 0; x < maxLineSize; x++) {
        if (grid[y][x]) {
          if (!foundLeftSweepNode) {
            foundLeftSweepNode = true;
            lastLeftSweepNode = grid[y][x].get();
            if (findCurrent) {
              current = lastLeftSweepNode;
              findCurrent = false;
            }
          }
          grid[y][x]->left = lastRightSweepNode;
          lastRightSweepNode = grid[y][x].get();
        }
      }
      if (foundLeftSweepNode) {
        for (int x = maxLineSize - 1; x >= 0; x--) {
          if (grid[y][x]) {
            grid[y][x]->right = lastLeftSweepNode;
            lastLeftSweepNode = grid[y][x].get();
          }
        }
      } else {
        throw std::runtime_error("Found right sweep node but not a left sweep node.");
      }
    }
  }

  for (int x = 0; x < maxLineSize; x++) {
    Node* lastUpSweepNode;
    bool foundUpSweepNode = false;
    // Search left to get rightmost node.
    for (int y = grid.size() - 1; y >= 0; y--) {
      if (grid[y][x]) {
        lastUpSweepNode = grid[y][x].get();
        foundUpSweepNode = true;
        break;
      }
    }
    if (foundUpSweepNode) {
      Node* lastDownSweepNode;
      bool foundDownSweepNode = false;
      for (int y = 0; y < grid.size(); y++) {
        if (grid[y][x]) {
          if (!foundDownSweepNode) {
            foundDownSweepNode = true;
            lastDownSweepNode = grid[y][x].get();
          }
          grid[y][x]->up = lastUpSweepNode;
          lastUpSweepNode = grid[y][x].get();
        }
      }
      if (foundDownSweepNode) {
        for (int y = grid.size() - 1; y >= 0; y--) {
          if (grid[y][x]) {
            grid[y][x]->down = lastDownSweepNode;
            lastDownSweepNode = grid[y][x].get();
          }
        }
      } else {
        throw std::runtime_error("Found up sweep node but not a down sweep node.");
      }
    }
  }

  // Parse the line of instructions.
  std::string lineOfInstructions;
  std::getline(std::cin, lineOfInstructions);
  std::vector<Instruction> instructions;
  int lastInstrIndex = 0;
  bool accumulatingNumber = false;
  for (int i = 0; i < lineOfInstructions.size(); ++i) {
    if (lineOfInstructions[i] == 'L') {
      if (accumulatingNumber) {
        accumulatingNumber = false;
        instructions.push_back(
            Instruction{true, std::stoi(lineOfInstructions.substr(lastInstrIndex, i - lastInstrIndex + 1))});
      }
      instructions.push_back(Instruction{false, 0, Rotate::Left});
    } else if (lineOfInstructions[i] == 'R') {
      if (accumulatingNumber) {
        accumulatingNumber = false;
        instructions.push_back(
            Instruction{true, std::stoi(lineOfInstructions.substr(lastInstrIndex, i - lastInstrIndex + 1))});
      }
      instructions.push_back(Instruction{false, 0, Rotate::Right});
    } else {
      if (!accumulatingNumber) {
        accumulatingNumber = true;
        lastInstrIndex = i;
      }
    }
  }
  if (accumulatingNumber) {
    instructions.push_back(Instruction{true, std::stoi(lineOfInstructions.substr(lastInstrIndex))});
  }

  Direction dir{Direction::Right};

  for (const auto& instruction : instructions) {
    if (instruction.isMove) {
      for (int i = 0; i < instruction.moveDistance; ++i) {
        bool done = false;
        switch (dir) {
        case Direction::Up:
          if (current->up->isWall) {
            done = true;
          } else {
            current = current->up;
          }
          break;
        case Direction::Right:
          if (current->right->isWall) {
            done = true;
          } else {
            current = current->right;
          }
          break;
        case Direction::Down:
          if (current->down->isWall) {
            done = true;
          } else {
            current = current->down;
          }
          break;
        case Direction::Left:
          if (current->left->isWall) {
            done = true;
          } else {
            current = current->left;
          }
          break;
        }
        if (done) {
          break;
        }
      }
    } else {
      switch (dir) {
      case Direction::Up:
        switch (instruction.rotate) {
        case Rotate::Right:
          dir = Direction::Right;
          break;
        case Rotate::Left:
          dir = Direction::Left;
          break;
        }
        break;
      case Direction::Right:
        switch (instruction.rotate) {
        case Rotate::Right:
          dir = Direction::Down;
          break;
        case Rotate::Left:
          dir = Direction::Up;
          break;
        }
        break;
      case Direction::Down:
        switch (instruction.rotate) {
        case Rotate::Right:
          dir = Direction::Left;
          break;
        case Rotate::Left:
          dir = Direction::Right;
          break;
        }
        break;
      case Direction::Left:
        switch (instruction.rotate) {
        case Rotate::Right:
          dir = Direction::Up;
          break;
        case Rotate::Left:
          dir = Direction::Down;
          break;
        }
        break;
      }
    }
  }

  int sum = 0;
  sum = sum + 1000 * (current->location.row + 1);
  sum = sum + 4 * (current->location.column + 1);
  switch (dir) {
  case Direction::Up:
    sum = sum + 3;
    break;
  case Direction::Right:
    sum = sum + 0;
    break;
  case Direction::Down:
    sum = sum + 1;
    break;
  case Direction::Left:
    sum = sum + 2;
    break;
  }
  std::cout << "Sum: " << sum << std::endl;

  return 0;
}
