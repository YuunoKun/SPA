#pragma once

#include <iostream>
#include <string>
#include <vector>
#include "Entity.h"
#include "Query.h"
#include "QueryToken.h"
#include "QueryTokenizer.h"
#include "SemanticErrorException.h"
#include "SyntacticErrorException.h"
#include "Utility.h"

class PatternRelRefValidator {
private:
	static ExprParser expr_parser;

	bool isStmtRef(Query&, std::vector<QueryToken>);

	bool isCommaRef(std::vector<QueryToken>);

	bool isEntRef(Query&, std::vector<QueryToken>);

	bool isCorrectSynEntRef(Query&, std::vector<QueryToken>, EntityType);

public:

	PatternRelRefValidator();

	void parseParameterSuchThat(Query&, QueryToken::QueryTokenType, std::vector<QueryToken>);

	void parseParameterPattern(Query&, Entity&, std::vector<QueryToken>);

	Entity setStmtRef(Query&, QueryToken);

	Entity setEntRef(Query&, std::vector<QueryToken>);

	Entity setCallEntRef(Query&, std::vector<QueryToken>);

	std::string setExpr(std::vector<QueryToken>);
};
