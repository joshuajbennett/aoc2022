#include <atomic>
#include <cmath>
#include <deque>
#include <functional>
#include <iostream>
#include <list>
#include <mutex>
#include <optional>
#include <queue>
#include <sstream>
#include <string>
#include <thread>
#include <unordered_map>
#include <unordered_set>
#include <vector>

long long wrappedVal(const long long value, const long long size) {
  // long long returnVal = value;
  // while (returnVal <= -size) {
  //   returnVal = returnVal + size;
  // }
  // while (returnVal >= size) {
  //   returnVal = returnVal - size;
  // }
  // return returnVal;
  if (value > 0) {
    return value % size;
  } else if (value < 0) {
    return -(-value) % size;
  }
  return value;
}

struct Node {
  long long value = 0;
  long long actualValue = 0;
  Node* prev = nullptr;
  Node* next = nullptr;
  bool isFirst = false;
};

void moveForward(Node* node, Node* currentLoc, long long steps) {
  if (steps < 0) {
    throw std::runtime_error("Only positive steps supported on moveForward");
  } else if (steps > 0) {
    moveForward(node, currentLoc->next, steps - 1);
  } else {
    auto afterCurrentLoc = currentLoc->next;
    currentLoc->next = node;
    node->prev = currentLoc;
    afterCurrentLoc->prev = node;
    node->next = afterCurrentLoc;
  }
}

void moveBackward(Node* node, Node* currentLoc, long long steps) {
  if (steps < 0) {
    throw std::runtime_error("Only positive steps supported on moveBackward");
  } else if (steps > 0) {
    moveBackward(node, currentLoc->prev, steps - 1);
  } else {
    auto beforeCurrentLoc = currentLoc->prev;
    currentLoc->prev = node;
    node->next = currentLoc;
    beforeCurrentLoc->next = node;
    node->prev = beforeCurrentLoc;
  }
}

long long getVal(Node* currentLoc, long long steps) {
  if (steps > 0) {
    return getVal(currentLoc->next, steps - 1);
  } else if (steps < 0) {
    return getVal(currentLoc->prev, steps + 1);
  }
  return currentLoc->actualValue;
}

Node* findZero(Node* currentLoc) {
  if (currentLoc->value == 0) {
    return currentLoc;
  }
  return findZero(currentLoc->next);
}

void printRing(Node* currentLoc) {
  if (currentLoc->next->isFirst) {
    std::cout << currentLoc->actualValue << std::endl;
    return;
  }
  std::cout << currentLoc->actualValue << ", ";
  printRing(currentLoc->next);
}

void applyWrappedValue(Node* current, const long long ringSize) {
  current->value = wrappedVal(current->actualValue, ringSize);
  if (current->next->isFirst) {
    return;
  }
  applyWrappedValue(current->next, ringSize);
}

int main(int argc, char** argv) {
  std::string line;
  std::vector<std::unique_ptr<Node>> nodes;
  bool firstNode = true;
  bool foundZero = false;
  long long decryptionKey = 811589153;
  while (std::getline(std::cin, line)) {
    if (firstNode) {
      nodes.push_back(std::make_unique<Node>());
      nodes.back()->actualValue = std::stoi(line) * decryptionKey;
      if (nodes.back()->actualValue == 0) {
        if (foundZero) {
          throw std::runtime_error("Already found zero.");
        }
        foundZero = true;
      }
      nodes.back()->isFirst = true;
      firstNode = false;
    } else {
      auto last = nodes.back().get();
      nodes.push_back(std::make_unique<Node>());
      nodes.back()->actualValue = std::stoi(line) * decryptionKey;
      if (nodes.back()->actualValue == 0) {
        if (foundZero) {
          throw std::runtime_error("Already found zero.");
        }
        foundZero = true;
      }
      nodes.back()->prev = last;
      last->next = nodes.back().get();
    }
  }
  if (!foundZero) {
    throw std::runtime_error("Didn't find a zero.");
  }

  nodes.front()->prev = nodes.back().get();
  nodes.back()->next = nodes.front().get();
  Node* first = nodes.front().get();
  auto ringSize = nodes.size() - 1;

  applyWrappedValue(first, ringSize);

  for (int j = 0; j < 10; ++j) {
    for (int i = 0; i < nodes.size(); ++i) {
      long long move = nodes[i]->value;
      if (move > 0) {
        if (nodes[i]->isFirst) {
          nodes[i]->next->isFirst = true;
          nodes[i]->isFirst = false;
          first = nodes[i]->next;
        }
        auto next = nodes[i]->next;
        auto prev = nodes[i]->prev;
        moveForward(nodes[i].get(), nodes[i].get(), move);
        prev->next = next;
        next->prev = prev;
      } else if (move < 0) {
        if (nodes[i]->isFirst) {
          nodes[i]->next->isFirst = true;
          nodes[i]->isFirst = false;
          first = nodes[i]->next;
        }
        auto next = nodes[i]->next;
        auto prev = nodes[i]->prev;
        moveBackward(nodes[i].get(), nodes[i].get(), -move);
        prev->next = next;
        next->prev = prev;
      }
    }
    std::cout << "After " << j + 1 << " round of mixing:" << std::endl;
  }

  auto zeroNode = findZero(first);

  auto one = getVal(zeroNode, wrappedVal(1000, ringSize + 1));
  auto two = getVal(zeroNode, wrappedVal(2000, ringSize + 1));
  auto three = getVal(zeroNode, wrappedVal(3000, ringSize + 1));
  std::cout << "First: " << one << " Second: " << two << " Third: " << three << std::endl;
  auto summed = one + two + three;

  std::cout << "Summed: " << summed << std::endl;
}
