#include <iostream>
#include <sstream>
#include <string>
#include <vector>

// To run build the code and then do the following command:
// ```
// cat ../day9/in.txt | day9/day9part1
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

Loc updateTail(const Loc& headLoc, const Loc& tailLoc) {
  int xDist = headLoc.x - tailLoc.x;
  int yDist = headLoc.y - tailLoc.y;
  // Flat in x
  if (xDist == 0) {
    if (yDist > 1) {
      return Loc{tailLoc.x, tailLoc.y + 1};
    } else if (yDist < -1) {
      return Loc{tailLoc.x, tailLoc.y - 1};
    }
    // Flat in y
  } else if (yDist == 0) {
    if (xDist > 1) {
      return Loc{tailLoc.x + 1, tailLoc.y};
    } else if (xDist < -1) {
      return Loc{tailLoc.x - 1, tailLoc.y};
    }
    // Within the diagonal or adjacent.
  } else if (yDist <= 1 && yDist >= -1 && xDist <= 1 && xDist >= -1) {
    return Loc{tailLoc.x, tailLoc.y};
  } else if (yDist > 1) {
    if (xDist > 0) {
      return Loc{tailLoc.x + 1, tailLoc.y + 1};
    } else if (xDist < 0) {
      return Loc{tailLoc.x - 1, tailLoc.y + 1};
    } else {
      return Loc{tailLoc.x, tailLoc.y + 1};
    }
  } else if (yDist < -1) {
    if (xDist > 0) {
      return Loc{tailLoc.x + 1, tailLoc.y - 1};
    } else if (xDist < 0) {
      return Loc{tailLoc.x - 1, tailLoc.y - 1};
    } else {
      return Loc{tailLoc.x, tailLoc.y - 1};
    }
  } else if (xDist > 1) {
    if (yDist > 0) {
      return Loc{tailLoc.x + 1, tailLoc.y + 1};
    } else if (yDist < 0) {
      return Loc{tailLoc.x + 1, tailLoc.y - 1};
    } else {
      return Loc{tailLoc.x + 1, tailLoc.y};
    }
  } else if (xDist < -1) {
    if (yDist > 0) {
      return Loc{tailLoc.x - 1, tailLoc.y + 1};
    } else if (yDist < 0) {
      return Loc{tailLoc.x - 1, tailLoc.y - 1};
    } else {
      return Loc{tailLoc.x - 1, tailLoc.y};
    }
  }
  return tailLoc;
}

void updateRope(const Loc& headLoc, std::vector<Loc>& intermediate, Loc& tail) {
  intermediate[0] = updateTail(headLoc, intermediate[0]);
  for (int i = 1; i < intermediate.size(); ++i) {
    intermediate[i] = updateTail(intermediate[i - 1], intermediate[i]);
  }
  tail = updateTail(intermediate.back(), tail);
}

int main(int argc, char** argv) {
  std::vector<Loc> tailLocations;
  Loc currentHeadLoc{0, 0};
  std::vector<Loc> intermediate(8, Loc{0, 0});
  Loc currentTailLoc{0, 0};
  tailLocations.push_back(currentTailLoc);
  std::string line;
  while (std::getline(std::cin, line)) {
    auto elements = split(line, ' ');
    if (elements.size() != 2) {
      throw std::runtime_error("Row didn't include two elements.");
    }
    auto steps = std::stoi(elements[1]);
    for (int i = 0; i < steps; ++i) {
      if (elements[0] == "R") {
        currentHeadLoc.x++;
        updateRope(currentHeadLoc, intermediate, currentTailLoc);
        tailLocations.push_back(currentTailLoc);
      } else if (elements[0] == "L") {
        currentHeadLoc.x--;
        updateRope(currentHeadLoc, intermediate, currentTailLoc);
        tailLocations.push_back(currentTailLoc);
      } else if (elements[0] == "U") {
        currentHeadLoc.y++;
        updateRope(currentHeadLoc, intermediate, currentTailLoc);
        tailLocations.push_back(currentTailLoc);
      } else if (elements[0] == "D") {
        currentHeadLoc.y--;
        updateRope(currentHeadLoc, intermediate, currentTailLoc);
        tailLocations.push_back(currentTailLoc);
      }
    }
  }
  int minX = 0;
  int maxX = 0;
  int minY = 0;
  int maxY = 0;
  for (const auto& loc : tailLocations) {
    if (loc.x > maxX) {
      maxX = loc.x;
    }
    if (loc.x < minX) {
      minX = loc.x;
    }
    if (loc.y > maxY) {
      maxY = loc.y;
    }
    if (loc.y < minY) {
      minY = loc.y;
    }
  }
  int width = maxX - minX + 1;
  int height = maxY - minY + 1;
  std::vector<bool> row(width, false);
  std::vector<std::vector<bool>> grid(height, row);
  for (const auto& loc : tailLocations) {
    grid[loc.y - minY][loc.x - minX] = true;
  }

  int numVisits = 0;
  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      if (grid[y][x]) {
        numVisits++;
      }
    }
  }

  for (int y = height - 1; y >= 0; --y) {
    for (int x = 0; x < width; ++x) {
      if (grid[y][x]) {
        std::cout << "#";
      } else {
        std::cout << ".";
      }
    }
    std::cout << std::endl;
  }

  std::cout << "Number of visited cells: " << numVisits << std::endl;

  return 0;
}
