#pragma once
#include <string>
#include <vector>
#include "QueryToken.h"
#include "SemanticErrorException.h"
#include "SyntacticErrorException.h"
#include "Utility.h"

class QueryTokenizer
{
public:
	
	//Constructor
	QueryTokenizer();

	//Tokenize for parser to convert to Entity/Pattern/RelRef
	void parse_into_query_tokens(std::string);

	std::vector<QueryToken> get_query_token_chain();

private:

	std::vector<QueryToken> query_token_cache;

	void add_query_token(QueryToken& query_token);
};
