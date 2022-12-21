#include <atomic>
#include <cmath>
#include <deque>
#include <functional>
#include <iostream>
#include <mutex>
#include <optional>
#include <queue>
#include <sstream>
#include <string>
#include <thread>
#include <unordered_map>
#include <unordered_set>
#include <vector>

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

struct Robots {
  int oreRobots = 0;
  int clayRobots = 0;
  int obsidianRobots = 0;
  int geodeRobots = 0;
  bool oreInProgress = false;
  bool clayInProgress = false;
  bool obsidianInProgress = false;
  bool geodeInProgress = false;
  Robots buildOreRobot() const {
    return Robots{oreRobots, clayRobots, obsidianRobots, geodeRobots, true, false, false, false};
  }
  Robots buildClayRobot() const {
    return Robots{oreRobots, clayRobots, obsidianRobots, geodeRobots, false, true, false, false};
  }
  Robots buildObsidianRobot() const {
    return Robots{oreRobots, clayRobots, obsidianRobots, geodeRobots, false, false, true, false};
  }
  Robots buildGeodeRobot() const {
    return Robots{oreRobots, clayRobots, obsidianRobots, geodeRobots, false, false, false, true};
  }
  bool building() const { return (oreInProgress || clayInProgress || obsidianInProgress || geodeInProgress); }
  Robots finishBuilding() const {
    if (oreInProgress) {
      return Robots{oreRobots + 1, clayRobots, obsidianRobots, geodeRobots};
    } else if (clayInProgress) {
      return Robots{oreRobots, clayRobots + 1, obsidianRobots, geodeRobots};
    } else if (obsidianInProgress) {
      return Robots{oreRobots, clayRobots, obsidianRobots + 1, geodeRobots};
    } else if (geodeInProgress) {
      return Robots{oreRobots, clayRobots, obsidianRobots, geodeRobots + 1};
    } else {
      return Robots{oreRobots, clayRobots, obsidianRobots, geodeRobots};
    }
  }
};

struct Resources {
  int ore = 0;
  int clay = 0;
  int obsidian = 0;
  int geode = 0;
};

struct RobotCost {
  bool parsed = false;
  int oreCost = 0;
  int clayCost = 0;
  int obsidianCost = 0;
  int geodeCost = 0;
};

// Takes 1 minute for the robot factor to construct any type of robot.
struct Blueprint {
  int id;
  // Each robot collects one of its resource type per minute.
  RobotCost oreRobotCost;
  RobotCost clayRobotCost;
  RobotCost obsidianRobotCost;
  RobotCost geodeRobotCost;
};

Resources incrementResources(const Resources& resources, const Robots& robots, int time) {
  return Resources{resources.ore + time * robots.oreRobots, resources.clay + time * robots.clayRobots,
                   resources.obsidian + time * robots.obsidianRobots, resources.geode + time * robots.geodeRobots};
}

bool costFeasible(const Resources& resources, const RobotCost& cost) {
  if (resources.ore >= cost.oreCost && resources.clay >= cost.clayCost && resources.obsidian >= cost.obsidianCost &&
      resources.geode >= cost.geodeCost) {
    return true;
  }
  return false;
}

std::optional<int> timeRequired(const Robots& robots, const Resources& resources, const RobotCost& cost) {
  int maxTime = std::numeric_limits<int>::lowest();
  if (cost.oreCost > 0) {
    if (robots.oreRobots > 0) {
      int time = (cost.oreCost - resources.ore + robots.oreRobots - 1) / robots.oreRobots;
      if (time > maxTime) {
        maxTime = time;
      }
    } else {
      return std::nullopt;
    }
  }
  if (cost.clayCost > 0) {
    if (robots.clayRobots > 0) {
      int time = (cost.clayCost - resources.clay + robots.clayRobots - 1) / robots.clayRobots;
      if (time > maxTime) {
        maxTime = time;
      }
    } else {
      return std::nullopt;
    }
  }
  if (cost.obsidianCost > 0) {
    if (robots.obsidianRobots > 0) {
      int time = (cost.obsidianCost - resources.obsidian + robots.obsidianRobots - 1) / robots.obsidianRobots;
      if (time > maxTime) {
        maxTime = time;
      }
    } else {
      return std::nullopt;
    }
  }
  if (cost.geodeCost > 0) {
    if (robots.geodeRobots > 0) {
      int time = (cost.geodeCost - resources.geode + robots.geodeRobots - 1) / robots.geodeRobots;
      if (time > maxTime) {
        maxTime = time;
      }
    } else {
      return std::nullopt;
    }
  }
  if (maxTime < 0) {
    return 0;
  }
  return maxTime;
}

Resources applyCost(const Resources& resources, const RobotCost& cost) {
  return Resources{resources.ore - cost.oreCost, resources.clay - cost.clayCost, resources.obsidian - cost.obsidianCost,
                   resources.geode - cost.geodeCost};
}

