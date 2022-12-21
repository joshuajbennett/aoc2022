#include "common.h"

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
