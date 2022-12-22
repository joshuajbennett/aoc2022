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
  int y = 0;
  int x = 0;
};

enum class Rotate { Left, Right, Reverse };

struct Node {
  Node* up = nullptr;
  std::optional<Rotate> upTransition = std::nullopt;
  Node* right = nullptr;
  std::optional<Rotate> rightTransition = std::nullopt;
  Node* down = nullptr;
  std::optional<Rotate> downTransition = std::nullopt;
  Node* left = nullptr;
  std::optional<Rotate> leftTransition = std::nullopt;
  bool isWall = false;
  Loc location;
};

struct Instruction {
  bool isMove = false;
  int moveDistance = 0;
  Rotate rotate;
};

struct CubeFace {
  int width;
  int height;
  int row;
  int column;
};

Loc traverse(Direction& dir, Node* current, int stepsRemaining) {
  if (stepsRemaining == 0) {
    return current->location;
  }
  Node* newNode;
  switch (dir) {
  case Direction::Up:
    if (current->upTransition.has_value()) {
      switch (current->upTransition.value()) {
      case Rotate::Right:
        dir = Direction::Right;
        break;
      case Rotate::Left:
        dir = Direction::Left;
        break;
      case Rotate::Reverse:
        dir = Direction::Down;
        break;
      }
    }
    newNode = current->up;
    break;
  case Direction::Right:
    if (current->rightTransition.has_value()) {
      switch (current->rightTransition.value()) {
      case Rotate::Right:
        dir = Direction::Down;
        break;
      case Rotate::Left:
        dir = Direction::Up;
        break;
      case Rotate::Reverse:
        dir = Direction::Left;
        break;
      }
    }
    newNode = current->right;
    break;
  case Direction::Down:
    if (current->downTransition.has_value()) {
      switch (current->downTransition.value()) {
      case Rotate::Right:
        dir = Direction::Left;
        break;
      case Rotate::Left:
        dir = Direction::Right;
        break;
      case Rotate::Reverse:
        dir = Direction::Up;
        break;
      }
    }
    newNode = current->down;
    break;
  case Direction::Left:
    if (current->leftTransition.has_value()) {
      switch (current->leftTransition.value()) {
      case Rotate::Right:
        dir = Direction::Up;
        break;
      case Rotate::Left:
        dir = Direction::Down;
        break;
      case Rotate::Reverse:
        dir = Direction::Right;
        break;
      }
    }
    newNode = current->left;
    break;
  }
  if (std::abs(newNode->location.x - current->location.x) > 1 ||
      std::abs(newNode->location.y - current->location.y) > 1) {
    std::string dirString = [&]() {
      switch (dir) {
      case Direction::Left:
        return "left";
      case Direction::Right:
        return "right";
      case Direction::Up:
        return "up";
      case Direction::Down:
        return "down";
      }
    }();
    std::cout << "Transitioning from {" << current->location.y << "," << current->location.x << "} to {"
              << newNode->location.y << "," << newNode->location.x << "} with dir " << dirString << std::endl;
  }
  return traverse(dir, newNode, stepsRemaining - 1);
}

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
  std::cout << "Width: " << maxLineSize << " Height: " << lines.size() << std::endl;

  // Parse the grid.
  std::vector<std::vector<std::unique_ptr<Node>>> grid;
  std::vector<CubeFace> cubeFaces;
  int lastCubeWidth = 0;
  int lastRowIndex = 0;
  bool firstRow = true;

  int rowNum = 0;
  for (const auto& row : lines) {
    int firstIndex = 0;
    bool foundFirstIndex = false;
    if (row[0] != ' ') {
      foundFirstIndex = true;
    }
    std::vector<std::unique_ptr<Node>> rowContents;
    rowContents.resize(maxLineSize);
    for (int i = 0; i < row.size(); ++i) {
      if (row[i] != ' ') {
        if (!foundFirstIndex) {
          firstIndex = i;
          foundFirstIndex = true;
        }
        rowContents[i] = std::make_unique<Node>();
        auto node = rowContents[i].get();
        node->location.y = rowNum;
        node->location.x = i;
        if (row[i] == '#') {
          node->isWall = true;
        } else if (row[i] == '.') {
          node->isWall = false;
        } else {
          throw std::runtime_error("Unrecognized");
        }
      }
      if (row[i] == ' ' || (i == (row.size() - 1))) {
        if (foundFirstIndex) {
          int cubeWidth = i - firstIndex + 1;
          if (lastCubeWidth != cubeWidth) {
            if (firstRow) {
              firstRow = false;
            } else {
              cubeFaces.push_back(CubeFace{lastCubeWidth, rowNum - lastRowIndex, lastRowIndex, firstIndex});
              lastRowIndex = rowNum;
            }
            lastCubeWidth = cubeWidth;
          }
        }
      }
    }
    rowNum++;
    grid.push_back(std::move(rowContents));
    if (rowNum == lines.size()) {
      cubeFaces.push_back(CubeFace{lastCubeWidth, rowNum - lastRowIndex, lastRowIndex, firstIndex});
    }
  }

  // The cube faces things is NUTS. Going to manually encode.
  for (const auto& cubeFace : cubeFaces) {
    std::cout << cubeFace.width << " " << cubeFace.height << " " << cubeFace.row << " " << cubeFace.column << std::endl;
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

  // Manually stitch sides together.

  if (maxLineSize > 100) {
    // Side 1:
    for (int i = 0; i < 50; ++i) {
      Loc right{i, 50};
      Loc left{149 - i, 0};
      if (!grid[right.y][right.x]->left) {
        throw std::runtime_error("Except.");
      }
      if (!grid[left.y][left.x]->left) {
        throw std::runtime_error("Except.");
      }
      grid[right.y][right.x]->left = grid[left.y][left.x].get();
      grid[right.y][right.x]->leftTransition = Rotate::Reverse;
      grid[left.y][left.x]->left = grid[right.y][right.x].get();
      grid[left.y][left.x]->leftTransition = Rotate::Reverse;
    }
    // Side 2:
    for (int i = 0; i < 50; ++i) {
      Loc right{i + 50, 50};
      Loc left{100, i};
      if (!grid[right.y][right.x]->left) {
        throw std::runtime_error("Except.");
      }
      if (!grid[left.y][left.x]->up) {
        throw std::runtime_error("Except.");
      }
      grid[right.y][right.x]->left = grid[left.y][left.x].get();
      grid[right.y][right.x]->leftTransition = Rotate::Left;
      grid[left.y][left.x]->up = grid[right.y][right.x].get();
      grid[left.y][left.x]->upTransition = Rotate::Right;
    }
    // Side 3:
    for (int i = 0; i < 50; ++i) {
      Loc right{0, i + 50};
      Loc left{150 + i, 0};
      if (!grid[right.y][right.x]->up) {
        throw std::runtime_error("Except.");
      }
      if (!grid[left.y][left.x]->left) {
        throw std::runtime_error("Except.");
      }

      grid[right.y][right.x]->up = grid[left.y][left.x].get();
      grid[right.y][right.x]->upTransition = Rotate::Right;
      grid[left.y][left.x]->left = grid[right.y][right.x].get();
      grid[left.y][left.x]->leftTransition = Rotate::Left;
    }
    // Side 4:
    for (int i = 0; i < 50; ++i) {
      Loc right{0, i + 100};
      Loc left{199, i};

      if (!grid[right.y][right.x]->up) {
        throw std::runtime_error("Except.");
      }
      if (!grid[left.y][left.x]->down) {
        throw std::runtime_error("Except.");
      }
      grid[right.y][right.x]->up = grid[left.y][left.x].get();
      grid[left.y][left.x]->down = grid[right.y][right.x].get();
    }
    // Side 5:
    for (int i = 0; i < 50; ++i) {
      Loc right{149, i + 50};
      Loc left{150 + i, 49};

      if (!grid[right.y][right.x]->down) {
        throw std::runtime_error("Except.");
      }
      if (!grid[left.y][left.x]->right) {
        throw std::runtime_error("Except.");
      }
      grid[right.y][right.x]->down = grid[left.y][left.x].get();
      grid[right.y][right.x]->downTransition = Rotate::Right;
      grid[left.y][left.x]->right = grid[right.y][right.x].get();
      grid[left.y][left.x]->rightTransition = Rotate::Left;
    }
    // Side 6:
    for (int i = 0; i < 50; ++i) {
      Loc right{49, i + 100};
      Loc left{50 + i, 99};

      if (!grid[right.y][right.x]->down) {
        throw std::runtime_error("Except.");
      }
      if (!grid[left.y][left.x]->right) {
        throw std::runtime_error("Except.");
      }
      grid[right.y][right.x]->down = grid[left.y][left.x].get();
      grid[right.y][right.x]->downTransition = Rotate::Right;
      grid[left.y][left.x]->right = grid[right.y][right.x].get();
      grid[left.y][left.x]->rightTransition = Rotate::Left;
    }
    // Side 7:
    for (int i = 0; i < 50; ++i) {
      Loc right{i, 149};
      Loc left{149 - i, 99};

      if (!grid[right.y][right.x]->right) {
        throw std::runtime_error("Except.");
      }
      if (!grid[left.y][left.x]->right) {
        throw std::runtime_error("Except.");
      }

      grid[right.y][right.x]->right = grid[left.y][left.x].get();
      grid[right.y][right.x]->rightTransition = Rotate::Reverse;
      grid[left.y][left.x]->right = grid[right.y][right.x].get();
      grid[left.y][left.x]->rightTransition = Rotate::Reverse;
    }

    std::vector<Direction> dirs{Direction::Up, Direction::Right, Direction::Down, Direction::Left};
    for (const auto& dir : dirs) {
      std::vector<Loc> locs{Loc{10, 60}, Loc{10, 110}, Loc{60, 60}, Loc{110, 10}, Loc{110, 60}, Loc{160, 10}};
      for (const auto& loc : locs) {
        std::string dirString = [&]() {
          switch (dir) {
          case Direction::Left:
            return "left";
          case Direction::Right:
            return "right";
          case Direction::Up:
            return "up";
          case Direction::Down:
            return "down";
          }
        }();
        std::cout << "Traversing loc {" << loc.y << "," << loc.x << "} with dir " << dirString << std::endl;
        Direction newDir = dir;
        auto newLoc = traverse(newDir, grid[loc.y][loc.x].get(), 200);
        if (newLoc.y != loc.y || newLoc.x != loc.x || newDir != dir) {
          std::cout << "row: " << loc.y << " vs new row: " << newLoc.y << " column: " << loc.x
                    << " vs new column: " << newLoc.x << std::endl;
          throw std::runtime_error("Mismatch.");
        }
      }
    }

  } else {
    // Side 1:
    for (int i = 0; i < 4; ++i) {
      Loc right{i, 8};
      Loc left{4, 4 + i};
      if (!grid[right.y][right.x]->left) {
        throw std::runtime_error("Except.");
      }
      if (!grid[left.y][left.x]->up) {
        throw std::runtime_error("Except.");
      }
      grid[right.y][right.x]->left = grid[left.y][left.x].get();
      grid[right.y][right.x]->leftTransition = Rotate::Left;
      grid[left.y][left.x]->up = grid[right.y][right.x].get();
      grid[left.y][left.x]->upTransition = Rotate::Right;
    }
    // Side 2:
    for (int i = 0; i < 4; ++i) {
      Loc right{0, 8 + i};
      Loc left{4, 3 - i};
      if (!grid[right.y][right.x]->up) {
        throw std::runtime_error("Except.");
      }
      if (!grid[left.y][left.x]->up) {
        throw std::runtime_error("Except.");
      }
      grid[right.y][right.x]->up = grid[left.y][left.x].get();
      grid[right.y][right.x]->upTransition = Rotate::Reverse;
      grid[left.y][left.x]->up = grid[right.y][right.x].get();
      grid[left.y][left.x]->upTransition = Rotate::Reverse;
    }
    // Side 3:
    for (int i = 0; i < 4; ++i) {
      Loc right{i, 11};
      Loc left{11 - i, 15};
      if (!grid[right.y][right.x]->right) {
        throw std::runtime_error("Except.");
      }
      if (!grid[left.y][left.x]->right) {
        throw std::runtime_error("Except.");
      }
      grid[right.y][right.x]->right = grid[left.y][left.x].get();
      grid[right.y][right.x]->rightTransition = Rotate::Reverse;
      grid[left.y][left.x]->right = grid[right.y][right.x].get();
      grid[left.y][left.x]->rightTransition = Rotate::Reverse;
    }
    // Side 4:
    for (int i = 0; i < 4; ++i) {
      Loc right{4 + i, 11};
      Loc left{8, 15 - i};
      if (!grid[right.y][right.x]->right) {
        throw std::runtime_error("Except.");
      }
      if (!grid[left.y][left.x]->up) {
        throw std::runtime_error("Except.");
      }
      grid[right.y][right.x]->right = grid[left.y][left.x].get();
      grid[right.y][right.x]->rightTransition = Rotate::Right;
      grid[left.y][left.x]->up = grid[right.y][right.x].get();
      grid[left.y][left.x]->upTransition = Rotate::Left;
    }
    // Side 5:
    for (int i = 0; i < 4; ++i) {
      Loc right{11, 12 + i};
      Loc left{7 - i, 0};
      if (!grid[right.y][right.x]->down) {
        throw std::runtime_error("Except.");
      }
      if (!grid[left.y][left.x]->left) {
        throw std::runtime_error("Except.");
      }
      grid[right.y][right.x]->down = grid[left.y][left.x].get();
      grid[right.y][right.x]->downTransition = Rotate::Left;
      grid[left.y][left.x]->left = grid[right.y][right.x].get();
      grid[left.y][left.x]->leftTransition = Rotate::Right;
    }
    // Side 6:
    for (int i = 0; i < 4; ++i) {
      Loc right{11, 8 + i};
      Loc left{7, 3 - i};
      if (!grid[right.y][right.x]->down) {
        throw std::runtime_error("Except.");
      }
      if (!grid[left.y][left.x]->down) {
        throw std::runtime_error("Except.");
      }
      grid[right.y][right.x]->down = grid[left.y][left.x].get();
      grid[right.y][right.x]->downTransition = Rotate::Reverse;
      grid[left.y][left.x]->down = grid[right.y][right.x].get();
      grid[left.y][left.x]->downTransition = Rotate::Reverse;
    }
    // Side 7:
    for (int i = 0; i < 4; ++i) {
      Loc right{8 + i, 8};
      Loc left{7, 7 - i};
      if (!grid[right.y][right.x]->down) {
        throw std::runtime_error("Except.");
      }
      if (!grid[left.y][left.x]->down) {
        throw std::runtime_error("Except.");
      }
      grid[right.y][right.x]->left = grid[left.y][left.x].get();
      grid[right.y][right.x]->leftTransition = Rotate::Right;
      grid[left.y][left.x]->down = grid[right.y][right.x].get();
      grid[left.y][left.x]->downTransition = Rotate::Left;
    }
    std::vector<Direction> dirs{Direction::Up, Direction::Right, Direction::Down, Direction::Left};
    for (const auto& dir : dirs) {
      std::vector<Loc> locs{Loc{1, 9}, Loc{5, 9}, Loc{5, 6}, Loc{5, 2}, Loc{9, 9}, Loc{9, 13}};
      for (const auto& loc : locs) {
        Direction newDir = dir;
        auto newLoc = traverse(newDir, grid[loc.y][loc.x].get(), 16);
        if (newLoc.y != loc.y || newLoc.x != loc.x || newDir != dir) {
          std::cout << "row: " << loc.y << " vs new row: " << newLoc.y << " column: " << loc.x
                    << " vs new column: " << newLoc.x << std::endl;
          throw std::runtime_error("Mismatch.");
        }
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
        // std::cout << "Y: " << current->location.y << " X: " << current->location.x << " i: " << i << std::endl;
        bool done = false;
        switch (dir) {
        case Direction::Up:
          if (current->up->isWall) {
            // std::cout << "up wall" << std::endl;
            done = true;
          } else {
            if (current->upTransition.has_value()) {
              switch (current->upTransition.value()) {
              case Rotate::Right:
                dir = Direction::Right;
                break;
              case Rotate::Left:
                dir = Direction::Left;
                break;
              case Rotate::Reverse:
                dir = Direction::Down;
                break;
              }
            }
            current = current->up;
          }
          break;
        case Direction::Right:
          if (current->right->isWall) {
            done = true;
            // std::cout << "right wall" << std::endl;
          } else {
            if (current->rightTransition.has_value()) {
              switch (current->rightTransition.value()) {
              case Rotate::Right:
                dir = Direction::Down;
                break;
              case Rotate::Left:
                dir = Direction::Up;
                break;
              case Rotate::Reverse:
                dir = Direction::Left;
                break;
              }
            }
            current = current->right;
          }
          break;
        case Direction::Down:
          if (current->down->isWall) {
            // std::cout << "down wall" << std::endl;
            done = true;
          } else {
            if (current->downTransition.has_value()) {
              switch (current->downTransition.value()) {
              case Rotate::Right:
                dir = Direction::Left;
                break;
              case Rotate::Left:
                dir = Direction::Right;
                break;
              case Rotate::Reverse:
                dir = Direction::Up;
                break;
              }
            }
            current = current->down;
          }
          break;
        case Direction::Left:
          if (current->left->isWall) {
            // std::cout << "left wall" << std::endl;
            done = true;
          } else {
            if (current->leftTransition.has_value()) {
              switch (current->leftTransition.value()) {
              case Rotate::Right:
                dir = Direction::Up;
                break;
              case Rotate::Left:
                dir = Direction::Down;
                break;
              case Rotate::Reverse:
                dir = Direction::Right;
                break;
              }
            }
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
  sum = sum + 1000 * (current->location.y + 1);
  sum = sum + 4 * (current->location.x + 1);
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
  std::cout << "Row: " << current->location.y + 1 << " Column: " << current->location.x + 1 << std::endl;
  std::cout << "Sum: " << sum << std::endl;

  // 15307 too large.
  return 0;
}
