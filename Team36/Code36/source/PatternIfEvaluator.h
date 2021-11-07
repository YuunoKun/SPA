#pragma once

#include "PatternEvaluatorInterface.h"
#include "Query.h"
#include "QueryResult.h"
#include "PKBAdapter.h"

class PatternIfEvaluator : public PatternEvaluatorInterface {
public:
	ResultTable evaluateSynonym(Pattern&) override;
	ResultTable evaluateWild(Pattern&) override;
	ResultTable evaluateConstant(Pattern&) override;
};
