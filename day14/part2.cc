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
// cat ../day13/in.txt | day13/day13part1
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

struct Loc {
  int x;
  int y;
};

int main(int argc, char** argv) {
  std::vector<std::vector<Loc>> segments;
  std::string line;
  while (std::getline(std::cin, line)) {
    std::vector<Loc> row;
    auto firstSplit = split(line, ' ');
    for (int i = 0; i < firstSplit.size(); i += 2) {
      auto secondSplit = split(firstSplit[i], ',');
      row.push_back(Loc{std::stoi(secondSplit[0]), std::stoi(secondSplit[1])});
    }
    segments.push_back(row);
  }
  Loc sandOrigin{500, 0};
  int minX = sandOrigin.x;
  int maxX = sandOrigin.x;
  int minY = sandOrigin.y;
  int maxY = sandOrigin.y;

  for (const auto& row : segments) {
    for (const auto& elem : row) {
      if (elem.x < minX) {
        minX = elem.x;
      }
      if (elem.x > maxX) {
        maxX = elem.x;
      }
      if (elem.y < minY) {
        minY = elem.y;
      }
      if (elem.y > maxY) {
        maxY = elem.y;
      }
    }
  };
  maxY += 2;
  int currentHeight = maxY - minY;
  minX -= currentHeight;
  maxX += currentHeight;

  std::cout << "MinX: " << minX << " MaxX: " << maxX << " MinY: " << minY << " MaxY: " << maxY << std::endl;

  int width = maxX - minX + 1;
  int height = maxY - minY + 1;
  std::vector<bool> row(width, false);
  std::vector<std::vector<bool>> occupancy(height, row);

  for (const auto& row : segments) {
    for (int i = 0; i < row.size() - 1; ++i) {
      const auto& current = row[i];
      const auto& next = row[i + 1];
      int xDist = next.x - current.x;
      int yDist = next.y - current.y;
      if (xDist != 0) {
        int direction = 1;
        if (xDist < 0) {
          direction = -1;
        }
        for (int i = 0; i <= std::abs(xDist); ++i) {
          occupancy[current.y - minY][current.x + i * direction - minX] = true;
        }
      } else if (yDist != 0) {
        int direction = 1;
        if (yDist < 0) {
          direction = -1;
        }
        for (int i = 0; i <= std::abs(yDist); ++i) {
          occupancy[current.y + i * direction - minY][current.x - minX] = true;
        }
      }
    }
  }

  for (int x = minX; x <= maxX; ++x) {
    occupancy[maxY - minY][x - minX] = true;
  }

  for (int y = minY; y <= maxY; ++y) {
    for (int x = minX; x <= maxX; ++x) {
      if (occupancy[y - minY][x - minX]) {
        std::cout << "#";
      } else {
        std::cout << ".";
      }
    }
    std::cout << std::endl;
  }

  std::vector<std::vector<bool>> empty{occupancy};

  bool moreSand = true;
  int amountOfSand = 0;
  while (moreSand) {
    amountOfSand++;
    bool keepSimulating = true;
    Loc sandLoc = sandOrigin;
    while (keepSimulating) {
      if (sandLoc.y + 1 > maxY) {
        keepSimulating = false;
        moreSand = false;
      } else if (!occupancy[sandLoc.y + 1 - minY][sandLoc.x - minX]) {
        sandLoc.y++;
      } else if (sandLoc.x - 1 >= minX && !occupancy[sandLoc.y + 1 - minY][sandLoc.x - 1 - minX]) {
        sandLoc.y++;
        sandLoc.x--;
      } else if (sandLoc.x + 1 <= maxX && !occupancy[sandLoc.y + 1 - minY][sandLoc.x + 1 - minX]) {
        sandLoc.y++;
        sandLoc.x++;
      } else if (sandLoc.x < minX) {
        keepSimulating = false;
        moreSand = false;
      } else if (sandLoc.x > maxX) {
        keepSimulating = false;
        moreSand = false;
      } else {
        // Settled into place.
        occupancy[sandLoc.y - minY][sandLoc.x - minX] = true;
        keepSimulating = false;
        if (sandLoc.y == sandOrigin.y && sandLoc.x == sandOrigin.x) {
          moreSand = false;
        }
      }
    }
  }

  std::cout << std::endl;
  for (int y = minY; y <= maxY; ++y) {
    for (int x = minX; x <= maxX; ++x) {
      if (empty[y - minY][x - minX]) {
        std::cout << "#";
      } else if (occupancy[y - minY][x - minX]) {
        std::cout << "o";
      } else {
        std::cout << ".";
      }
    }
    std::cout << std::endl;
  }

  std::cout << "Sent " << amountOfSand << " sand kernels." << std::endl;

  return 0;
}
