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

struct SearchCube {
  std::vector<std::vector<std::vector<bool>>> cube;
  int width;
  int depth;
  int height;
  std::optional<long long> toId(const Loc& loc) {
    if (loc.x >= width || loc.x < 0) {
      return std::nullopt;
    }
    if (loc.y >= depth || loc.y < 0) {
      return std::nullopt;
    }
    if (loc.z >= height || loc.z < 0) {
      return std::nullopt;
    }
    return loc.x + loc.y * width + loc.z * depth * width;
  }
  std::optional<Loc> toLoc(const long long id) {
    if (id < 0) {
      return std::nullopt;
    }
    int remainder = (id / width) % (depth);
    int z = (id - remainder) / (width * depth);
    if (z >= height) {
      return std::nullopt;
    }
    int lessId = id - z * depth * width;
    int x = lessId % width;
    int y = (lessId - x) / width;
    if (y >= depth) {
      return std::nullopt;
    }
    return Loc{x, y, z};
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
  // Done calculating the original surface area, now do some tricks to make sure we get all the inner holes.

  SearchCube search{cube, width, depth, height};
  // Loc loc{3, 4, 5};
  // auto id = search.toId(loc).value();
  // auto newLoc = search.toLoc(id).value();
  // std::cout << newLoc.x << "," << newLoc.y << "," << newLoc.z << std::endl;

  // for (int x = 0; x < width; x++) {
  //   for (int y = 0; y < depth; y++) {
  //     if (cube[0][y][x]) {
  //       std::cout << "X";
  //     } else {
  //       std::cout << " ";
  //     }
  //   }
  //   std::cout << std::endl;
  // }
  // std::cout << std::endl;
  // for (int x = 0; x < width; x++) {
  //   for (int y = 0; y < depth; y++) {
  //     if (cube[height - 1][y][x]) {
  //       std::cout << "X";
  //     } else {
  //       std::cout << " ";
  //     }
  //   }
  //   std::cout << std::endl;
  // }
  // std::cout << std::endl;

  // for (int z = 0; z < height; z++) {
  //   for (int y = 0; y < depth; y++) {
  //     if (cube[z][y][0]) {
  //       std::cout << "X";
  //     } else {
  //       std::cout << " ";
  //     }
  //   }
  //   std::cout << std::endl;
  // }
  // std::cout << std::endl;
  // for (int z = 0; z < height; z++) {
  //   for (int y = 0; y < depth; y++) {
  //     if (cube[z][y][width - 1]) {
  //       std::cout << "X";
  //     } else {
  //       std::cout << " ";
  //     }
  //   }
  //   std::cout << std::endl;
  // }
  // std::cout << std::endl;

  // for (int x = 0; x < width; x++) {
  //   for (int z = 0; z < height; z++) {
  //     if (cube[z][0][x]) {
  //       std::cout << "X";
  //     } else {
  //       std::cout << " ";
  //     }
  //   }
  //   std::cout << std::endl;
  // }
  // std::cout << std::endl;
  // for (int x = 0; x < width; x++) {
  //   for (int z = 0; z < height; z++) {
  //     if (cube[z][depth - 1][x]) {
  //       std::cout << "X";
  //     } else {
  //       std::cout << " ";
  //     }
  //   }
  //   std::cout << std::endl;
  // }
  // std::cout << std::endl;

  // temp commenting.

  std::unordered_set<long long> visited;
  std::queue<Loc> visit;
  for (int x = 0; x < width; x++) {
    for (int y = 0; y < depth; y++) {
      if (!cube[0][y][x]) {
        Loc first{x, y, 0};
        visited.insert(search.toId(first).value());
        visit.push(first);
        cube[0][y][x] = true;
      }
      if (!cube[height - 1][y][x]) {
        Loc second{x, y, height - 1};
        visited.insert(search.toId(second).value());
        visit.push(second);
        cube[height - 1][y][x] = true;
      }
    }
  }
  for (int z = 0; z < height; z++) {
    for (int y = 0; y < depth; y++) {
      if (!cube[z][y][0]) {
        Loc first{0, y, z};
        visited.insert(search.toId(first).value());
        visit.push(first);
        cube[z][y][0] = true;
      }
      if (!cube[z][y][width - 1]) {
        Loc second{width - 1, y, z};
        visited.insert(search.toId(second).value());
        visit.push(second);
        cube[z][y][width - 1] = true;
      }
    }
  }
  for (int x = 0; x < width; x++) {
    for (int z = 0; z < height; z++) {
      if (!cube[z][0][x]) {
        Loc first{x, 0, z};
        visited.insert(search.toId(first).value());
        visit.push(first);
        cube[z][0][x] = true;
      }
      if (!cube[z][depth - 1][x]) {
        Loc second{x, depth - 1, z};
        visited.insert(search.toId(second).value());
        visit.push(second);
        cube[z][depth - 1][x] = true;
      }
    }
  }

  Loc first{0, 0, 0};
  visited.insert(search.toId(first).value());
  visit.push(first);
  cube[0][0][0] = true;

  while (!visit.empty()) {
    auto thisLoc = visit.front();
    visit.pop();
    for (const auto& dir : directions) {
      auto dirLoc = thisLoc + dir;
      auto maybeId = search.toId(dirLoc);
      if (maybeId.has_value()) {
        auto id = maybeId.value();
        // Not occupied, push and explore.
        if (!cube[dirLoc.z][dirLoc.y][dirLoc.x]) {
          auto iter = visited.find(id);
          if (iter == visited.end()) {
            visit.push(dirLoc);
            visited.insert(id);
            cube[dirLoc.z][dirLoc.y][dirLoc.x] = true;
          }
        }
      }
    }
  }
  int internalSurfaceArea = 0;
  for (int z = minZ; z <= maxZ; z++) {
    for (int y = minY; y <= maxY; y++) {
      for (int x = minX; x <= maxX; x++) {
        Loc current{x, y, z};
        if (cube[z - minZ][y - minY][x - minX]) {
          for (const auto& dir : directions) {
            auto dirLoc = current + dir;
            if (dirLoc.x < minX || dirLoc.x > maxX || dirLoc.y < minY || dirLoc.y > maxY || dirLoc.z < minZ ||
                dirLoc.z > maxZ) {
            } else {
              if (!cube[dirLoc.z - minZ][dirLoc.y - minY][dirLoc.x - minX]) {
                internalSurfaceArea++;
              }
            }
          }
        }
      }
    }
  }

  std::cout << "Interal surface area: " << internalSurfaceArea << std::endl;
  int outerSurfaceArea = surfaceArea - internalSurfaceArea;
  std::cout << "Outer surface area: " << outerSurfaceArea << std::endl;
  // 1984 too low.
  // 2046 too high.

  return 0;
}
