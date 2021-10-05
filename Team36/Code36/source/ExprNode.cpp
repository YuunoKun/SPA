#include "ExprNode.h"

ExprNode::ExprNode(ExprSymbol sym) {
	symbol = sym;
}

ExprNode::ExprNode(std::string val) {
	symbol = ExprSymbol::EXPR_IDENTIFIER;
	value = val;
}

ExprNode::~ExprNode() {
	if (lhs) lhs->~ExprNode();
	if (rhs) rhs->~ExprNode();
}

void ExprNode::setSymbol(ExprSymbol sym) {
	symbol = sym;
}

void ExprNode::setValue(std::string val) {
	value = val;
}

void ExprNode::setLHS(ExprNode* node) {
	lhs = node;
}

void ExprNode::setRHS(ExprNode* node) {
	rhs = node;
}

ExprSymbol ExprNode::getSymbol() const {
	return symbol;
}

std::string ExprNode::getValue() const {
	return value;
}

ExprNode* ExprNode::getLHS() const {
	return lhs;
}

ExprNode* ExprNode::getRHS() const {
	return rhs;
}

bool ExprNode::contains(ExprNode* expr) {
	if (!expr) {
		return false;
	}

	if (this->equals(expr)) {
		return true;
	}
	else {
		return (lhs && lhs->contains(expr)) || (rhs && rhs->contains(expr));
	}
}

bool ExprNode::equals(ExprNode* expr) {
	if (this == nullptr && expr == nullptr) {
		return true;
	}

	if (expr && symbol == expr->getSymbol() && value == expr->getValue()) {
		if (lhs == expr->getLHS() && rhs == expr->getRHS()) {
			return true;
		}

		if ((lhs && !expr->getLHS()) || (!lhs && expr->getLHS()) || (rhs && !expr->getRHS()) || (!rhs && expr->getRHS())) {
			return false;
		}
		else {
			if (lhs == expr->getLHS()) {
				return rhs->equals(expr->getRHS());
			}
			else if (rhs == expr->getRHS()) {
				return lhs->equals(expr->getLHS());
			}
			else {
				return lhs->equals(expr->getLHS()) && rhs->equals(expr->getRHS());
			}
		}
	}

	return false;
}