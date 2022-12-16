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
  std::unordered_map<std::string, int> tunnelTimes;
};

struct QueuedAction {
  std::string location;
  int timeLeft;
  int additionalScore;
};

int releasePressure(const std::unordered_map<std::string, Valve>& valves, const int timeLeft,
                    const std::string location, const int currentScore,
                    const std::unordered_set<std::string>& nodesOfInterest,
                    const std::optional<QueuedAction> otherAction) {
  auto& node = valves.at(location);

  // Compiling a list of scores from the recursion.
  std::vector<int> scores;
  for (const auto& newValve : nodesOfInterest) {
    const auto& dist = node.tunnelTimes.at(newValve);
    int newTime = timeLeft - dist - 1;
    if (newTime <= 0) {
      continue;
    }
    const auto& newNode = valves.at(newValve);
    int additionalScore = newTime * newNode.flowRate;
    auto newNodesOfInterest = nodesOfInterest;
    newNodesOfInterest.erase(newValve);
    if (otherAction.has_value()) {
      auto action = otherAction.value();
      if (newTime < action.timeLeft) {
        int newScore = currentScore + action.additionalScore;
        scores.push_back(releasePressure(valves, action.timeLeft, action.location, newScore, newNodesOfInterest,
                                         QueuedAction{newValve, newTime, additionalScore}));
        continue;
      }
    }
    int newScore = currentScore + additionalScore;
    scores.push_back(releasePressure(valves, newTime, newValve, newScore, newNodesOfInterest, otherAction));
  }
  if (otherAction.has_value()) {
    auto action = otherAction.value();
    int newScore = currentScore + action.additionalScore;
    scores.push_back(
        releasePressure(valves, action.timeLeft, action.location, newScore, nodesOfInterest, std::nullopt));
  }

  if (scores.empty()) {
    return currentScore;
  }
  std::sort(scores.begin(), scores.end(), std::greater<int>());
  return scores.front();
}

struct NextNode {
  std::string valveName;
  int distTraveled;
  bool operator<(const NextNode& rhs) const { return distTraveled > rhs.distTraveled; }
};

int computeDistance(const std::unordered_map<std::string, Valve>& valves, const std::string& origin,
                    const std::string& destination) {
  std::unordered_map<std::string, int> bestDistance;
  std::priority_queue<NextNode> toVisit;
  toVisit.push(NextNode{origin, 0});
  bestDistance.insert(std::make_pair(origin, 0));
  while (!toVisit.empty()) {
    auto visiting = toVisit.top();
    toVisit.pop();
    // std::cout << visiting.distTraveled << std::endl;
    if (visiting.valveName == destination) {
      // std::cout << "Dist from " << origin << " to " << destination << " is " << visiting.distTraveled << std::endl;
      return visiting.distTraveled;
    }
    const auto& valve = valves.at(visiting.valveName);
    for (const auto& [neighbor, distance] : valve.tunnelTimes) {
      int dist = visiting.distTraveled + distance;
      auto bestDistIter = bestDistance.find(neighbor);
      if (bestDistIter == bestDistance.end()) {
        bestDistance.insert(std::make_pair(neighbor, dist));
        toVisit.push(NextNode{neighbor, dist});
      } else {
        if (bestDistIter->second > dist) {
          bestDistIter->second = dist;
          toVisit.push(NextNode{neighbor, dist});
        }
      }
    }
  }
  throw std::runtime_error("Didn't find path.");
}

int main(int argc, char** argv) {
  std::string line;
  int timeLeft = 30;

  // Teach the elephant
  timeLeft -= 4;

  std::unordered_map<std::string, Valve> valves;

  while (std::getline(std::cin, line)) {
    auto elements = split(line, ' ');
    auto name = elements[1];
    auto flowRateElem = elements[4];
    auto flowRate = std::stoi(flowRateElem.substr(5, flowRateElem.size() - 5));
    std::unordered_map<std::string, int> tunnelTimes;
    for (int i = 9; i < elements.size(); ++i) {
      auto tunnelElem = elements[i];
      if (i == elements.size() - 1) {
        tunnelTimes.insert(std::make_pair(elements[i], 1));
      } else {
        auto tunnel = tunnelElem.substr(0, tunnelElem.size() - 1);
        tunnelTimes.insert(std::make_pair(tunnel, 1));
      }
    }
    valves.insert(std::make_pair(name, Valve{flowRate, tunnelTimes}));
  }

  std::unordered_set<std::string> nodesOfInterest;
  for (auto& [name, valve] : valves) {
    if (valve.flowRate > 0) {
      nodesOfInterest.insert(name);
    }
  }
  for (auto& [name, valve] : valves) {
    for (const auto& interest : nodesOfInterest) {
      if (interest == name) {
        continue;
      }
      if (valve.tunnelTimes.find(interest) == valve.tunnelTimes.end()) {
        auto dist = computeDistance(valves, name, interest);
        valve.tunnelTimes.insert(std::make_pair(interest, dist));
      }
    }
  }

  int maxPressure = releasePressure(valves, timeLeft, "AA", 0, nodesOfInterest, QueuedAction{"AA", timeLeft, 0});
  std::cout << "Maximum pressure release: " << maxPressure << std::endl;

  return 0;
}
