#include <iostream>
#include "parser.h"

int main () {

  Parser p {};

  std::string equation;
  std::cin >> equation;
// Falta procesar variables
  auto result = p.evaluate(equation);

  std::cout << "Result: " << result << '\n';

  return 0;
}
