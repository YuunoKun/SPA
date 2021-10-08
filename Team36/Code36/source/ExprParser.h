#pragma once

#include <vector>
#include <deque>
#include "ExprNode.h"

class ExprParser {
public:
	ExprNode parse(std::string);

private:
	std::deque<std::string> sliceString(const std::string);
	ExprNode parseExpression(std::deque<std::string>&);
	ExprNode parseTerm(std::deque<std::string>&);
	ExprNode parseFactor(std::deque<std::string>&);

	void protectedPopFront(std::deque<std::string>&);
	void protectedPopFront(std::deque<std::string>&, std::string);
};