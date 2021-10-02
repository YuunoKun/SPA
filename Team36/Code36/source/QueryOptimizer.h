#pragma once
#include <list>
#include "Clause.h"

class QueryOptimizer {
public:
	static std::vector<Clause> optimizeClauses(std::vector<Clause>&);
};