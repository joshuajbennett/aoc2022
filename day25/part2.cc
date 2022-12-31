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

enum class Dir { North, East, South, West };

struct Loc {
  int x = 0;
  int y = 0;
  bool operator==(const Loc& rhs) const { return (x == rhs.x && y == rhs.y); }
};

struct State {
  bool isNorth = false;
  bool isEast = false;
  bool isSouth = false;
  bool isWest = false;
  bool isOccupied = false;
  int numOccupied = 0;
  std::vector<Loc> neighbors;
  bool visited = false;
};

struct NextNode {
  Loc location;
  int timeTraveled{0};
  int distFromDestination{0};
  bool operator<(const NextNode& rhs) const { return timeTraveled > rhs.timeTraveled; }
};

int wrapVal(int val, int width) {
  if (val < 0) {
    return val + width;
  }
  if (val >= width) {
    return val - width;
  }
  return val;
}

std::vector<std::vector<State>> getPropagatedGrid(const std::vector<std::vector<State>>& grid) {
  int width = grid.front().size();
  int height = grid.size();
  std::vector<State> emptyRow(width, State{});
  std::vector<std::vector<State>> newGrid(height, emptyRow);
  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      if (grid[y][x].isNorth) {
        int wrappedY = wrapVal(y - 1, height);
        newGrid[wrappedY][x].isNorth = true;
        newGrid[wrappedY][x].isOccupied = true;
        newGrid[wrappedY][x].numOccupied++;
      }
      if (grid[y][x].isEast) {
        int wrappedX = wrapVal(x + 1, width);
        newGrid[y][wrappedX].isEast = true;
        newGrid[y][wrappedX].isOccupied = true;
        newGrid[y][wrappedX].numOccupied++;
      }
      if (grid[y][x].isSouth) {
        int wrappedY = wrapVal(y + 1, height);
        newGrid[wrappedY][x].isSouth = true;
        newGrid[wrappedY][x].isOccupied = true;
        newGrid[wrappedY][x].numOccupied++;
      }
      if (grid[y][x].isWest) {
        int wrappedX = wrapVal(x - 1, width);
        newGrid[y][wrappedX].isWest = true;
        newGrid[y][wrappedX].isOccupied = true;
        newGrid[y][wrappedX].numOccupied++;
      }
    }
  }
  return newGrid;
}

void renderGrid(const std::vector<std::vector<State>>& grid) {
  for (int y = 0; y < grid.size(); ++y) {
    std::cout << "#";
    for (int x = 0; x < grid.front().size(); ++x) {
      if (!grid[y][x].isOccupied) {
        std::cout << ".";
      } else if (grid[y][x].numOccupied > 1) {
        std::cout << grid[y][x].numOccupied;
      } else if (grid[y][x].isNorth) {
        std::cout << "^";
      } else if (grid[y][x].isWest) {
        std::cout << "<";
      } else if (grid[y][x].isEast) {
        std::cout << ">";
      } else if (grid[y][x].isSouth) {
        std::cout << "v";
      }
    }
    std::cout << "#";
    std::cout << std::endl;
  }
  std::cout << std::endl;
}

bool inBounds(const Loc& loc, const int width, const int height) {
  return (loc.x >= 0 && loc.x < width && loc.y >= 0 && loc.y < height);
}

