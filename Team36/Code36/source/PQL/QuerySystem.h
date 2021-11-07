#pragma once

#include "PQL/QueryObject/Query.h"
#include "PQL/QueryPreprocessor/QueryPreprocessor.h"
#include "QueryOptimizer.h"
#include "PQL/QueryEvaluator/QueryEvaluator.h"

class QuerySystem {
public:
	std::list<std::string> processQuery(std::string);
private:
	std::list<std::string> returnEmptyResult(Entity&);
};