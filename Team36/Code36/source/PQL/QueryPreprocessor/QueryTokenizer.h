#pragma once
#include <string>
#include <vector>
#include "QueryToken.h"
#include "Common/Common.h"
#include "Common/SemanticErrorException.h"
#include "Common/SyntacticErrorException.h"
#include "PQL/Helper/Utility.h"

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
	void handleAlpha(Query&, char, QueryToken&);
	void handleChar(Query&, char, QueryToken&);
	void handleSpace(Query&, bool&, QueryToken&);
	void handleQuotation(Query&, bool&, QueryToken&);
	void handleRelrefStr(Query&, QueryToken&);
	void addQueryToken(Query&, QueryToken& query_token);
};
