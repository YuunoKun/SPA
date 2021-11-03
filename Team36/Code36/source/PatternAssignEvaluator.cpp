
#include "PatternAssignEvaluator.h"
#include "Query.h"
#include "QueryResult.h"
#include "PKBAdapter.h"


ResultTable PatternAssignEvaluator::evaluateSynonym(Pattern& pattern) {
	std::pair<Entity, Entity> header = { pattern.getPatternType(), pattern.getLeftExpression() };
	if (isExpressionWild(pattern)) {
		return ResultTable(header, pkb.getAssignInfo());
	} else {
		return ResultTable(header, pkb.getAssignInfo(pattern.getExpression(), pattern.isWild()));
	}
}

ResultTable PatternAssignEvaluator::evaluateWild(Pattern& pattern) {
	Entity header = pattern.getPatternType();
	if (isExpressionWild(pattern)) {
		return ResultTable(header, pkb.getAssignInfoFiltered());
	} else {
		return ResultTable(header, pkb.getAssignInfoFiltered(pattern.getExpression(), pattern.isWild()));
	}
}

ResultTable PatternAssignEvaluator::evaluateConstant(Pattern& pattern) {
	Entity header = pattern.getPatternType();
	var_name var = pattern.getLeftExpression().getValue();
	if (isExpressionWild(pattern)) {
		return ResultTable(header, pkb.getAssignInfoFiltered(var));
	} else {
		return ResultTable(header, pkb.getAssignInfoFiltered(var, pattern.getExpression(), pattern.isWild()));
	}
}

bool PatternAssignEvaluator::isExpressionWild(Pattern& pattern) {
	return pattern.getExpression() == EXPR_NULL && pattern.isWild();
}