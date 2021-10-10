#pragma once

#include "Query.h"
#include "QueryResult.h"
#include "PKBAdapter.h"
#include "Common.h"

class PatternEvaluator {
public:
	void evaluatePattern(QueryResult&, Pattern&);

private:

	void evaluateAssignPattern(QueryResult&, Pattern&);
	void evaluateIfsPattern(QueryResult&, Pattern&);
	void evaluateWhilePattern(QueryResult&, Pattern&);

	bool isExpressionWild(Pattern&);

	PKBAdapter pkb;
};