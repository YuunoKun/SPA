#include "ExprNode.h"


ExprNode::ExprNode(ExprSymbol sym) {
	symbol = sym;
}


ExprNode::ExprNode(std::string val) {
	symbol = ExprSymbol::EXPR_IDENTIFIER;
	value = val;
}


ExprNode::ExprNode(const ExprNode& to_copy) {
	symbol = to_copy.symbol;
	value = to_copy.value;
	if (to_copy.lhs) {
		lhs = new ExprNode(*to_copy.lhs);
	}
	if (to_copy.rhs) {
		rhs = new ExprNode(*to_copy.rhs);
	}
}


ExprNode::~ExprNode() {
	delete lhs;
	delete rhs;
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


ExprSymbol ExprNode::getSymbol() {
	return symbol;
}


std::string ExprNode::getValue() {
	return value;
}


ExprNode* ExprNode::getLHS() {
	return lhs;
}


ExprNode* ExprNode::getRHS() {
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
			else if(rhs == expr->getRHS()) {
				return lhs->equals(expr->getLHS());
			}
			else {
				return lhs->equals(expr->getLHS()) && rhs->equals(expr->getRHS());
			}
		}
	}

	return false;
}


ExprNode& ExprNode::operator=(const ExprNode& to_copy) {
	return ExprNode(to_copy);
}