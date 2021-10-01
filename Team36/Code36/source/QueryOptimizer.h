#pragma once
#include <list>
#include "Clause.h"

class QueryOptimizer {
public:
	static std::list<Clause> optimizeClauses(std::list<Clause>&);
};