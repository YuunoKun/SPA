#include <stdexcept>
#include <cassert>
#include "ExprParser.h"


ExprNode* ExprParser::parse(std::string str) {
	if (str.size() == 0) {
		return nullptr;
	}

	std::deque<std::string> strs = sliceString(str);
	if (strs.size() == 0) {
		return nullptr;
	}
	else {
		ExprNode* res = parseExpression(strs);
		if (strs.size() == 0) {
			return res;
		}
		else {
			throw std::runtime_error("Syntax error while parsing expression.");
		}
	}
}


std::deque<std::string> ExprParser::sliceString(const std::string str) {
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
			if (segments.rbegin()->size() == 0) {
				segments.pop_back();
			}
			segments.push_back(std::string(1, c));
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

	if (segments.rbegin()->size() == 0) {
		segments.pop_back();
	}

	return segments;
}


ExprNode* ExprParser::parseExpression(std::deque<std::string>& dq) {
	ExprNode* lhs = parseTerm(dq);

	while (!dq.empty()) {
		ExprNode* parent;
		if (*dq.begin() == "+") {
			parent = new ExprNode(ExprSymbol::EXPR_PLUS);
		}
		else if (*dq.begin() == "-") {
			parent = new ExprNode(ExprSymbol::EXPR_MINUS);
		}
		else if (*dq.begin() == "(") {
			protectedPopFront(dq);
			ExprNode* node = parseExpression(dq);
			protectedPopFront(dq, ")");
			parent->setLHS(lhs);
			parent->setRHS(node);
			lhs = parent;
			continue;
		}
		else {
			break;
		}
		protectedPopFront(dq);
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
		if (*dq.begin() == "*") {
			parent = new ExprNode(ExprSymbol::EXPR_MUL);
		}
		else if (*dq.begin() == "/") {
			parent = new ExprNode(ExprSymbol::EXPR_DIV);
		}
		else if (*dq.begin() == "%") {
			parent = new ExprNode(ExprSymbol::EXPR_MOD);
		}
		else if (*dq.begin() == "(") {
			protectedPopFront(dq);
			ExprNode* node = parseExpression(dq);
			protectedPopFront(dq, ")");
			parent->setLHS(lhs);
			parent->setRHS(node);
			lhs = parent;
			continue;
		}
		else {
			break;
		}
		protectedPopFront(dq);
		parent->setLHS(lhs);
		parent->setRHS(parseFactor(dq));
		lhs = parent;
	}

	return lhs;
}


ExprNode* ExprParser::parseFactor(std::deque<std::string>& dq) {
	if (dq.empty()) {
		throw std::runtime_error("Unexpected end of expression.");
	}

	if (!dq.begin()->empty() && isalnum(*dq.begin()->begin())) {
		ExprNode* node = new ExprNode(*dq.begin());
		protectedPopFront(dq);
		return node;
	}
	else if (*dq.begin() == "(") {
		protectedPopFront(dq);
		ExprNode* node = parseExpression(dq);
		protectedPopFront(dq, ")");
		return node;
	}
	else {
		throw std::runtime_error("Possible syntax error while parsing expression.");
	}
}


void ExprParser::protectedPopFront(std::deque<std::string>& dq) {
	if (dq.empty()) {
		throw std::runtime_error("Unexpected end of expression.");
	}
	else {
		dq.pop_front();
	}
}


void ExprParser::protectedPopFront(std::deque<std::string>& dq, std::string str) {
	if (dq.empty()) {
		throw std::runtime_error("Unexpected end of expression.");
	}
	else if(*dq.begin() != str) {
		throw std::runtime_error("Invalid expression.");
	}
	else {
		dq.pop_front();
	}
}