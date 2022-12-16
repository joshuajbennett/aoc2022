#include <cmath>
#include <deque>
#include <functional>
#include <iostream>
#include <optional>
#include <queue>
#include <sstream>
#include <string>
#include <unordered_map>
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

struct Valve {
  int flowRate;
  bool opened;
  std::vector<std::string> tunnels;
};

int releasePressure(const std::unordered_map<std::string, Valve>& valves, const int timeLeft,
                    const std::string location, const int currentScore) {
  auto& node = valves.at(location);
  std::vector<int> finalPressures;
  int newTimeLeft = timeLeft - 1;
  if (newTimeLeft == 0) {
    return currentScore;
  }
  // Move somewhere else.
  for (const auto& newLocation : node.tunnels) {
    finalPressures.push_back(releasePressure(valves, newTimeLeft, newLocation, currentScore));
  }
  // Open the valve
  if (!node.opened && node.flowRate > 0) {
    int newScore = node.flowRate * newTimeLeft + currentScore;
    auto valvesCopy = valves;
    valvesCopy.at(location).opened = true;
    finalPressures.push_back(releasePressure(valvesCopy, newTimeLeft, location, newScore));
  }
  std::sort(finalPressures.begin(), finalPressures.end(), std::greater<int>());
  return finalPressures.front();
}

int main(int argc, char** argv) {
  std::string line;
  int timeLeft = 30;
  std::unordered_map<std::string, Valve> valves;

  while (std::getline(std::cin, line)) {
    auto elements = split(line, ' ');
    auto name = elements[1];
    auto flowRateElem = elements[4];
    auto flowRate = std::stoi(flowRateElem.substr(5, flowRateElem.size() - 5));
    std::vector<std::string> tunnels;
    for (int i = 9; i < elements.size(); ++i) {
      auto tunnelElem = elements[i];
      if (i == elements.size() - 1) {
        tunnels.push_back(elements[i]);
      } else {
        auto tunnel = tunnelElem.substr(0, tunnelElem.size() - 1);
        tunnels.push_back(tunnel);
      }
    }
    valves.insert(std::make_pair(name, Valve{flowRate, false, tunnels}));
  }
  int maxPressure = releasePressure(valves, timeLeft, "AA", 0);
  std::cout << "Maximum pressure release: " << maxPressure << std::endl;

  return 0;
}
