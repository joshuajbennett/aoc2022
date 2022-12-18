#include <cmath>
#include <deque>
#include <functional>
#include <iostream>
#include <optional>
#include <queue>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

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
  int z;
  Loc operator+(const Loc& b) {
    Loc loc;
    loc.x = this->x + b.x;
    loc.y = this->y + b.y;
    loc.z = this->z + b.z;
    return loc;
  }
};

int main(int argc, char** argv) {
  std::string line;
  std::vector<Loc> locs;
  while (std::getline(std::cin, line)) {
    auto elements = split(line, ',');
    locs.push_back(Loc{std::stoi(elements[0]), std::stoi(elements[1]), std::stoi(elements[2])});
  }
  int maxX = std::numeric_limits<int>::lowest();
  int maxY = std::numeric_limits<int>::lowest();
  int maxZ = std::numeric_limits<int>::lowest();
  int minX = std::numeric_limits<int>::max();
  int minY = std::numeric_limits<int>::max();
  int minZ = std::numeric_limits<int>::max();
  for (const auto& loc : locs) {
    if (loc.x > maxX) {
      maxX = loc.x;
    }
    if (loc.y > maxY) {
      maxY = loc.y;
    }
    if (loc.z > maxZ) {
      maxZ = loc.z;
    }
    if (loc.x < minX) {
      minX = loc.x;
    }
    if (loc.y < minY) {
      minY = loc.y;
    }
    if (loc.z < minZ) {
      minZ = loc.z;
    }
  }
  int width = maxX - minX + 1;  // X
  int depth = maxY - minY + 1;  // Y
  int height = maxZ - minZ + 1; // Z

  std::vector<bool> row(width, false);
  std::vector<std::vector<bool>> plane(depth, row);
  std::vector<std::vector<std::vector<bool>>> cube(height, plane);

  for (const auto& loc : locs) {
    cube[loc.z - minZ][loc.y - minY][loc.x - minX] = true;
  }
  std::vector<Loc> directions{Loc{1, 0, 0}, Loc{-1, 0, 0}, Loc{0, 1, 0}, Loc{0, -1, 0}, Loc{0, 0, 1}, Loc{0, 0, -1}};
  int surfaceArea = 0;
  for (int z = minZ; z <= maxZ; z++) {
    for (int y = minY; y <= maxY; y++) {
      for (int x = minX; x <= maxX; x++) {
        Loc current{x, y, z};
        if (cube[z - minZ][y - minY][x - minX]) {
          for (const auto& dir : directions) {
            auto dirLoc = current + dir;
            if (dirLoc.x < minX || dirLoc.x > maxX || dirLoc.y < minY || dirLoc.y > maxY || dirLoc.z < minZ ||
                dirLoc.z > maxZ) {
              surfaceArea++;
            } else {
              if (!cube[dirLoc.z - minZ][dirLoc.y - minY][dirLoc.x - minX]) {
                surfaceArea++;
              }
            }
          }
        }
      }
    }
  }
  std::cout << "Surface area: " << surfaceArea << std::endl;

  return 0;
}
