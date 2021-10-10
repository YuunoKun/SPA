#include "PatternEvaluator.h"
#include <stdexcept>

void PatternEvaluator::evaluatePattern(QueryResult& queryResult, Pattern& pattern) {
	switch (pattern.getPatternType().getType()) {
	case ASSIGN: return evaluateAssignPattern(queryResult, pattern);
		break;
	case IF: return evaluateIfsPattern(queryResult, pattern);
		break;
	case WHILE: return evaluateWhilePattern(queryResult, pattern);
		break;
	default:
		throw std::invalid_argument("evaluatePattern(): only does not support this type");
	}
}

void PatternEvaluator::evaluateAssignPattern(QueryResult& queryResult, Pattern& pattern) {
	bool isWild = isExpressionWild(pattern);
	Entity patternType = pattern.getPatternType();
	Entity lhsEntity = pattern.getLeftExpression();

	if (lhsEntity.isSynonym()) {
		//if lhs side is at least is synonym e.g a(v, _"x"_), return 2 column table
		std::pair<Entity, Entity> header = { patternType, lhsEntity };
		std::vector<pattern_info> assignInfos;
		if (isWild) {
			assignInfos = pkb.getAssignInfo();
		}
		else {
			assignInfos = pkb.getAssignInfo(pattern.getExpression(), pattern.isWild());
		}

		ResultTable resultTable(header, assignInfos);
		queryResult.addResult(resultTable);
	}
	else {
		std::vector<stmt_index> table;
		if (isWild && lhsEntity.getType() == WILD) {
			//If left side is wild and right side is wild: e.g a(_,_)
			table = pkb.getAssignInfoFlitered();
		}
		else if (lhsEntity.getType() == WILD) {
			//If left side is wild and right side is expression: e.g a(_, "x")
			table = pkb.getAssignInfoFlitered(pattern.getExpression(), pattern.isWild());
		}
		else if (isWild) {
			//If left side is constant and right side is wild: e.g a("x", _)
			table = pkb.getAssignInfoFlitered(lhsEntity.getValue());
		} else {
			//If left side is constant and right side is expression: e.g a("x", "x")
			table = pkb.getAssignInfoFlitered(lhsEntity.getValue(), pattern.getExpression(), pattern.isWild());
		}
		ResultTable resultTable(patternType, table);
		queryResult.addResult(resultTable);
	}
}

void PatternEvaluator::evaluateIfsPattern(QueryResult& queryResult, Pattern& pattern) {
	Entity patternType = pattern.getPatternType();
	Entity lhsEntity = pattern.getLeftExpression();

	if (lhsEntity.isSynonym()) {
		//if lhs side is at least is synonym e.g ifs(v, _, _), return 2 column table
		std::pair<Entity, Entity> header = { patternType, lhsEntity };
		std::vector<std::pair<stmt_index, var_name>> table = pkb.getAllIfUses();

		ResultTable resultTable(header, table);
		queryResult.addResult(resultTable);
	}
	else {
		std::vector<stmt_index> table;
		if (lhsEntity.getType() == WILD) {
			//If left side is wild and right side is wild: e.g ifs(_, _, _)
			table = pkb.getIfUses();
		}
		else {
			//If left side is constant and right side is expression: e.g ifs("x", _, _)
			table = pkb.getIfUses(lhsEntity.getValue());
		}
		ResultTable resultTable(patternType, table);
		queryResult.addResult(resultTable);
	}
}

void PatternEvaluator::evaluateWhilePattern(QueryResult& queryResult, Pattern& pattern) {
	Entity patternType = pattern.getPatternType();
	Entity lhsEntity = pattern.getLeftExpression();

	if (lhsEntity.isSynonym()) {
		//if lhs side is at least is synonym e.g w(v, _), return 2 column table
		std::pair<Entity, Entity> header = { patternType, lhsEntity };
		std::vector<std::pair<stmt_index, var_name>> table = pkb.getAllWhileUses();

		ResultTable resultTable(header, table);
		queryResult.addResult(resultTable);
	}
	else {
		std::vector<stmt_index> table;
		if (lhsEntity.getType() == WILD) {
			//If left side is wild and right side is wild: e.g w(_, _)
			table = pkb.getWhileUses();
		}
		else {
			//If left side is constant and right side is expression: e.g w("x", _)
			table = pkb.getWhileUses(lhsEntity.getValue());
		}
		ResultTable resultTable(patternType, table);
		queryResult.addResult(resultTable);
	}
}

bool PatternEvaluator::isExpressionWild(Pattern& pattern) {
	return pattern.getExpression() == EXPR_NULL && pattern.isWild();
}
