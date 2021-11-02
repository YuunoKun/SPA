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
	int parenthesis_counter;
	QueryToken nextToken;

	// Methods

	void handleDeclaration(QueryToken&);

	void handleSelection(QueryToken&);

	void handleIsSelecting(QueryToken&);

	void handleSelectingMultipleClause(QueryToken&);

	void handleWithinParameter(QueryToken&);

	void handleParenthesisOpen(QueryToken&);

	void handleAddParameterTokensAndParseWith(QueryToken&);

	void checkParseWith();

	void handleValidatePatternAndSuchThat(QueryToken&);

	void handleParenthesisClose(QueryToken&);

	void setIdentifierToQueryTokenType(QueryToken&);

	void addEntityToQuery(QueryToken&);

	void addSelectedToQuery(QueryToken&);

	void addPatternToQuery(QueryToken&);

	void setQueryParameter();

	void setPatternTypeEntity(std::vector<QueryToken>&, QueryToken&, Entity&);

	void validateQuery();

	Query returnAndResetQuery();
};
