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
#include "FollowsParser.h"
#include "FollowsTParser.h"
#include "ParentParser.h"
#include "ParentTParser.h"
#include "CallsParser.h"
#include "CallsTParser.h"
#include "NextParser.h"
#include "NextTParser.h"
#include "AffectsParser.h"
#include "AffectsTParser.h"
#include "AssignPatternParser.h"
#include "IfPatternParser.h"
#include "WhilePatternParser.h"
#include "WithParser.h"

class QueryPatternRelRefParser {
public: 
	QueryPatternRelRefParser();
	void parseParameterSuchThat(Query&, QueryToken::QueryTokenType, std::vector<QueryToken>);
	void parseParameterPattern(Query&, Entity&, std::vector<QueryToken>);
	void parseWith(Query&, std::vector<QueryToken>);
};
