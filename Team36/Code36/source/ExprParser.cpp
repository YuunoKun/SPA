#include <stdexcept>
#include <cassert>
#include "ExprParser.h"
#include "SyntacticErrorException.h"


ExprNode ExprParser::parse(std::string str) {
	if (str.size() == 0) {
		return ExprNode(EXPR_NULL);
	}

	std::deque<std::string> strs = sliceString(str);
	if (strs.size() == 0) {
		return ExprNode(EXPR_NULL);
	}
	else {
		ExprNode res = parseExpression(strs);
		if (strs.size() == 0) {
			return res;
		}
		else {
			throw SyntacticErrorException("ExprParser failure : Syntax error while parsing expression.");
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
				throw SyntacticErrorException("ExprParser failure : Invalid symbol detected in expression.");
			}
			break;
		}
	}

	if (segments.rbegin()->size() == 0) {
		segments.pop_back();
	}

	return segments;
}


ExprNode ExprParser::parseExpression(std::deque<std::string>& dq) {
	ExprNode lhs = parseTerm(dq);

	while (!dq.empty()) {
		ExprNode parent;
		if (*dq.begin() == "+") {
			parent = ExprNode(ExprSymbol::EXPR_PLUS);
		}
		else if (*dq.begin() == "-") {
			parent = ExprNode(ExprSymbol::EXPR_MINUS);
		}
		else if (*dq.begin() == "(") {
			protectedPopFront(dq);
			ExprNode node = parseExpression(dq);
			protectedPopFront(dq, ")");
			parent.setLHS(lhs);
			parent.setRHS(node);
			lhs = parent;
			continue;
		}
		else {
			break;
		}
		protectedPopFront(dq);
		parent.setLHS(lhs);
		parent.setRHS(parseTerm(dq));
		lhs = parent;
	}

	return lhs;
}


ExprNode ExprParser::parseTerm(std::deque<std::string>& dq) {
	ExprNode lhs = parseFactor(dq);

	while (!dq.empty()) {
		ExprNode parent;
		if (*dq.begin() == "*") {
			parent = ExprNode(ExprSymbol::EXPR_MUL);
		}
		else if (*dq.begin() == "/") {
			parent = ExprNode(ExprSymbol::EXPR_DIV);
		}
		else if (*dq.begin() == "%") {
			parent = ExprNode(ExprSymbol::EXPR_MOD);
		}
		else if (*dq.begin() == "(") {
			protectedPopFront(dq);
			ExprNode node = parseExpression(dq);
			protectedPopFront(dq, ")");
			parent.setLHS(lhs);
			parent.setRHS(node);
			lhs = parent;

			continue;
		}
		else {
			break;
		}
		protectedPopFront(dq);
		parent.setLHS(lhs);
		parent.setRHS(parseFactor(dq));
		lhs = parent;
	}

	return lhs;
}


ExprNode ExprParser::parseFactor(std::deque<std::string>& dq) {
	if (dq.empty()) {
		throw SyntacticErrorException("ExprParser failure : Unexpected end of expression.");
	}

	if (!dq.begin()->empty() && isalnum(*dq.begin()->begin())) {
		ExprNode node = ExprNode(*dq.begin());
		protectedPopFront(dq);
		return node;
	}
	else if (*dq.begin() == "(") {
		protectedPopFront(dq);
		ExprNode node = parseExpression(dq);
		protectedPopFront(dq, ")");
		return node;
	}
	else {
		throw SyntacticErrorException("ExprParser failure : Possible syntax error while parsing expression.");
	}
}


void ExprParser::protectedPopFront(std::deque<std::string>& dq) {
	if (dq.empty()) {
		throw SyntacticErrorException("ExprParser failure : Unexpected end of expression.");
	}
	else {
		dq.pop_front();
	}
}


void ExprParser::protectedPopFront(std::deque<std::string>& dq, std::string str) {
	if (dq.empty()) {
		throw SyntacticErrorException("ExprParser failure : Unexpected end of expression.");
	}
	else if(*dq.begin() != str) {
		throw SyntacticErrorException("ExprParser failure : Invalid expression.");
	}
	else {
		dq.pop_front();
	}
}