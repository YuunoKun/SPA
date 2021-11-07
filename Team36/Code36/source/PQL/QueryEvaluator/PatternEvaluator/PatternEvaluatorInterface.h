#pragma once

#include "PQL/QueryObject/Query.h"
#include "PQL/QueryEvaluator/QueryResult.h"
#include "PKB/PKBAdapter.h"

class PatternEvaluatorInterface {
public:
	virtual ResultTable evaluateSynonym(Pattern&) = 0;
	virtual ResultTable evaluateWild(Pattern&) = 0;
	virtual ResultTable evaluateConstant(Pattern&) = 0;
protected:
	PKBAdapter pkb;
};