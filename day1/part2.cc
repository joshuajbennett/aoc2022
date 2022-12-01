#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

// To run build the code and then do the following command:
// ```
// cat ../day1/in.txt | day1/part2
// ```

int main(int argc, char** argv) {

  std::vector<int> values;
  int currentVal = 0;
  std::string line;
  while (std::getline(std::cin, line)) {
    if (line == "") {
      values.push_back(currentVal);
      currentVal = 0;
      continue;
    }
    auto val = std::stoi(line);
    currentVal += val;
  }
  std::sort(values.begin(), values.end(), std::greater<int>());

  int summedVal = 0;
  for (int i = 0; i < values.size(); i++) {
    std::cout << "Value at " << i << " is << " << values[i] << std::endl;
    summedVal += values[i];
    if (i == 2) {
      break;
    }
  }

  std::cout << "Summation of max three values: " << summedVal << std::endl;

  return 0;
}
