#pragma once

#include <iostream>
#include <string>
#include <vector>
#include "Query.h"
#include "QueryToken.h"


class PatternRelRefValidator {
private:

	bool PatternRelRefValidator::isStmtRef(Query&, std::vector<QueryToken>);

	bool PatternRelRefValidator::isCommaRef(std::vector<QueryToken>);

	bool PatternRelRefValidator::isEntRef(Query&, std::vector<QueryToken>);

public: 

	PatternRelRefValidator();

	void parseParameterSuchThat(Query&, QueryToken::QueryTokenType, std::vector<QueryToken>);

	void parseParameterPattern(Query&, Entity&, std::vector<QueryToken>);

	Entity setStmtRef(Query&, QueryToken);

	Entity setEntRef(Query&, std::vector<QueryToken>);

	expr setExpr(std::vector<QueryToken>);
};
