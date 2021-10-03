#pragma once

#include <string>

typedef enum EXPR_SYMBOL {
	EXPR_PLUS,
	EXPR_MINUS,
	EXPR_MUL,
	EXPR_DIV,
	EXPR_MOD,
	EXPR_IDENTIFIER
} ExprSymbol;


class ExprNode {
public:

	ExprNode(ExprSymbol);

	ExprNode(std::string);

	~ExprNode();

	void setSymbol(ExprSymbol);
	void setValue(std::string);
	void setLHS(ExprNode*);
	void setRHS(ExprNode*);

	ExprSymbol getSymbol();
	std::string getValue();
	ExprNode* getLHS();
	ExprNode* getRHS();

	bool contains(ExprNode*);
	bool equals(ExprNode*);

private:
	ExprSymbol symbol;
	std::string value{""};
	ExprNode* lhs{nullptr};
	ExprNode* rhs{nullptr};
};