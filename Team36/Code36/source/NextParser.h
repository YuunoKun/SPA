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

class NextParser {
public:
    NextParser();

    void parseNext(Query&, std::vector<QueryToken>);

};