#pragma once
#include "QueryToken.h"
#include "Common/SemanticErrorException.h"
#include "Common/SyntacticErrorException.h"
#include "PQL/QueryObject/Query.h"

class QueryValidator {
public:

	QueryValidator();
	void validateSelectMultipleClauses(QueryToken&, QueryToken&);
	void validateQuery(Query&, bool&);
	void validateSelecting(QueryToken&, QueryToken&);
	void validatePatternType(Entity&, Query&);
	void validateAnd(QueryToken&, QueryToken&, std::vector<QueryToken>&);
	void validateAttributeType(Query&, QueryToken&, QueryToken&);
	void isExpectingIdentifier(QueryToken&);
	void validateNotSuchThat(QueryToken&);
};