#pragma once
#include <string>
#include <vector>
#include "QueryToken.h"
#include "SemanticErrorException.h"
#include "SyntacticErrorException.h"
#include "Utility.h"
#include "Common.h"

class QueryTokenizer
{
public:
	
	//Constructor
	QueryTokenizer();

	//Tokenize for parser to convert to Entity/Pattern/RelRef
	void tokenize(Query&, std::string);

	std::vector<QueryToken> get_query_token_chain();

private:

	std::vector<QueryToken> query_token_cache;

	void handleDigit(Query&, char, QueryToken&);
	void add_query_token(Query&, QueryToken& query_token);
};
