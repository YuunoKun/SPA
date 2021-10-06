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

class AssignPatternParser {
public:
    AssignPatternParser();

    void parseParameterAssign(Query&, Entity&, std::vector<QueryToken>);

private:
    bool isWild(std::vector<QueryToken>);
};