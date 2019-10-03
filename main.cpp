#include <iostream>
#include "parser.h"

int main () {
  const std::string input = "((7*3)/4+6*(3^2)/2)";

  const auto tokens = tokenize(input);
  auto q = buildQueue(tokens);
  auto result = evaluate(q);

  std::cout << "Result: " << result << '\n';

  return 0;
}
