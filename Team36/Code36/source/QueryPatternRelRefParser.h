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
#include "NextParser.h"
#include "AssignPatternParser.h"


class QueryPatternRelRefParser {
public: 
	QueryPatternRelRefParser();
	
	bool isStmtRef(Query&, std::vector<QueryToken>);
	bool isEntRef(Query&, std::vector<QueryToken>);
	bool isLineRef(Query&, std::vector<QueryToken>);
	bool isExpr(std::vector<QueryToken>);
	bool isCorrectSynEntRef(Query&, std::vector<QueryToken>, EntityType);
	Entity setStmtRef(Query&, QueryToken);
	Entity setEntRef(Query&, std::vector<QueryToken>);
	Entity setLineRef(Query&, QueryToken);
	Entity setCallEntRef(Query&, std::vector<QueryToken>);
	std::string setExpr(std::vector<QueryToken>);
	void parseParameterSuchThat(Query&, QueryToken::QueryTokenType, std::vector<QueryToken>);
	void parseParameterPattern(Query&, Entity&, std::vector<QueryToken>);

private:
	bool isCommaRef(std::vector<QueryToken>);

};
