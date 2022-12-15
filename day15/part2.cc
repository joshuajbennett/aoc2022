#include <cmath>
#include <deque>
#include <functional>
#include <iostream>
#include <optional>
#include <queue>
#include <sstream>
#include <string>
#include <unordered_set>
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
  }
  // long long maxSearchCoords = 20;
  long long maxSearchCoords = 4000000;
  std::unordered_set<unsigned long long> answers;
  std::vector<Loc> locs(4, Loc{0, 0});
  for (const auto& outerSensor : sensors) {
    std::cout << "Eval sensor" << std::endl;
    auto searchDist = std::abs(outerSensor.sensorLoc.x - outerSensor.beaconLoc.x) +
                      std::abs(outerSensor.sensorLoc.y - outerSensor.beaconLoc.y) + 1;
    for (int i = 0; i <= searchDist; ++i) {
      locs[0].x = outerSensor.sensorLoc.x - searchDist + i;
      locs[0].y = outerSensor.sensorLoc.y + i;
      locs[1].x = outerSensor.sensorLoc.x + searchDist - i;
      locs[1].y = outerSensor.sensorLoc.y + i;
      locs[2].x = outerSensor.sensorLoc.x - searchDist + i;
      locs[2].y = outerSensor.sensorLoc.y - i;
      locs[3].x = outerSensor.sensorLoc.x + searchDist - i;
      locs[3].y = outerSensor.sensorLoc.y - i;
      for (const auto& loc : locs) {
        if (loc.x < 0 || loc.x > maxSearchCoords || loc.y < 0 || loc.y > maxSearchCoords) {
          continue;
        }
        bool withinSensor = false;
        for (const auto& sensor : sensors) {
          auto dist =
              std::abs(sensor.sensorLoc.x - sensor.beaconLoc.x) + std::abs(sensor.sensorLoc.y - sensor.beaconLoc.y);
          auto yDist = std::abs(sensor.sensorLoc.y - loc.y);
          if (yDist <= dist) {
            auto xDist = dist - yDist;
            if (std::abs(sensor.sensorLoc.x - loc.x) <= xDist) {
              withinSensor = true;
              break;
            }
          }
        }
        if (!withinSensor) {
          bool notBeacon = true;
          for (const auto& sensor : sensors) {
            if (loc.x == sensor.beaconLoc.x && loc.y == sensor.beaconLoc.y) {
              notBeacon = false;
            }
            auto dist =
                std::abs(sensor.sensorLoc.x - sensor.beaconLoc.x) + std::abs(sensor.sensorLoc.y - sensor.beaconLoc.y);
            auto currentDist = std::abs(sensor.sensorLoc.x - loc.x) + std::abs(sensor.sensorLoc.y - loc.y);
            if (dist == currentDist) {
              notBeacon = false;
            }
          }
          if (notBeacon) {
            std::cout << "Loc x: " << loc.x << " Loc y: " << loc.y << std::endl;
            answers.insert(static_cast<unsigned long long>(loc.x) * 4000000 + static_cast<unsigned long long>(loc.y));
          }
        }
      }
    }
  }

  if (answers.size() > 1) {
    std::cout << "Found more than one answer " << std::endl;
  } else if (answers.size() == 0) {
    std::cout << "No answers found." << std::endl;
  } else {
    std::cout << "Tuning freq: " << *answers.begin() << std::endl;
  }

  return 0;
}
