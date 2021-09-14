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

	//void parseParameterSuchThat(Query&, QueryToken::QueryTokenType, std::vector<QueryToken>);

	//void parseParameterPattern(Query&, Entity&, std::vector<QueryToken>);

	//Entity setStmtRef(Query&, QueryToken);

	//Entity setEntRef(Query&, std::vector<QueryToken>);

	//expr setExpr(std::vector<QueryToken>);

private:

	QueryToken setIdentifierToQueryTokenType(QueryToken&, QueryToken&, QueryToken&);

	void validateDeclarationQuery(QueryToken&, QueryToken&);

	void addEntityToQuery(Query&, Entity&, std::vector<QueryToken>&, QueryToken&, QueryToken&);

	void addSelectedToQuery(Query&, Entity&, std::vector<QueryToken>&, std::vector<QueryToken>, QueryToken&, QueryToken&, bool&);

	void setQueryParameter(QueryToken&, QueryToken&);

	void setPatternTypeEntity(std::vector<QueryToken>&, QueryToken&, Entity&);
};