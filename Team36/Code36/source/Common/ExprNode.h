#pragma once

#include <string>

typedef enum EXPR_SYMBOL {
	EXPR_PLUS,
	EXPR_MINUS,
	EXPR_MUL,
	EXPR_DIV,
	EXPR_MOD,
	EXPR_IDENTIFIER,
	EXPR_NULL
} ExprSymbol;

class ExprNode {
public:
	ExprNode();
	ExprNode(ExprSymbol);
	ExprNode(std::string);
	ExprNode(const ExprNode&);

	~ExprNode();

	void setSymbol(ExprSymbol);
	void setValue(std::string);
	void setLHS(ExprNode&);
	void setRHS(ExprNode&);

	ExprSymbol getSymbol() const;
	std::string getValue() const;
	ExprNode* getLHS() const;
	ExprNode* getRHS() const;

	bool contains(ExprNode*);
	bool equals(ExprNode*);

	bool operator==(const ExprNode&) const;
	ExprNode& operator=(const ExprNode&);

private:
	ExprSymbol symbol;
	std::string value;
	ExprNode* lhs{ nullptr };
	ExprNode* rhs{ nullptr };
};