int search(const Loc& initialLoc, const Loc& finalLoc, const int maxNumPropagations, const int initialTime,
           bool searchBelow, std::vector<std::vector<std::vector<State>>> propagatedGrids) {
  int width = propagatedGrids.front().front().size();
  int height = propagatedGrids.front().size();
  for (int i = 0; i < propagatedGrids.size() - 1; ++i) {
    auto& currentGrid = propagatedGrids[i];
    const auto& nextGrid = propagatedGrids[i + 1];
    for (int y = 0; y < height; ++y) {
      for (int x = 0; x < width; ++x) {
        if (!currentGrid[y][x].isOccupied) {
          std::vector<Loc> toCheck{Loc{x, y}, Loc{x + 1, y}, Loc{x, y + 1}, Loc{x - 1, y}, Loc{x, y - 1}};
          for (const auto& check : toCheck) {
            if (check == finalLoc) {
              currentGrid[y][x].neighbors.push_back(check);
            } else if (inBounds(check, width, height)) {
              if (!nextGrid[check.y][check.x].isOccupied) {
                currentGrid[y][x].neighbors.push_back(check);
              }
            }
          }
        }
      }
    }
  }

  std::queue<NextNode> toVisit;

  int distFromFinal = std::abs(initialLoc.x - finalLoc.x) + std::abs(initialLoc.y - finalLoc.y);
  int shortestDist = distFromFinal;
  toVisit.push(NextNode{initialLoc, initialTime, distFromFinal});

  std::cout << "Initial: {" << initialLoc.x << "," << initialLoc.y << "}" << std::endl;
  std::cout << "Final: {" << finalLoc.x << "," << finalLoc.y << "}" << std::endl;
  int numIter = 0;
  while (!toVisit.empty()) {
    auto visiting = toVisit.front();
    // auto visiting = toVisit.top();
    toVisit.pop();

    const auto& currentGrid = propagatedGrids[visiting.timeTraveled];
    auto& nextGrid = propagatedGrids[visiting.timeTraveled + 1];
    if (visiting.location == initialLoc) {
      if (visiting.timeTraveled < maxNumPropagations - 2) {
        int distFromFinal = std::abs(visiting.location.x - finalLoc.x) + std::abs(visiting.location.y - finalLoc.y);
        toVisit.push(NextNode{visiting.location, visiting.timeTraveled + 1, distFromFinal});
        if (searchBelow) {
          Loc below{visiting.location.x, visiting.location.y + 1};
          if (!nextGrid[below.y][below.x].isOccupied) {
            toVisit.push(NextNode{below, visiting.timeTraveled + 1, distFromFinal});
          }
        } else {
          Loc above{visiting.location.x, visiting.location.y - 1};
          if (!nextGrid[above.y][above.x].isOccupied) {
            toVisit.push(NextNode{above, visiting.timeTraveled + 1, distFromFinal});
          }
        }
      }
    } else {
      for (const auto& neighbor : currentGrid[visiting.location.y][visiting.location.x].neighbors) {
        if (neighbor == finalLoc) {
          return visiting.timeTraveled + 1;
        } else if (neighbor == initialLoc) {
          continue;
        } else {
          if (visiting.timeTraveled < maxNumPropagations - 2) {
            int distFromFinal = std::abs(neighbor.x - finalLoc.x) + std::abs(neighbor.y - finalLoc.y);
            if (distFromFinal < shortestDist) {
              shortestDist = distFromFinal;
              // std::cout << "New shortest dist at time " << visiting.timeTraveled + 1 << " and loc {" << neighbor.x
              //           << "," << neighbor.y << "}" << std::endl;
            }
            if (!nextGrid[neighbor.y][neighbor.x].visited) {
              toVisit.push(NextNode{neighbor, visiting.timeTraveled + 1, distFromFinal});
              nextGrid[neighbor.y][neighbor.x].visited = true;
            }
          }
        }
      }
    }
  }
  throw std::runtime_error("Failed to find a path.");
}

int main(int argc, char** argv) {
  std::string line;
  Loc initialLoc;
  Loc finalLoc;

  std::vector<std::string> lines;
  while (std::getline(std::cin, line)) {
    lines.push_back(line);
  }
  const auto& firstLine = lines.front();
  for (int i = 1; i < firstLine.size() - 1; ++i) {
    if (firstLine[i] == '.') {
      initialLoc = Loc{i - 1, -1};
      break;
    }
  }
  const auto& lastLine = lines.back();
  for (int i = 1; i < lastLine.size() - 1; ++i) {
    if (lastLine[i] == '.') {
      finalLoc = Loc{i - 1, static_cast<int>(lines.size() - 2)};
      break;
    }
  }

  std::vector<std::vector<State>> grid;
  for (int j = 1; j < lines.size() - 1; ++j) {
    const auto& row = lines[j];
    std::vector<State> stateRow(row.size() - 2, State{});
    for (int i = 1; i < row.size() - 1; ++i) {
      if (row[i] == '<') {
        stateRow[i - 1].isWest = true;
        stateRow[i - 1].isOccupied = true;
      }
      if (row[i] == 'v') {
        stateRow[i - 1].isSouth = true;
        stateRow[i - 1].isOccupied = true;
      }
      if (row[i] == '>') {
        stateRow[i - 1].isEast = true;
        stateRow[i - 1].isOccupied = true;
      }
      if (row[i] == '^') {
        stateRow[i - 1].isNorth = true;
        stateRow[i - 1].isOccupied = true;
      }
    }
    grid.push_back(stateRow);
  }

  int width = grid.front().size();
  int height = grid.size();

  std::vector<std::vector<std::vector<State>>> propagatedGrids;
  // renderGrid(grid);
  propagatedGrids.push_back(std::move(grid));
  int maxNumPropagations = 1500;
  for (int i = 0; i < maxNumPropagations; ++i) {
    propagatedGrids.push_back(getPropagatedGrid(propagatedGrids.back()));
    // renderGrid(propagatedGrids.back());
    // std::this_thread::sleep_for(std::chrono::seconds(1));
  }
  std::cout << "Width: " << width << " Height: " << height << std::endl;

  auto numSteps = search(initialLoc, finalLoc, maxNumPropagations, 0, true, propagatedGrids);
  std::cout << "Num steps: " << numSteps << std::endl;
  auto nextNumSteps = search(finalLoc, initialLoc, maxNumPropagations, numSteps, false, propagatedGrids);
  std::cout << "Next num steps: " << nextNumSteps << std::endl;
  auto finalNumSteps = search(initialLoc, finalLoc, maxNumPropagations, nextNumSteps, true, propagatedGrids);
  std::cout << "Took " << finalNumSteps << " to reach the end." << std::endl;
  // 843 too small.
  return 0;
}
