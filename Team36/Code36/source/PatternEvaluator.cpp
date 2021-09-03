#include "PatternEvaluator.h"

void PatternEvaluator::evaluatePattern(QueryResult& queryResult, Pattern& pattern) {

	Entity patternType = pattern.getPatternType();
	Entity lhsEntity = pattern.getLeftExpression();

	TNode expression = pattern.getExpression();

	ResultTable* resultTable;

	//if lhs side is at least one side is declaration
	if (lhsEntity.isDeclaration()) {
		 if ( pattern.isWild()) {
			//If left side is declaration and right side is WILD: e.g a(x, _""_)
			//Todo

		}else{
			//If left side is declaration and right side is not wild: e.g a(x, "x")
			//Todo

		}
	}
	else {
		//if lhs side is not declaration
		if (lhsEntity.getType() == WILD && pattern.isWild()) {
			//If left side is wild and right side is WILD: e.g a(_, _"x"_)
			//Todo
		}else if (lhsEntity.getType() == WILD) {
			//If left side is wild and right side is normal: e.g a(_, "x")
			//Todo
		}else if (pattern.isWild()) {
			//If left side is constant and right side is WILD: e.g a("x", _"x"_)
			//Todo
		}
		else {
			//If left side is constant and right side is normal: e.g a(_, "x")
			//Todo
		}
	}

	if (resultTable->isEmpty()) {
		queryResult.setNoResult();
	}
	else {
		queryResult.addResult(*resultTable);
	}
}