#pragma once

#include <iostream>
#include <string>
#include <vector>
#include "Entity.h"
#include "Query.h"
#include "QueryToken.h"
#include "SemanticErrorException.h"
#include "SyntacticErrorException.h"
#include "Utility.h"
#include "UsesParser.h"
#include "ModifiesParser.h"
#include "FollowsParser.h"
#include "ParentParser.h"
#include "CallsParser.h"
#include "NextParser.h"
#include "AffectsParser.h"
#include "AssignPatternParser.h"
#include "IfPatternParser.h"
#include "WhilePatternParser.h"
#include "WithParser.h"

class QueryClauseParser {
public: 
	QueryClauseParser();
	void parseParameterSuchThat(Query&, QueryToken::QueryTokenType, std::vector<QueryToken>);
	void parseParameterPattern(Query&, Entity&, std::vector<QueryToken>);
	void parseWith(Query&, std::vector<QueryToken>);
};
