
#include "PatternIfEvaluator.h"
#include "Query.h"
#include "QueryResult.h"
#include "PKBAdapter.h"

ResultTable PatternIfEvaluator::evaluateSynonym(Pattern& pattern) {
    std::pair<Entity, Entity> header = { pattern.getPatternType(), pattern.getLeftExpression() };
    return ResultTable(header, pkb.getAllIfUses());
}

ResultTable PatternIfEvaluator::evaluateWild(Pattern& pattern) {
    Entity header = pattern.getPatternType();
    return ResultTable(header, pkb.getIfUses());
}

ResultTable PatternIfEvaluator::evaluateConstant(Pattern& pattern) {
    Entity header = pattern.getPatternType();
    return ResultTable(header, pkb.getIfUses(pattern.getLeftExpression().getValue()));
}

