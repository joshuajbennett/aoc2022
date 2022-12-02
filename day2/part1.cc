#include <iostream>
#include <string>

// To run build the code and then do the following command:
// ```
// cat ../day2/in.txt | day2/day2part1
// ```

enum class Selection { Rock, Paper, Scissors };

int main(int argc, char** argv) {

  std::string line;
  int score = 0;

  while (std::getline(std::cin, line)) {
    if (line.size() < 3) {
      std::cerr << "Bad line encountered." << std::endl;
      return -1;
    }
    Selection opponent = [&]() {
      if (line[0] == 'A')
        return Selection::Rock;
      if (line[0] == 'B')
        return Selection::Paper;
      if (line[0] == 'C')
        return Selection::Scissors;
      throw std::runtime_error("Invalid opponent entry.");
    }();

    Selection mine = [&]() {
      if (line[2] == 'X')
        return Selection::Rock;
      if (line[2] == 'Y')
        return Selection::Paper;
      if (line[2] == 'Z')
        return Selection::Scissors;
      throw std::runtime_error("Invalid entry for mine.");
    }();

    switch (mine) {
    case Selection::Rock:
      score += 1;
      switch (opponent) {
      case Selection::Rock:
        score += 3;
        break;
      case Selection::Paper:
        score += 0;
        break;
      case Selection::Scissors:
        score += 6;
        break;
      }
      break;
    case Selection::Paper:
      score += 2;
      switch (opponent) {
      case Selection::Rock:
        score += 6;
        break;
      case Selection::Paper:
        score += 3;
        break;
      case Selection::Scissors:
        score += 0;
        break;
      }
      break;
    case Selection::Scissors:
      score += 3;
      switch (opponent) {
      case Selection::Rock:
        score += 0;
        break;
      case Selection::Paper:
        score += 6;
        break;
      case Selection::Scissors:
        score += 3;
        break;
      }
      break;
    }
  }

  std::cout << "Total score: " << score << std::endl;

  return 0;
}
