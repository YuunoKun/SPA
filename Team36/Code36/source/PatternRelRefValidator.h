#pragma once

#include <iostream>
#include <string>
#include <vector>
#include "Query.h"
#include "QueryToken.h"


class PatternRelRefValidator {
private:

	bool PatternRelRefValidator::isStmtRef(Query& query, std::vector<QueryToken> token_chain);

	bool PatternRelRefValidator::isCommaRef(std::vector<QueryToken> token_chain);

	bool PatternRelRefValidator::isEntRef(std::vector<QueryToken> token_chain);

public: 

	PatternRelRefValidator();

	void parseParameterSuchThat(Query&, QueryToken::QueryTokenType, std::vector<QueryToken>);

	void parseParameterPattern(Query&, Entity&, std::vector<QueryToken>);

	Entity setStmtRef(Query&, QueryToken);

	Entity setEntRef(Query&, std::vector<QueryToken>);

	expr setExpr(std::vector<QueryToken>);
};
