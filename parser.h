#include <sstream>
#include <iostream>
#include <string>
#include <vector>
#include <deque>
#include <cstdio>
#include <cmath>
#include <functional>

class Token {
  public:
	enum Type {
	  Unknown = 0,
	  Number,
	  Operator,
	  LeftParenthesis,
	  RightParenthesis,
	};

	Token(Type t, const std::string& s, int prec = -1, bool ra = false)
	  : type {t}, str (s), precedence {prec}, rightAssociative {ra} {}

	Type type {Type::Unknown};
	std::string str {};
	int precedence {-1};
	bool rightAssociative {false};
};

std::deque<Token> tokenize(const std::string& expr) {
  std::deque<Token> tokens;

  for(const auto* p = expr.c_str(); *p; ++p) {
	if(isdigit(*p)) {
	  const auto* b = p;

	  for(; isdigit(*p); ++p) { ; }

	  const auto s = std::string(b, p);
	  tokens.push_back({Token::Type::Number, s});
	  --p;
	} else {
	  Token::Type t = Token::Type::Unknown;
	  int pr = -1;
	  bool ra = false;
	  switch(*p) {
		default: break;
		case '(': t = Token::Type::LeftParenthesis; break;
		case ')': t = Token::Type::RightParenthesis; break;
		case '^': t = Token::Type::Operator; pr = 4; ra = true; break;
		case '*': t = Token::Type::Operator; pr = 3; break;
		case '/': t = Token::Type::Operator; pr = 3; break;
		case '+': t = Token::Type::Operator; pr = 2; break;
		case '-': t = Token::Type::Operator; pr = 2; break;
	  }
	  tokens.push_back({t, std::string(1, *p), pr, ra});
	}
  }

  return tokens;
}


std::deque<Token> buildQueue(const std::deque<Token>& tokens) {
  std::deque<Token> queue;
  std::vector<Token> stack;

  for(auto &token : tokens) {
	switch(token.type) {
	  case Token::Type::Number:
		queue.push_back(token);
		break;

	  case Token::Type::Operator: {
		  const auto o1 = token;

		  while(!stack.empty()) {
			const auto o2 = stack.back();

			if((!o1.rightAssociative && o1.precedence <= o2.precedence) || (o1.rightAssociative && o1.precedence <  o2.precedence)) {
			  stack.pop_back();
			  queue.push_back(o2);
			  continue;
			}
			break;
		  }
		  stack.push_back(o1);
		}
		break;

	  case Token::Type::LeftParenthesis:
		stack.push_back(token);
		break;

	  case Token::Type::RightParenthesis: {
		  bool match = false;
		  while(!stack.empty() && stack.back().type != Token::Type::LeftParenthesis) {
			queue.push_back(stack.back());
			stack.pop_back();
			match = true;
		  }
		  stack.pop_back();

		  if(!match && stack.empty()) {
			return {};
		  }
		}
		break;

	  default:
		exit(0);
		break;
	}
  }

  while(! stack.empty()) {
	if(stack.back().type == Token::Type::LeftParenthesis) {
	  std::cout << "Mismatched parenthesis\n";
	  exit(0);
	}
	queue.push_back(std::move(stack.back()));
	stack.pop_back();
  }
  return queue;
}

double evaluate(std::deque<Token>& q) {
  std::vector<double> stack;

  while(!q.empty()) {
	const auto token = q.front();
	q.pop_front();

	switch(token.type) {
	  case Token::Type::Number:
		stack.push_back(std::stoi(token.str));
		break;

	  case Token::Type::Operator: {
		  const auto rhs = stack.back();
		  stack.pop_back();
		  const auto lhs = stack.back();
		  stack.pop_back();

		  switch(token.str[0]) {
			default:
			  exit(0);
			  break;
			case '^':
			  stack.push_back(pow(lhs, rhs));
			  break;
			case '*':
			  stack.push_back(lhs * rhs);
			  break;
			case '/':
			  stack.push_back(lhs / rhs);
			  break;
			case '+':
			  stack.push_back(lhs + rhs);
			  break;
			case '-':
			  stack.push_back(lhs - rhs);
			  break;
		  }
		}
		break;

	  default:
		std::cout << "Token Error\n";
		exit(0);
	}
  }
  return stack.back();
}
