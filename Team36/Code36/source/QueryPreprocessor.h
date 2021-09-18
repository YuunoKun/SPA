#pragma once

#include <iostream>
#include <string>
#include <vector>
#include "Query.h"
#include "QueryToken.h"

class QueryPreprocessor {
public:

	// Constructor
	QueryPreprocessor();

	Query parse(std::string);

private:

	QueryToken setIdentifierToQueryTokenType(QueryToken&, QueryToken&, QueryToken&);

	void validateDeclarationQuery(QueryToken&, QueryToken&);

	void addEntityToQuery(Query&, Entity&, std::vector<QueryToken>&, QueryToken&, QueryToken&);

	void addSelectedToQuery(Query&, Entity&, std::vector<QueryToken>&, std::vector<QueryToken>, QueryToken&, bool&);

	void addPatternToQuery(Entity&, std::vector<QueryToken>&, QueryToken&, QueryToken&);

	void setQueryParameter(QueryToken&, QueryToken&);

	void setPatternTypeEntity(std::vector<QueryToken>&, QueryToken&, Entity&);

	void validateQuery(Query&);
};
