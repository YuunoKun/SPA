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

	Entity isStmtRef(Query& query, QueryToken token);

	Entity isEntRef(Query& query, std::vector<QueryToken> token_chain);

	TNode isExpr(std::vector<QueryToken> token_chain);
};