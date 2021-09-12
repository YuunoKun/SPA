#pragma once

#include <iostream>
#include <string>
#include <vector>
#include "Query.h"
#include "QueryToken.h"

class QueryPreprocessor {
public:

	// Constructor
	QueryPreprocessor();

	Query parse(std::string);

private:

	QueryToken setIdentifierToQueryTokenType(QueryToken&, QueryToken&, QueryToken&);

	void validateDeclarationQuery(QueryToken&, QueryToken&);
};