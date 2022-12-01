#include <iostream>
#include <string>

// To run build the code and then do the following command:
// ```
// cat ../day1/in.txt | day1/part1
// ```

int main(int argc, char** argv) {

  int maxVal = 0;
  int currentVal = 0;
  std::string line;
  while (std::getline(std::cin, line)) {
    if (line == "") {
      if (currentVal > maxVal) {
        maxVal = currentVal;
      }
      currentVal = 0;
      continue;
    }
    auto val = std::stoi(line);
    currentVal += val;
  }

  std::cout << "max summed value: " << maxVal << std::endl;

  return 0;
}
