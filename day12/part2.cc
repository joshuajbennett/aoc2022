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
// cat ../day12/in.txt | day12/day12part1
// ```

int extractElevation(const char in) { return static_cast<int>(in) - static_cast<int>('a'); }

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

struct SearchMap {
  Loc start;
  Loc end;
  int width;
  int height;
  std::vector<std::vector<int>> heightGrid;
  std::optional<long> toId(const Loc& loc) {
    if (loc.x >= width || loc.x < 0) {
      return std::nullopt;
    }
    if (loc.y >= height || loc.y < 0) {
      return std::nullopt;
    }
    return loc.x + loc.y * width;
  }
  std::optional<Loc> toLoc(const long id) {
    if (id < 0) {
      return std::nullopt;
    }
    int x = id % width;
    int y = (id - x) / width;
    if (y >= height) {
      return std::nullopt;
    }
    return Loc{x, y};
  }
};

SearchMap parseMap() {
  std::string line;
  std::vector<std::vector<int>> heightGrid;
  std::optional<Loc> maybeStart;
  std::optional<Loc> maybeEnd;
  int y = 0;
  while (std::getline(std::cin, line)) {
    std::vector<int> row;
    for (int x = 0; x < line.size(); ++x) {
      char location = line[x];
      if (location == 'S') {
        row.push_back(extractElevation('a'));
        maybeStart = Loc{x, y};
      } else if (location == 'E') {
        row.push_back(extractElevation('z'));
        maybeEnd = Loc{x, y};
      } else {
        row.push_back(extractElevation(location));
      }
    }
    heightGrid.push_back(row);
    y++;
  }
  if (!maybeStart.has_value()) {
    throw std::runtime_error("Couldn't find a start location.");
  }
  if (!maybeEnd.has_value()) {
    throw std::runtime_error("Couldn't find an end location.");
  }
  return SearchMap{maybeStart.value(), maybeEnd.value(), static_cast<int>(heightGrid.front().size()),
                   static_cast<int>(heightGrid.size()), heightGrid};
}

struct VisitInfo {
  long parentId;
  int numSteps;
};

int main(int argc, char** argv) {
  auto map = parseMap();

  std::vector<Loc> initialLocations;
  for (int y = 0; y < map.height; ++y) {
    for (int x = 0; x < map.width; ++x) {
      if (map.heightGrid[y][x] == 0) {
        initialLocations.push_back(Loc{x, y});
      }
    }
  }

  int shortestPath = std::numeric_limits<int>::max();
  for (const auto& start : initialLocations) {

    std::queue<long> explore;
    auto startId = map.toId(start).value();
    explore.push(startId);

    std::vector<std::optional<VisitInfo>> row(map.width, std::nullopt);
    std::vector<std::vector<std::optional<VisitInfo>>> visited(map.height, row);

    visited[start.y][start.x] = VisitInfo{startId, 0};
    std::vector<Loc> directions{Loc{-1, 0}, Loc{1, 0}, Loc{0, -1}, Loc{0, 1}};
    while (!explore.empty()) {
      // Get the next search cell.
      auto nextId = explore.front();
      explore.pop();
      auto nextLoc = map.toLoc(nextId).value();
      int currentHeight = map.heightGrid[nextLoc.y][nextLoc.x];
      int stepsToGetHere = visited[nextLoc.y][nextLoc.x].value().numSteps;
      // Expand to any possible neighbors.
      for (const auto& dir : directions) {
        auto newLoc = Loc{nextLoc.x + dir.x, nextLoc.y + dir.y};
        auto maybeNewId = map.toId(newLoc);
        if (maybeNewId.has_value()) {
          // Check if this is a valid motion.
          if (map.heightGrid[newLoc.y][newLoc.x] - currentHeight <= 1) {
            // Check if this is more efficient than a previous visitor.
            if (visited[newLoc.y][newLoc.x].has_value()) {
              if (visited[newLoc.y][newLoc.x].value().numSteps <= stepsToGetHere + 1) {
                // Skip this element and pop the next of the queue.
                continue;
              }
            }
            visited[newLoc.y][newLoc.x] = VisitInfo{nextId, stepsToGetHere + 1};
            explore.push(maybeNewId.value());
          }
        }
      }
    }

    if (!visited[map.end.y][map.end.x].has_value()) {
      std::cout << "Didn't explore to an end location." << std::endl;
      continue;
    }
    int distanceToEnd = visited[map.end.y][map.end.x].value().numSteps;
    if (distanceToEnd < shortestPath) {
      shortestPath = distanceToEnd;
    }
  }
  std::cout << "Distance to the end: " << shortestPath << std::endl;

  return 0;
}
