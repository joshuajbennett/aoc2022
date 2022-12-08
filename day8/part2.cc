#include <iostream>
#include <sstream>
#include <string>
#include <vector>

// To run build the code and then do the following command:
// ```
// cat ../day8/in.txt | day8/day8part1
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

struct TreeGrid {
  int width;
  int height;
  std::vector<std::vector<int>> grid;
};

void renderGrid(const TreeGrid& trees) {
  for (int y = 0; y < trees.height; ++y) {
    for (int x = 0; x < trees.width; ++x) {
      std::cout << trees.grid[y][x];
    }
    std::cout << std::endl;
  }
}

void renderVisibility(const std::vector<std::vector<bool>>& visibility) {
  for (size_t y = 0; y < visibility.size(); ++y) {
    for (size_t x = 0; x < visibility.back().size(); ++x) {
      std::cout << (visibility[y][x] ? "1" : " ");
    }
    std::cout << std::endl;
  }
}

int main(int argc, char** argv) {
  TreeGrid trees = []() {
    std::string line;
    std::vector<std::vector<int>> grid;
    while (std::getline(std::cin, line)) {
      std::vector<int> row;
      for (int i = 0; i < line.size(); ++i) {
        row.push_back(std::stoi(line.substr(i, 1)));
      }
      if (grid.size() > 0) {
        if (grid.back().size() != row.size()) {
          throw std::runtime_error("Rows weren't equal in size.");
        }
      }
      grid.push_back(row);
    }
    return TreeGrid{static_cast<int>(grid.back().size()), static_cast<int>(grid.size()), grid};
  }();
  renderGrid(trees);

  int maxScenicScore = 0;
  for (int x = 1; x < trees.width - 1; ++x) {
    for (int y = 1; y < trees.height - 1; ++y) {
      int currentHeight = trees.grid[y][x];
      // Compute scenic score from this location.
      int scoreEast = 0;
      for (int newX = x + 1; newX < trees.width; ++newX) {
        scoreEast++;
        if (trees.grid[y][newX] >= currentHeight) {
          break;
        }
      }
      int scoreWest = 0;
      for (int newX = x - 1; newX >= 0; --newX) {
        scoreWest++;
        if (trees.grid[y][newX] >= currentHeight) {
          break;
        }
      }
      int scoreNorth = 0;
      for (int newY = y + 1; newY < trees.height; ++newY) {
        scoreNorth++;
        if (trees.grid[newY][x] >= currentHeight) {
          break;
        }
      }
      int scoreSouth = 0;
      for (int newY = y - 1; newY >= 0; --newY) {
        scoreSouth++;
        if (trees.grid[newY][x] >= currentHeight) {
          break;
        }
      }
      int scenicScore = scoreEast * scoreWest * scoreNorth * scoreSouth;
      if (scenicScore > maxScenicScore) {
        maxScenicScore = scenicScore;
      }
    }
  }

  std::cout << "Highest scenic score: " << maxScenicScore << std::endl;

  return 0;
}
