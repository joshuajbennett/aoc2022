#include <chrono>
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

enum class Jet { Left, Right };

struct Pos {
  long long x;
  long long y;
};

struct Sprite {
  long long width;
  long long height;
  std::vector<std::vector<bool>> pixels;
};

struct Object {
  Sprite sprite;
  Pos pos;
  bool at(const Pos& queryPos) const {
    if (queryPos.x < pos.x) {
      return false;
    }
    if (queryPos.y < pos.y) {
      return false;
    }
    if (queryPos.x > pos.x + sprite.width - 1) {
      return false;
    }
    if (queryPos.y > pos.y + sprite.height - 1) {
      return false;
    }
    return sprite.pixels[queryPos.y - pos.y][queryPos.x - pos.x];
  }
};

struct Landed {
  std::deque<std::vector<bool>> rows;
  long long currentHeight = 0;
  long long width = 7;
  long long numObjects = 0;
  long long maxHeight = 60;
  bool collides(const Object& newObject) const {
    if (newObject.pos.x <= 0) {
      return true;
    }
    if (newObject.pos.x + newObject.sprite.width - 1 >= width + 1) {
      return true;
    }
    if (newObject.pos.y <= 0) {
      return true;
    }
    if (currentHeight - newObject.pos.y >= (maxHeight - 5)) {
      throw std::runtime_error("Increase max height.");
    }
    for (long long y = 0; y < newObject.sprite.height; ++y) {
      if (y + newObject.pos.y <= currentHeight) {
        for (long long x = 0; x < newObject.sprite.width; ++x) {
          if (newObject.sprite.pixels[y][x]) {
            int dequeY = currentHeight - (y + newObject.pos.y);
            if (rows[dequeY][x + newObject.pos.x]) {
              return true;
            }
          }
        }
      }
    }
    return false;
  }
  bool at(const Pos& queryPos) const {
    if (queryPos.x <= 0) {
      return true;
    }
    if (queryPos.x >= width + 1) {
      return true;
    }
    if (queryPos.y <= 0) {
      return true;
    }

    if (queryPos.y <= currentHeight) {
      int dequeY = currentHeight - (queryPos.y);
      if (rows[dequeY][queryPos.x]) {
        return true;
      }
    }
    return false;
  }

  void append(const Object& newObject) {
    numObjects++;
    long long potentialNewHeight = newObject.pos.y + newObject.sprite.height - 1;
    if (potentialNewHeight > currentHeight) {
      std::vector<bool> row(width + 2, false);
      for (int i = 0; i < (potentialNewHeight - currentHeight); ++i) {
        rows.push_front(row);
        rows.pop_back();
      }
      currentHeight = potentialNewHeight;
    }
    for (long long y = 0; y < newObject.sprite.height; ++y) {
      if (y + newObject.pos.y <= currentHeight) {
        for (long long x = 0; x < newObject.sprite.width; ++x) {
          if (newObject.sprite.pixels[y][x]) {
            int dequeY = currentHeight - (y + newObject.pos.y);
            rows[dequeY][x + newObject.pos.x] = true;
          }
        }
      }
    }
  }
};

void renderRocks(const Landed& landed, const Object& object) {
  long long topOfObject = object.pos.y + object.sprite.height - 1;
  long long currentHeight = landed.currentHeight;
  int numY = 0;
  for (long long y = std::max(topOfObject, currentHeight); y >= 0; --y) {
    numY++;
    if (numY > 60) {
      std::cout << std::endl;
      return;
    }

    for (long long x = 0; x <= landed.width + 1; ++x) {
      if (y == 0) {
        if (x == 0) {
          std::cout << "+";
        } else if (x == landed.width + 1) {
          std::cout << "+";
        } else {
          std::cout << "-";
        }
      } else if (x == 0) {
        std::cout << "|";
      } else if (x == landed.width + 1) {
        std::cout << "|";
      } else if (landed.at(Pos{x, y})) {
        std::cout << "#";
      } else if (object.at(Pos{x, y})) {
        std::cout << "@";
      } else {
        std::cout << ".";
      }
    }
    std::cout << std::endl;
  }
  std::cout << std::endl;
}

int main(int argc, char** argv) {
  std::string line;

  if (!std::getline(std::cin, line)) {
    std::cerr << "Didn't get a line." << std::endl;
    return -1;
  }
  std::vector<Jet> jets;
  for (int i = 0; i < line.size(); ++i) {
    if (line[i] == '>') {
      jets.push_back(Jet::Right);
    } else if (line[i] == '<') {
      jets.push_back(Jet::Left);
    } else {
      std::cerr << "Couldn't parse a jet direction." << std::endl;
      return -1;
    }
  }

  std::vector<Sprite> sprites;
  // Horizontal bar.
  sprites.push_back(Sprite{4, 1, std::vector<std::vector<bool>>{{true, true, true, true}}});
  // Plus sign.
  sprites.push_back(
      Sprite{3, 3, std::vector<std::vector<bool>>{{false, true, false}, {true, true, true}, {false, true, false}}});
  // Backwards L.
  sprites.push_back(
      Sprite{3, 3, std::vector<std::vector<bool>>{{true, true, true}, {false, false, true}, {false, false, true}}});
  // Vertical bar.
  sprites.push_back(Sprite{1, 4, std::vector<std::vector<bool>>{{true}, {true}, {true}, {true}}});
  // Square.
  sprites.push_back(Sprite{2, 2, std::vector<std::vector<bool>>{{true, true}, {true, true}}});

  int spriteIndex = 0;
  int jetsIndex = 0;

  int numRocks = 0;
  long long maxNumRocks = 1000000000000;
  // long long maxNumRocks = 2022;
  // long long maxNumRocks = 12;

  Landed landed;
  std::vector<bool> row(landed.width + 2, false);
  for (int i = 0; i < landed.maxHeight; ++i) {
    landed.rows.push_front(row);
  }

  numRocks++;
  while (numRocks <= maxNumRocks) {
    Object newRock{sprites[spriteIndex], Pos{3, landed.currentHeight + 4}};
    if (landed.numObjects % 5000000 == 0) {
      // renderRocks(landed, newRock);
      std::cout << "Current Height: " << landed.currentHeight
                << " Progress: " << static_cast<float>(landed.numObjects) / static_cast<float>(maxNumRocks) * 100
                << std::endl;
    }

    spriteIndex++;
    if (spriteIndex >= sprites.size()) {
      spriteIndex = 0;
    }
    // renderRocks(landed, newRock);
    while (!landed.collides(newRock)) {
      // renderRocks(landed, newRock);
      if (jets[jetsIndex] == Jet::Left) {
        // Try to move left.
        newRock.pos.x--;
        if (landed.collides(newRock)) {
          newRock.pos.x++;
        }
      } else {
        // Try to move right.
        newRock.pos.x++;
        if (landed.collides(newRock)) {
          newRock.pos.x--;
        }
      }
      // renderRocks(landed, newRock);
      jetsIndex++;
      if (jetsIndex >= jets.size()) {
        jetsIndex = 0;
      }
      // Try to move down.
      newRock.pos.y--;
    }
    newRock.pos.y++;
    landed.append(newRock);
    // renderRocks(landed, newRock);
    if (landed.numObjects >= maxNumRocks) {
      std::cout << "Height: " << landed.currentHeight << std::endl;
      return 0;
    }
  }

  return 0;
}