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

struct Sensor {
  Loc sensorLoc;
  Loc beaconLoc;
};

int main(int argc, char** argv) {
  std::vector<Sensor> sensors;
  std::string line;
  while (std::getline(std::cin, line)) {
    auto firstSplit = split(line, ' ');
    auto sensorX = firstSplit[2];
    auto sensorY = firstSplit[3];
    auto beaconX = firstSplit[8];
    auto beaconY = firstSplit[9];

    sensors.push_back(Sensor{
        Loc{std::stoi(sensorX.substr(2, sensorX.size() - 2)), std::stoi(sensorY.substr(2, sensorY.size() - 2))},
        Loc{std::stoi(beaconX.substr(2, beaconX.size() - 2)), std::stoi(beaconY.substr(2, beaconY.size() - 2))}});
    std::cout << sensors.back().sensorLoc.x << " " << sensors.back().sensorLoc.y << " " << sensors.back().beaconLoc.x
              << " " << sensors.back().beaconLoc.y << std::endl;
  }
  int minX = sensors.front().sensorLoc.x;
  int maxX = sensors.front().sensorLoc.x;
  int minY = sensors.front().sensorLoc.y;
  int maxY = sensors.front().sensorLoc.y;

  for (const auto& sensor : sensors) {
    auto dist = std::abs(sensor.sensorLoc.x - sensor.beaconLoc.x) + std::abs(sensor.sensorLoc.y - sensor.beaconLoc.y);
    int localMinX = sensor.sensorLoc.x - dist;
    int localMaxX = sensor.sensorLoc.x + dist;
    int localMinY = sensor.sensorLoc.y - dist;
    int localMaxY = sensor.sensorLoc.y + dist;
    if (localMinX < minX) {
      minX = localMinX;
    }
    if (localMinY < minY) {
      minY = localMinY;
    }
    if (localMaxX > maxX) {
      maxX = localMaxX;
    }
    if (localMaxY > maxY) {
      maxY = localMaxY;
    }
  }

  int width = maxX - minX + 1;
  int height = maxY - minY + 1;

  std::vector<bool> row(width, false);
  std::vector<std::vector<bool>> grid(height, row);

  for (const auto& sensor : sensors) {
    auto dist = std::abs(sensor.sensorLoc.x - sensor.beaconLoc.x) + std::abs(sensor.sensorLoc.y - sensor.beaconLoc.y);
    int localMinX = sensor.sensorLoc.x - dist;
    int localMaxX = sensor.sensorLoc.x + dist;
    int localMinY = sensor.sensorLoc.y - dist;
    int localMaxY = sensor.sensorLoc.y + dist;
    for (int x = localMinX; x <= localMaxX; x++) {
      for (int y = localMinY; y <= localMaxY; y++) {
        auto localDist = std::abs(sensor.sensorLoc.x - x) + std::abs(sensor.sensorLoc.y - y);
        if (localDist <= dist) {
          grid[y - minY][x - minX] = true;
        }
      }
    }
  }

  for (const auto& sensor : sensors) {
    grid[sensor.beaconLoc.y - minY][sensor.beaconLoc.x - minX] = false;
  }

  int numOccupied = 0;
  int y = 2000000;
  for (int x = minX; x <= maxX; ++x) {
    if (grid[y - minY][x - minX]) {
      numOccupied++;
    }
  }

  std::cout << "Number occupied: " << numOccupied << std::endl;

  return 0;
}
