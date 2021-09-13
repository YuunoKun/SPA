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

	void parseParameterSuchThat(Query& query, QueryToken::QueryTokenType token_type, std::vector<QueryToken> token_chain);

	void parseParameterPattern(Query& query, Entity& synonym, std::vector<QueryToken> token_chain);

	Entity setStmtRef(Query& query, QueryToken token);

	Entity setEntRef(Query& query, std::vector<QueryToken> token_chain);

	expr setExpr(std::vector<QueryToken> token_chain);

private:

	QueryToken setIdentifierToQueryTokenType(QueryToken&, QueryToken&, QueryToken&);

	void validateDeclarationQuery(QueryToken&, QueryToken&);

	void addEntityToQuery(Query&, Entity&, std::vector<QueryToken>&, QueryToken&, QueryToken&);

	void addSelectedToQuery(Query&, Entity&, std::vector<QueryToken>&, std::vector<QueryToken>, QueryToken&, QueryToken&, bool&);
};