int evaluate(const Blueprint& blueprint, const Robots& robots, const Resources& resources, const int minutesLeft,
             std::atomic_int& numThreads) {
  bool inParallel = numThreads.load() < 50 && minutesLeft > 15;
  std::mutex mut;
  std::vector<int> scores;
  std::vector<std::thread> threads;

  if (robots.building()) {
    auto resourcesAtEnd = incrementResources(resources, robots, 1);
    auto robotsAtEnd = robots.finishBuilding();
    int timeLeft = minutesLeft - 1;
    if (timeLeft == 0) {
      return resourcesAtEnd.geode;
    }
    bool ableToBuild = false;
    auto oreTimeRequired = timeRequired(robotsAtEnd, resourcesAtEnd, blueprint.oreRobotCost);
    if (oreTimeRequired.has_value()) {
      auto timeReq = oreTimeRequired.value();
      if (timeReq <= timeLeft) {
        ableToBuild = true;
        if (inParallel) {
          numThreads++;
          threads.push_back(std::thread([robotsAtEnd, resourcesAtEnd, timeReq, blueprint, timeLeft, &mut, &scores,
                                         &numThreads]() {
            auto score =
                evaluate(blueprint, robotsAtEnd.buildOreRobot(),
                         applyCost(incrementResources(resourcesAtEnd, robotsAtEnd, timeReq), blueprint.oreRobotCost),
                         timeLeft - timeReq, numThreads);

            std::scoped_lock l{mut};
            scores.push_back(score);
          }));
        } else {
          scores.push_back(
              evaluate(blueprint, robotsAtEnd.buildOreRobot(),
                       applyCost(incrementResources(resourcesAtEnd, robotsAtEnd, timeReq), blueprint.oreRobotCost),
                       timeLeft - timeReq, numThreads));
        }
      }
    }
    auto clayTimeRequired = timeRequired(robotsAtEnd, resourcesAtEnd, blueprint.clayRobotCost);
    if (clayTimeRequired.has_value()) {
      auto timeReq = clayTimeRequired.value();
      if (timeReq <= timeLeft) {
        ableToBuild = true;
        if (inParallel) {
          numThreads++;
          threads.push_back(std::thread([robotsAtEnd, resourcesAtEnd, timeReq, blueprint, timeLeft, &mut, &scores,
                                         &numThreads]() {
            auto score =
                evaluate(blueprint, robotsAtEnd.buildClayRobot(),
                         applyCost(incrementResources(resourcesAtEnd, robotsAtEnd, timeReq), blueprint.clayRobotCost),
                         timeLeft - timeReq, numThreads);
            std::scoped_lock l{mut};
            scores.push_back(score);
          }));
        } else {
          scores.push_back(
              evaluate(blueprint, robotsAtEnd.buildClayRobot(),
                       applyCost(incrementResources(resourcesAtEnd, robotsAtEnd, timeReq), blueprint.clayRobotCost),
                       timeLeft - timeReq, numThreads));
        }
      }
    }
    auto obsidianTimeRequired = timeRequired(robotsAtEnd, resourcesAtEnd, blueprint.obsidianRobotCost);
    if (obsidianTimeRequired.has_value()) {
      auto timeReq = obsidianTimeRequired.value();
      if (timeReq <= timeLeft) {
        ableToBuild = true;
        if (inParallel) {
          numThreads++;
          threads.push_back(
              std::thread([robotsAtEnd, resourcesAtEnd, timeReq, blueprint, timeLeft, &mut, &scores, &numThreads]() {
                auto score = evaluate(
                    blueprint, robotsAtEnd.buildObsidianRobot(),
                    applyCost(incrementResources(resourcesAtEnd, robotsAtEnd, timeReq), blueprint.obsidianRobotCost),
                    timeLeft - timeReq, numThreads);
                std::scoped_lock l{mut};
                scores.push_back(score);
              }));
        } else {
          scores.push_back(
              evaluate(blueprint, robotsAtEnd.buildObsidianRobot(),
                       applyCost(incrementResources(resourcesAtEnd, robotsAtEnd, timeReq), blueprint.obsidianRobotCost),
                       timeLeft - timeReq, numThreads));
        }
      }
    }
    auto geodeTimeRequired = timeRequired(robotsAtEnd, resourcesAtEnd, blueprint.geodeRobotCost);
    if (geodeTimeRequired.has_value()) {
      auto timeReq = geodeTimeRequired.value();
      if (timeReq <= timeLeft) {
        ableToBuild = true;
        if (inParallel) {
          numThreads++;
          threads.push_back(std::thread([robotsAtEnd, resourcesAtEnd, timeReq, blueprint, timeLeft, &mut, &scores,
                                         &numThreads]() {
            auto score =
                evaluate(blueprint, robotsAtEnd.buildGeodeRobot(),
                         applyCost(incrementResources(resourcesAtEnd, robotsAtEnd, timeReq), blueprint.geodeRobotCost),
                         timeLeft - timeReq, numThreads);
            std::scoped_lock l{mut};
            scores.push_back(score);
          }));
        } else {
          scores.push_back(
              evaluate(blueprint, robotsAtEnd.buildGeodeRobot(),
                       applyCost(incrementResources(resourcesAtEnd, robotsAtEnd, timeReq), blueprint.geodeRobotCost),
                       timeLeft - timeReq, numThreads));
        }
      }
    }
    if (inParallel) {
      for (auto& thread : threads) {
        thread.join();
        numThreads--;
      }
    }
    if (!ableToBuild) {
      return incrementResources(resourcesAtEnd, robotsAtEnd, timeLeft).geode;
    }
  } else {
    auto resourcesAtEnd = incrementResources(resources, robots, 1);
    int timeLeft = minutesLeft - 1;
    if (timeLeft == 0) {
      return resourcesAtEnd.geode;
    }

    if (costFeasible(resources, blueprint.oreRobotCost)) {
      scores.push_back(evaluate(blueprint, robots.buildOreRobot(), applyCost(resources, blueprint.oreRobotCost),
                                minutesLeft, numThreads));
    }
    if (costFeasible(resources, blueprint.clayRobotCost)) {
      scores.push_back(evaluate(blueprint, robots.buildClayRobot(), applyCost(resources, blueprint.clayRobotCost),
                                minutesLeft, numThreads));
    }
    if (costFeasible(resources, blueprint.obsidianRobotCost)) {
      scores.push_back(evaluate(blueprint, robots.buildObsidianRobot(),
                                applyCost(resources, blueprint.obsidianRobotCost), minutesLeft, numThreads));
    }
    if (costFeasible(resources, blueprint.geodeRobotCost)) {
      scores.push_back(evaluate(blueprint, robots.buildGeodeRobot(), applyCost(resources, blueprint.geodeRobotCost),
                                minutesLeft, numThreads));
    }
    if (scores.size() == 0) {
      scores.push_back(evaluate(blueprint, robots, resourcesAtEnd, timeLeft, numThreads));
    }
  }
  std::sort(scores.begin(), scores.end(), std::greater<int>());
  return scores.front();
}

