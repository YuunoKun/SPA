#pragma once

#include "Query.h"
#include "QueryResult.h"
#include "PKBAdapter.h"

class PatternEvaluatorInterface {
public:
	virtual ResultTable evaluateSynonym(Pattern&) = 0;
	virtual ResultTable evaluateWild(Pattern&) = 0;
	virtual ResultTable evaluateConstant(Pattern&) = 0;
protected:
	PKBAdapter pkb;
};