#pragma once

#include <iostream>
#include <string>
#include <vector>

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
#include "Common/SemanticErrorException.h"
#include "Common/SyntacticErrorException.h"
#include "PQL/Helper/Utility.h"
#include "PQL/QueryObject/Query.h"
#include "PQL/QueryObject/Entity.h"
#include "PQL/QueryPreprocessor/QueryToken.h"

class QueryClauseParser {
public: 
	QueryClauseParser();
	void parseParameterSuchThat(Query&, QueryToken::QueryTokenType, std::vector<QueryToken>);
	void parseParameterPattern(Query&, Entity&, std::vector<QueryToken>);
	void parseWith(Query&, std::vector<QueryToken>);
};