int main(int argc, char** argv) {
  std::string line;
  std::vector<Blueprint> blueprints;
  while (std::getline(std::cin, line)) {
    auto outerElements = split(line, ':');
    auto innerLeft = split(outerElements[0], ' ');
    Blueprint blueprint;
    blueprint.id = std::stoi(innerLeft[1]);
    auto statements = split(outerElements[1], '.');
    for (const auto& statement : statements) {
      auto elements = split(statement, ' ');
      RobotCost cost;
      for (int i = 4; i < elements.size(); i += 3) {
        int costValue = std::stoi(elements[i]);
        auto costType = elements[i + 1];
        if (costType == "ore") {
          cost.oreCost = costValue;
        } else if (costType == "clay") {
          cost.clayCost = costValue;
        } else if (costType == "obsidian") {
          cost.obsidianCost = costValue;
        } else if (costType == "geode") {
          cost.geodeCost = costValue;
        } else {
          throw std::runtime_error("Dude what you smokin?");
        }
      }
      cost.parsed = true;
      std::string parsedType = elements[1];
      if (parsedType == "ore") {
        blueprint.oreRobotCost = cost;
      } else if (parsedType == "clay") {
        blueprint.clayRobotCost = cost;
      } else if (parsedType == "obsidian") {
        blueprint.obsidianRobotCost = cost;
      } else if (parsedType == "geode") {
        blueprint.geodeRobotCost = cost;
      } else {
        throw std::runtime_error("Dude what you smokin?");
      }
    }
    if (!blueprint.oreRobotCost.parsed || !blueprint.clayRobotCost.parsed || !blueprint.obsidianRobotCost.parsed ||
        !blueprint.geodeRobotCost.parsed) {
      throw std::runtime_error("Didn't parse a type or robot cost.");
    }
    blueprints.push_back(blueprint);
  }

  Robots robots;
  robots.oreRobots = 1;
  Resources resources;
  std::atomic_int numThreads = 0;
  int qualitySum = 0;
  for (const auto& blueprint : blueprints) {
    std::cout << "Evaluating blueprint " << blueprint.id << std::endl;
    auto geodes = evaluate(blueprint, robots, resources, 24, numThreads);
    std::cout << "Done evaluating, opened " << geodes << " with id " << blueprint.id << std::endl;
    qualitySum += geodes * blueprint.id;
  }
  // 1171 too low :(
  std::cout << "Quality level of all blueprints summed: " << qualitySum << std::endl;
}
