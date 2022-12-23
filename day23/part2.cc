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
};

struct Decision {
  Loc location;
  Dir direction;
  Loc proposedLoc;
};

void render(const std::vector<std::vector<bool>>& grid) {

  // Find the minimum rectangle.
  int minX = std::numeric_limits<int>::max();
  int maxX = std::numeric_limits<int>::lowest();
  int minY = std::numeric_limits<int>::max();
  int maxY = std::numeric_limits<int>::lowest();

  for (int y = 1; y < grid.size() - 1; ++y) {
    for (int x = 1; x < grid.back().size() - 1; ++x) {
      if (grid[y][x]) {
        if (x < minX) {
          minX = x;
        }
        if (x > maxX) {
          maxX = x;
        }
        if (y < minY) {
          minY = y;
        }
        if (y > maxY) {
          maxY = y;
        }
      }
    }
  }
  for (int y = minY; y <= maxY; y = y + 2) {
    for (int x = minX; x <= maxX; x = x + 2) {
      if (grid[y][x]) {
        if (grid[y + 1][x]) {
          if (grid[y][x + 1]) {
            if (grid[y + 1][x + 1]) {
              std::cout << "▓";
            } else {
              std::cout << "▛";
            }
          } else {
            if (grid[y + 1][x + 1]) {
              std::cout << "▙";
            } else {
              std::cout << "▌";
            }
          }
        } else {
          if (grid[y][x + 1]) {
            if (grid[y + 1][x + 1]) {
              std::cout << "▜";
            } else {
              std::cout << "▀";
            }
          } else {
            if (grid[y + 1][x + 1]) {
              std::cout << "▚";
            } else {
              std::cout << "▘";
            }
          }
        }
      } else {
        if (grid[y + 1][x]) {
          if (grid[y][x + 1]) {
            if (grid[y + 1][x + 1]) {
              std::cout << "▟";
            } else {
              std::cout << "▞";
            }
          } else {
            if (grid[y + 1][x + 1]) {
              std::cout << "▄";
            } else {
              std::cout << "▖";
            }
          }
        } else {
          if (grid[y][x + 1]) {
            if (grid[y + 1][x + 1]) {
              std::cout << "▐";
            } else {
              std::cout << "▝";
            }
          } else {
            if (grid[y + 1][x + 1]) {
              std::cout << "▗";
            } else {
              std::cout << " ";
            }
          }
        }
      }
    }
    std::cout << std::endl;
  }
}

int main(int argc, char** argv) {
  std::string line;
  int buffer = 200;
  std::vector<std::vector<bool>> grid;
  while (std::getline(std::cin, line)) {
    std::vector<bool> row(line.size() + 2 * buffer, false);
    for (int i = 0; i < line.size(); ++i)
      if (line[i] == '#') {
        row[buffer + i] = true;
      }
    grid.push_back(row);
  }
  int width = grid.back().size();
  std::vector<bool> bufferRow(grid.back().size(), false);
  for (int i = 0; i < buffer; ++i) {
    grid.insert(grid.begin(), bufferRow);
  }
  for (int i = 0; i < buffer; ++i) {
    grid.push_back(bufferRow);
  }
  std::deque<Dir> directions{Dir::North, Dir::South, Dir::West, Dir::East};
  for (int round = 0; round < 2000; round++) {
    render(grid);
    std::cout << std::endl;
    // Consider decisions.
    std::vector<Decision> decisions;
    std::vector<int> proposedRow(width, 0);
    std::vector<std::vector<int>> proposedGrid(grid.size(), proposedRow);
    for (int y = 1; y < grid.size() - 1; ++y) {
      for (int x = 1; x < width - 1; ++x) {
        if (grid[y][x] == true) {
          bool foundDir = false;

          if (!grid[y - 1][x - 1] && !grid[y - 1][x] && !grid[y - 1][x + 1] && !grid[y + 1][x - 1] && !grid[y + 1][x] &&
              !grid[y + 1][x + 1] && !grid[y][x - 1] && !grid[y][x + 1]) {
            proposedGrid[y][x]++;
            continue;
          }

          for (const auto& direction : directions) {
            switch (direction) {
            case Dir::North:
              if (!grid[y - 1][x - 1] && !grid[y - 1][x] && !grid[y - 1][x + 1]) {
                decisions.push_back(Decision{Loc{x, y}, Dir::North, Loc{x, y - 1}});
                proposedGrid[y - 1][x]++;
                foundDir = true;
              }
              break;
            case Dir::East:
              if (!grid[y - 1][x + 1] && !grid[y][x + 1] && !grid[y + 1][x + 1]) {
                decisions.push_back(Decision{Loc{x, y}, Dir::North, Loc{x + 1, y}});
                proposedGrid[y][x + 1]++;
                foundDir = true;
              }
              break;
            case Dir::South:
              if (!grid[y + 1][x - 1] && !grid[y + 1][x] && !grid[y + 1][x + 1]) {
                decisions.push_back(Decision{Loc{x, y}, Dir::North, Loc{x, y + 1}});
                proposedGrid[y + 1][x]++;
                foundDir = true;
              }
              break;
            case Dir::West:
              if (!grid[y - 1][x - 1] && !grid[y][x - 1] && !grid[y + 1][x - 1]) {
                decisions.push_back(Decision{Loc{x, y}, Dir::North, Loc{x - 1, y}});
                proposedGrid[y][x - 1]++;
                foundDir = true;
              }
              break;
            }
            if (foundDir) {
              break;
            }
          }
          if (!foundDir) {
            proposedGrid[y][x]++;
          }
        }
      }
    }
    if (decisions.size() == 0) {
      std::cout << "No elves moved: " << round + 1 << std::endl;
      return 0;
    }
    // Simultaneously move.
    for (const auto& decision : decisions) {
      const auto& currentLoc = decision.location;
      const auto& loc = decision.proposedLoc;
      if (proposedGrid[loc.y][loc.x] <= 1) {
        grid[currentLoc.y][currentLoc.x] = false;
        grid[loc.y][loc.x] = true;
      }
    }
    // Move first direction to back of deque.
    directions.push_back(directions.front());
    directions.pop_front();
  }
  // Find the minimum rectangle.
  int minX = std::numeric_limits<int>::max();
  int maxX = std::numeric_limits<int>::lowest();
  int minY = std::numeric_limits<int>::max();
  int maxY = std::numeric_limits<int>::lowest();

  for (int y = 1; y < grid.size() - 1; ++y) {
    for (int x = 1; x < width - 1; ++x) {
      if (grid[y][x]) {
        if (x < minX) {
          minX = x;
        }
        if (x > maxX) {
          maxX = x;
        }
        if (y < minY) {
          minY = y;
        }
        if (y > maxY) {
          maxY = y;
        }
      }
    }
  }
  int numOpen = 0;
  for (int y = minY; y <= maxY; ++y) {
    for (int x = minX; x <= maxX; ++x) {
      if (!grid[y][x]) {
        numOpen++;
      }
    }
  }
  std::cout << "Number open: " << numOpen << std::endl;

  return 0;
}
