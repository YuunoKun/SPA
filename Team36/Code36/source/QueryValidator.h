#pragma once
#include "QueryToken.h"
#include "SemanticErrorException.h"
#include "SyntacticErrorException.h"

class QueryValidator {
public:

	// Constructor
	QueryValidator();

	// Methods
	void validateSelectMultipleClauses(QueryToken&, QueryToken&);

	void validateQuery(Query&, bool&);

	void validateSelecting(QueryToken&, QueryToken&);
};