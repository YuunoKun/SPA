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
#include "UsesParser.h"
#include "ModifiesParser.h"
#include "NextParser.h"
#include "NextTParser.h"
#include "AssignPatternParser.h"
#include "IfPatternParser.h"
#include "WhilePatternParser.h"



class QueryPatternRelRefParser {
public: 
	QueryPatternRelRefParser();
	
	bool isStmtRef(Query&, std::vector<QueryToken>);
	bool isEntRef(Query&, std::vector<QueryToken>);
	bool isLineRef(Query&, std::vector<QueryToken>);
	bool isExpr(std::vector<QueryToken>);
	bool isCorrectSynEntRef(Query&, std::vector<QueryToken>, EntityType);
	bool isWildCard(std::vector<QueryToken>);
	Entity setStmtRef(Query&, QueryToken);
	Entity setEntRef(Query&, std::vector<QueryToken>, EntityType);
	Entity setLineRef(Query&, QueryToken);
	std::string setExpr(std::vector<QueryToken>);
	void parseParameterSuchThat(Query&, QueryToken::QueryTokenType, std::vector<QueryToken>);
	void parseParameterPattern(Query&, Entity&, std::vector<QueryToken>);

};
