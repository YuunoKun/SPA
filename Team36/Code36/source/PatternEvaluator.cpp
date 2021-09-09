#include "PatternEvaluator.h"
#include <stdexcept>

void PatternEvaluator::evaluatePattern(QueryResult& queryResult, Pattern& pattern) {
	if (pattern.getPatternType().getType() != ASSIGN) {
		throw std::invalid_argument("evaluatePattern(): only ASSIGN pattern type is allowed for iteration 1");
	}

	Entity patternType = pattern.getPatternType();
	Entity lhsEntity = pattern.getLeftExpression();

	TNode expression = pattern.getExpression();

	ResultTable* resultTable;

	//if lhs side is at least is declaration, return 2 column table
	if (lhsEntity.isSynonym()) {

	} else {
		//if lhs side is not declaration
		if (lhsEntity.getType() == WILD){
			//If left side is wild and right side is WILD: e.g a(_, _"x"_)
			//return all assign statment that match expression
			//Todo
		} else {
			//If left side is constant and right side is normal: e.g a("x", "x")
			//return all assign statment that match expression and lhs
		}
	}

	if (resultTable->isEmpty()) {
		queryResult.setNoResult();
	}
	else {
		queryResult.addResult(*resultTable);
	}
}