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
  int x;
  int y;
};

struct Sprite {
  int width;
  int height;
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
  std::vector<Object> objects;
  int currentHeight = 0;
  int width = 7;
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
    for (const auto& object : objects) {
      if (std::abs(object.pos.y - newObject.pos.y) > 4) {
        continue;
      }
      for (int y = 0; y < newObject.sprite.height; ++y) {
        for (int x = 0; x < newObject.sprite.width; ++x) {
          if (newObject.sprite.pixels[y][x]) {
            if (object.at(Pos{x + newObject.pos.x, y + newObject.pos.y})) {
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
    for (const auto& object : objects) {
      if (std::abs(object.pos.y - queryPos.y) > 4) {
        continue;
      }
      if (object.at(queryPos)) {
        return true;
      }
    }
    return false;
  }

  void append(const Object& newObject) {
    objects.push_back(newObject);
    int potentialNewHeight = newObject.pos.y + newObject.sprite.height - 1;
    if (potentialNewHeight > currentHeight) {
      currentHeight = potentialNewHeight;
    }
  }
};

void renderRocks(const Landed& landed, const Object& object) {
  int topOfObject = object.pos.y + object.sprite.height - 1;
  int currentHeight = landed.currentHeight;
  for (int y = std::max(topOfObject, currentHeight); y >= 0; --y) {
    for (int x = 0; x <= landed.width + 1; ++x) {
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
  int maxNumRocks = 2022;

  Landed landed;

  numRocks++;
  while (numRocks <= maxNumRocks) {
    Object newRock{sprites[spriteIndex], Pos{3, landed.currentHeight + 4}};
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
    if (landed.objects.size() >= maxNumRocks) {
      std::cout << "Height: " << landed.currentHeight << std::endl;
      return 0;
    }
  }

  return 0;
}
