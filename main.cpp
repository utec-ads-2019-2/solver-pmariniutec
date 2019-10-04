#include <iostream>
#include "parser.h"

int main () {

  Parser p {};
  auto result = p.evaluate("((7*3)/4+6*(3^2)/2)");

  std::cout << "Result: " << result << '\n';

  return 0;
}
