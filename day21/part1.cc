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

enum class Operator { Number, Add, Subtract, Divide, Multiply };
struct NumberOrOperation {
  Operator oper;
  // Applicable if this is a number.
  int number = 0;
  // Applicable if this is an operation.
  std::string lhs;
  std::string rhs;
};

Operator parseOperator(const std::string& op) {
  if (op == "+") {
    return Operator::Add;
  } else if (op == "-") {
    return Operator::Subtract;
  } else if (op == "/") {
    return Operator::Divide;
  } else if (op == "*") {
    return Operator::Multiply;
  }
  throw std::runtime_error("Invalid input");
}

long long retrieveResult(const std::unordered_map<std::string, NumberOrOperation>& ops, const std::string name) {
  auto findName = ops.find(name);
  if (findName == ops.end()) {
    throw std::runtime_error("Name not in map.");
  }
  switch (findName->second.oper) {
  case Operator::Number:
    return findName->second.number;
  case Operator::Add:
    return retrieveResult(ops, findName->second.lhs) + retrieveResult(ops, findName->second.rhs);
  case Operator::Subtract:
    return retrieveResult(ops, findName->second.lhs) - retrieveResult(ops, findName->second.rhs);
  case Operator::Multiply:
    return retrieveResult(ops, findName->second.lhs) * retrieveResult(ops, findName->second.rhs);
  case Operator::Divide:
    return retrieveResult(ops, findName->second.lhs) / retrieveResult(ops, findName->second.rhs);
  }
}

long long getLeft(const std::unordered_map<std::string, NumberOrOperation>& ops) {
  auto findName = ops.find("root");
  if (findName == ops.end()) {
    throw std::runtime_error("Name not in map.");
  }
  return retrieveResult(ops, findName->second.lhs);
}

bool containsHumn(const std::unordered_map<std::string, NumberOrOperation>& ops, const std::string name) {
  auto findName = ops.find(name);
  if (findName == ops.end()) {
    throw std::runtime_error("Name not in map.");
  }
  if (findName->second.oper == Operator::Number) {
    return false;
  }
  if (findName->second.lhs == "humn") {
    return true;
  }
  if (findName->second.rhs == "humn") {
    return true;
  }
  if (containsHumn(ops, findName->second.lhs)) {
    return true;
  }
  if (containsHumn(ops, findName->second.rhs)) {
    return true;
  }
}

long long deriveHumn(const std::unordered_map<std::string, NumberOrOperation>& ops, const std::string& name,
                     const long long equals) {
  if (name == "humn") {
    return equals;
  }
  auto findName = ops.find(name);
  if (findName == ops.end()) {
    throw std::runtime_error("Name not in map.");
  }
  if (containsHumn(ops, findName->second.lhs)) {
    auto rightSide = retrieveResult(ops, findName->second.rhs);
    long long newEquals = equals;
    switch (findName->second.oper) {
    case Operator::Number:
      throw std::runtime_error("Encountered a number when we shouldn't have.");
    case Operator::Add:
      newEquals -= rightSide;
    case Operator::Subtract:
      newEquals += rightSide;
    case Operator::Multiply:
      newEquals /= rightSide;
    case Operator::Divide:
      newEquals *= rightSide;
    }
    return deriveHumn(ops, findName->second.lhs, newEquals);
  } else {
    auto leftSide = retrieveResult(ops, findName->second.lhs);
    long long newEquals = equals;
    switch (findName->second.oper) {
    case Operator::Number:
      throw std::runtime_error("Encountered a number when we shouldn't have.");
    case Operator::Add:
      newEquals -= leftSide;
    case Operator::Subtract:
      newEquals = leftSide - newEquals;
    case Operator::Multiply:
      newEquals /= leftSide;
    case Operator::Divide:
      newEquals = leftSide / newEquals;
    }
    return deriveHumn(ops, findName->second.rhs, newEquals);
  }
}

long long deriveLeft(const std::unordered_map<std::string, NumberOrOperation>& ops, const long long right) {
  auto findName = ops.find("root");
  if (findName == ops.end()) {
    throw std::runtime_error("Name not in map.");
  }
  return deriveHumn(ops, findName->second.lhs, right);
}

long long getRight(const std::unordered_map<std::string, NumberOrOperation>& ops) {
  auto findName = ops.find("root");
  if (findName == ops.end()) {
    throw std::runtime_error("Name not in map.");
  }
  return retrieveResult(ops, findName->second.rhs);
}

bool checkEqual(const std::unordered_map<std::string, NumberOrOperation>& ops) {
  auto findName = ops.find("root");
  if (findName == ops.end()) {
    throw std::runtime_error("Name not in map.");
  }
  return retrieveResult(ops, findName->second.lhs) == retrieveResult(ops, findName->second.rhs);
}

int main(int argc, char** argv) {
  std::string line;
  std::unordered_map<std::string, NumberOrOperation> ops;
  while (std::getline(std::cin, line)) {
    auto outerElements = split(line, ':');
    std::string name = outerElements[0];
    auto innerElements = split(outerElements[1], ' ');
    if (innerElements.size() > 1) {
      // parse an operation
      ops[name] = NumberOrOperation{parseOperator(innerElements[1]), 0, innerElements[0], innerElements[2]};
    } else {
      // parse a value
      ops[name] = NumberOrOperation{Operator::Number, std::stoi(innerElements[0]), "", ""};
    }
  }
  auto num = retrieveResult(ops, "root");
  std::cout << "answer: " << num << std::endl;
  return 0;
}
