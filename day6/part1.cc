#include <algorithm>
#include <deque>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_set>

// To run build the code and then do the following command:
// ```
// cat ../day5/in.txt | day5/day5part1
// ```

int main(int argc, char** argv) {
  char c;
  std::deque<char> chars;
  int loc = 0;
  while (std::cin.get(c)) {
    chars.push_back(c);
    loc++;
    if (chars.size() == 4) {
      bool found = true;
      for (int i = 0; i < chars.size(); ++i) {
        for (int j = i + 1; j < chars.size(); ++j) {
          if (chars[i] == chars[j]) {
            found = false;
            break;
          }
        }
      }
      if (found) {
        // Found the element.
        std::cout << "Location: " << loc << std::endl;
        return 0;
      }
      chars.pop_front();
    }
  }
  std::cout << "Didn't find a char." << std::endl;
  return 1;
}
