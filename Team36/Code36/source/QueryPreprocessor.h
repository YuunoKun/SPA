#pragma once

#include <iostream>
#include <string>
#include <vector>
#include "Query.h"
#include "QueryToken.h"

enum ParseStatus {
	NEUTRAL,
	IS_SELECTING,
	IS_SELECTING_MULTIPLE_CLAUSE,
};

class QueryPreprocessor {
public:

	// Constructor
	QueryPreprocessor();

	Query parse(std::string);

private:

	// Class variables
	Query query;

	QueryToken setIdentifierToQueryTokenType(QueryToken&, QueryToken&, QueryToken&, ParseStatus&, bool&);

	void addEntityToQuery(std::vector<QueryToken>&, QueryToken&, QueryToken&);

	void addSelectedToQuery(Entity&, std::vector<QueryToken>&, std::vector<QueryToken>, QueryToken&, bool&);

	void addPatternToQuery(Entity&, std::vector<QueryToken>&, QueryToken&);

	void setQueryParameter(QueryToken&, QueryToken&);

	void setPatternTypeEntity(std::vector<QueryToken>&, QueryToken&, Entity&);

	void validateQuery();

	Query returnAndResetQuery();
};
