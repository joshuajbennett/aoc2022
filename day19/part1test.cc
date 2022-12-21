#include "common.h"
#include <gtest/gtest.h>

TEST(Day19Part1Test1, no_args) {
  Robots robots{.oreRobots = 1, .clayRobots = 0, .obsidianRobots = 0, .geodeRobots = 0};
  Resources resources{.ore = 0, .clay = 0, .obsidian = 0, .geode = 0};
  RobotCost cost{.oreCost = 4, .clayCost = 0, .obsidianCost = 0, .geodeCost = 0};
  auto timeReq = timeRequired(robots, resources, cost);
  EXPECT_EQ(timeReq.has_value(), true);
  EXPECT_EQ(timeReq.value(), 4);
}

TEST(Day19Part1Test2, no_args) {
  Robots robots{.oreRobots = 1, .clayRobots = 2, .obsidianRobots = 0, .geodeRobots = 0};
  Resources resources{.ore = 0, .clay = 0, .obsidian = 0, .geode = 0};
  RobotCost cost{.oreCost = 4, .clayCost = 12, .obsidianCost = 0, .geodeCost = 0};
  auto timeReq = timeRequired(robots, resources, cost);
  EXPECT_EQ(timeReq.has_value(), true);
  EXPECT_EQ(timeReq.value(), 6);
}

TEST(Day19Part1Test3, no_args) {
  Robots robots{.oreRobots = 1, .clayRobots = 2, .obsidianRobots = 0, .geodeRobots = 0};
  Resources resources{.ore = 0, .clay = 0, .obsidian = 0, .geode = 0};
  RobotCost cost{.oreCost = 4, .clayCost = 11, .obsidianCost = 0, .geodeCost = 0};
  auto timeReq = timeRequired(robots, resources, cost);
  EXPECT_EQ(timeReq.has_value(), true);
  EXPECT_EQ(timeReq.value(), 6);
}

TEST(Day19Part1Test4, no_args) {
  Robots robots{.oreRobots = 1, .clayRobots = 2, .obsidianRobots = 0, .geodeRobots = 0};
  Resources resources{.ore = 0, .clay = 0, .obsidian = 0, .geode = 0};
  RobotCost cost{.oreCost = 4, .clayCost = 13, .obsidianCost = 0, .geodeCost = 0};
  auto timeReq = timeRequired(robots, resources, cost);
  EXPECT_EQ(timeReq.has_value(), true);
  EXPECT_EQ(timeReq.value(), 7);
}

TEST(Day19Part1Test5, no_args) {
  Robots robots{.oreRobots = 1, .clayRobots = 2, .obsidianRobots = 0, .geodeRobots = 0};
  Resources resources{.ore = 0, .clay = 1, .obsidian = 0, .geode = 0};
  RobotCost cost{.oreCost = 4, .clayCost = 13, .obsidianCost = 0, .geodeCost = 0};
  auto timeReq = timeRequired(robots, resources, cost);
  EXPECT_EQ(timeReq.has_value(), true);
  EXPECT_EQ(timeReq.value(), 6);
}

TEST(Day19Part1Test6, no_args) {
  Robots robots{.oreRobots = 1, .clayRobots = 2, .obsidianRobots = 0, .geodeRobots = 0};
  Resources resources{.ore = 0, .clay = 1, .obsidian = 0, .geode = 0};
  RobotCost cost{.oreCost = 4, .clayCost = 13, .obsidianCost = 0, .geodeCost = 0};
  auto timeReq = timeRequired(robots, resources, cost);
  EXPECT_EQ(timeReq.has_value(), true);
  EXPECT_EQ(timeReq.value(), 6);
}
