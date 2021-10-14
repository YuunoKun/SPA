#pragma once

#include "PatternEvaluatorInterface.h"
#include "Query.h"
#include "QueryResult.h"
#include "PKBAdapter.h"


class PatternsEvaluator {
public:
	void evaluatePattern(QueryResult&, Pattern&);
private:
	void evaluatePattern(QueryResult&, Pattern&, PatternEvaluatorInterface&);

    PKBAdapter pkb;
};