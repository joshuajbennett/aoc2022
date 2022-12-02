#include <iostream>
#include <string>

// To run build the code and then do the following command:
// ```
// cat ../day2/in.txt | day2/day2part1
// ```

enum class Selection { Rock, Paper, Scissors };

enum class Outcome { Lose, Draw, Win };

Selection getChoice(const Selection& opponent, const Outcome& result) {
  switch (result) {
  case Outcome::Lose:
    switch (opponent) {
    case Selection::Rock:
      return Selection::Scissors;
    case Selection::Paper:
      return Selection::Rock;
    case Selection::Scissors:
      return Selection::Paper;
    }
    break;
  case Outcome::Draw:
    return opponent;
  case Outcome::Win:
    switch (opponent) {
    case Selection::Rock:
      return Selection::Paper;
    case Selection::Paper:
      return Selection::Scissors;
    case Selection::Scissors:
      return Selection::Rock;
    }
    break;
  }
}

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

    Outcome result = [&]() {
      if (line[2] == 'X')
        return Outcome::Lose;
      if (line[2] == 'Y')
        return Outcome::Draw;
      if (line[2] == 'Z')
        return Outcome::Win;
      throw std::runtime_error("Invalid entry for result.");
    }();

    switch (result) {
    case Outcome::Lose:
      score += 0;
      break;
    case Outcome::Draw:
      score += 3;
      break;
    case Outcome::Win:
      score += 6;
      break;
    }

    Selection choice = getChoice(opponent, result);
    switch (choice) {
    case Selection::Rock:
      score += 1;
      break;
    case Selection::Paper:
      score += 2;
      break;
    case Selection::Scissors:
      score += 3;
      break;
    }
  }

  std::cout << "Total score: " << score << std::endl;

  return 0;
}
