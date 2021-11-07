#include "PatternWhileEvaluator.h"
#include "PQL/QueryObject/Pattern.h"
#include "PQL/QueryEvaluator/ResultTable.h"
#include "PKB/PKBAdapter.h"

ResultTable PatternWhileEvaluator::evaluateSynonym(Pattern& pattern) {
	return ResultTable({ pattern.getPatternType(), pattern.getLeftExpression() }, pkb.getAllWhileUses());
}

ResultTable PatternWhileEvaluator::evaluateWild(Pattern& pattern) {
	return ResultTable(pattern.getPatternType(), pkb.getWhileUses());
}

ResultTable PatternWhileEvaluator::evaluateConstant(Pattern& pattern) {
	return ResultTable(pattern.getPatternType(), pkb.getWhileUses(pattern.getLeftExpression().getValue()));
}