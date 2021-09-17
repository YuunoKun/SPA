#pragma once

#include <iostream>
#include <string>
#include <vector>
#include "Query.h"
#include "QueryToken.h"


class PatternRelRefValidator {
public: 

	PatternRelRefValidator();

	void parseParameterSuchThat(Query&, QueryToken::QueryTokenType, std::vector<QueryToken>);

	void parseParameterPattern(Query&, Entity&, std::vector<QueryToken>);

	Entity setStmtRef(Query&, QueryToken);

	Entity setEntRef(Query&, std::vector<QueryToken>);

	expr setExpr(std::vector<QueryToken>);
};
