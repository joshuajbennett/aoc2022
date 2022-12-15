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

  int numOccupied = 0;
  int y = 2000000;
  // int y = 10;
  for (int x = minX; x <= maxX; ++x) {
    bool notBeacon = false;
    for (const auto& sensor : sensors) {
      if (sensor.beaconLoc.y == y && sensor.beaconLoc.x == x) {
        notBeacon = false;
        break;
      }
      auto dist = std::abs(sensor.sensorLoc.x - sensor.beaconLoc.x) + std::abs(sensor.sensorLoc.y - sensor.beaconLoc.y);
      auto yDist = std::abs(sensor.sensorLoc.y - y);
      if (yDist < dist) {
        auto xDist = dist - yDist;
        if (std::abs(sensor.sensorLoc.x - x) <= xDist) {
          notBeacon = true;
          break;
        }
      }
    }
    if (notBeacon) {
      numOccupied++;
    }
  }

  std::cout << "Number occupied: " << numOccupied << std::endl;

  return 0;
}
