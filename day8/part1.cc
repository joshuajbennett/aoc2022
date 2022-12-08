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

  std::vector<bool> rowVisibility(trees.width, false);
  std::vector<std::vector<bool>> visibility(trees.height, rowVisibility);

  // Sweep from all sides and compute visibility.
  visibility[0][0] = true;
  visibility[0][trees.width - 1] = true;
  visibility[trees.height - 1][trees.width - 1] = true;
  visibility[trees.height - 1][0] = true;

  for (int x = 0; x < trees.width; ++x) {
    int prevHeight = -1;
    for (int y = 0; y < trees.height; ++y) {
      // Sweeping left from X.
      if (trees.grid[y][x] > prevHeight) {
        visibility[y][x] = true;
        prevHeight = trees.grid[y][x];
      }
    }
  }

  for (int x = 0; x < trees.width; ++x) {
    int prevHeight = -1;
    for (int y = trees.height - 1; y >= 0; --y) {
      // Sweeping right from X.
      if (trees.grid[y][x] > prevHeight) {
        visibility[y][x] = true;
        prevHeight = trees.grid[y][x];
      }
    }
  }

  for (int y = 0; y < trees.height; ++y) {
    int prevHeight = -1;
    for (int x = 0; x < trees.width; ++x) {
      // Sweeping down from Y.
      if (trees.grid[y][x] > prevHeight) {
        visibility[y][x] = true;
        prevHeight = trees.grid[y][x];
      }
    }
  }

  for (int y = 0; y < trees.height; ++y) {
    int prevHeight = -1;
    for (int x = trees.width - 1; x >= 0; --x) {
      // Sweeping down from Y.
      if (trees.grid[y][x] > prevHeight) {
        visibility[y][x] = true;
        prevHeight = trees.grid[y][x];
      }
    }
  }

  // Sum the visible trees.
  int numVisibleTrees = 0;
  for (int y = 0; y < trees.height; ++y) {
    for (int x = 0; x < trees.width; ++x) {
      if (visibility[y][x]) {
        numVisibleTrees++;
      }
    }
  }

  std::cout << std::endl;
  renderVisibility(visibility);
  std::cout << std::endl;
  std::cout << "Number of visible trees: " << numVisibleTrees << std::endl;

  return 0;
}
