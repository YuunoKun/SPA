#pragma once

#include <iostream>
#include <string>
#include <vector>
#include "Query.h"
#include "QueryToken.h"

enum ParseStatus {
	NEUTRAL,
	IS_SELECTING,
	IS_SELECTING_MULTIPLE_CLAUSE,
};

class QueryPreprocessor {
public:

	// Constructor
	QueryPreprocessor();

	Query parse(std::string);

	// For Query System to reset query for invalid queries
	void resetQuery();

private:

	// Class variables
	Query query;
	bool isSelect;
	bool isParameter;
	bool isExpectingPatternType;
	bool isExpectingAttribute;
	bool endOfCurrentClauses;
	std::vector<QueryToken> output;
	std::vector<QueryToken> selected;
	std::vector<QueryToken> parameterClause;
	QueryToken prevToken;
	QueryToken declarationType;
	bool haveNextDeclaration;
	bool endOfCurrentDeclaration = true;
	QueryToken prevTokenSelect;
	QueryToken queryParameter;
	QueryToken patternOrSuchThat;
	Entity patternTypeEntity;
	ParseStatus status;

	// Methods

	void setIdentifierToQueryTokenType(QueryToken&, ParseStatus&);

	void addEntityToQuery(QueryToken&);

	void addSelectedToQuery(Entity&, QueryToken&);

	void addPatternToQuery(QueryToken&);

	void setQueryParameter();

	void setPatternTypeEntity(std::vector<QueryToken>&, QueryToken&, Entity&);

	void validateQuery();

	Query returnAndResetQuery();
};
