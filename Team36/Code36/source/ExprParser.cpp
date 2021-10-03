#include "ExprParser.h"
#include <stdexcept>
#include <deque>

ExprNode* ExprParser::parse(std::string str) {
	if (str.size() == 0) {
		return nullptr;
	}

	std::deque<std::string> strs = sliceString(str);
	return parseExpression(strs);
}


std::deque<std::string>& ExprParser::sliceString(const std::string str) {
	std::deque<std::string> segments;
	segments.push_back(std::string());

	for (char c : str) {
		switch (c) {
		case '+':
		case '-':
		case '*':
		case '/':
		case '%':
		case '(':
		case ')':
			segments.push_back(std::string(c, 1));
			segments.push_back(std::string());
			break;
		case ' ':
		case '\t':
			continue;
			break;
		default:
			if (isalnum(c)) {
				segments.rbegin()->push_back(c);
			}
			else {
				throw std::runtime_error("Invalid symbol detected in expression.");
			}
			break;
		}
	}

	if (segments.rend()->size() == 0) {
		segments.pop_back();
	}

	return segments;
}


ExprNode* ExprParser::parseExpression(std::deque<std::string>& dq) {
	ExprNode* lhs = parseTerm(dq);

	while (!dq.empty()) {
		ExprNode* parent;
		if (*dq.begin()->begin() == '+') {
			parent = new ExprNode(ExprSymbol::EXPR_PLUS);
		}
		else if (*dq.begin()->begin() == '-') {
			parent = new ExprNode(ExprSymbol::EXPR_MINUS);
		}
		else {
			throw std::runtime_error("Syntax error.");
		}
		dq.pop_front();
		parent->setLHS(lhs);
		parent->setRHS(parseTerm(dq));
		lhs = parent;
	}

	return lhs;
}


ExprNode* ExprParser::parseTerm(std::deque<std::string>& dq) {
	ExprNode* lhs = parseFactor(dq);

	while (!dq.empty()) {
		ExprNode* parent;
		if (*dq.begin()->begin() == '*') {
			parent = new ExprNode(ExprSymbol::EXPR_MUL);
		}
		else if (*dq.begin()->begin() == '/') {
			parent = new ExprNode(ExprSymbol::EXPR_DIV);
		}
		else if (*dq.begin()->begin() == '%') {
			parent = new ExprNode(ExprSymbol::EXPR_MOD);
		}
		else {
			throw std::runtime_error("Syntax error.");
		}
		dq.pop_front();
		parent->setLHS(lhs);
		parent->setRHS(parseFactor(dq));
		lhs = parent;
	}

	return lhs;
}


ExprNode* ExprParser::parseFactor(std::deque<std::string>& dq) {
	if (!dq.empty() && *dq.begin() == "(") {
		dq.pop_front();
		ExprNode* node = parseExpression(dq);
		if (!dq.empty() && *dq.begin() != ")") {
			throw std::runtime_error("Expression missing close parenthesis.");
		}
		dq.pop_front();
		return node;
	}
	else if (!dq.begin()->empty() && isalnum(*dq.begin()->begin())) {
		ExprNode* node = new ExprNode(*dq.begin());
		dq.pop_front();
		return node;
	}
	else {
		throw std::runtime_error("Possible syntax error while parsing expression.");
	}
}
