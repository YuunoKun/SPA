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

	TNode setExpr(std::vector<QueryToken> token_chain);
};