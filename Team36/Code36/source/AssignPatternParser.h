#pragma once

#include <iostream>
#include <string>
#include <vector>
#include "PatternParserInterface.h"
#include "Entity.h"
#include "Query.h"
#include "QueryToken.h"
#include "QueryTokenizer.h"
#include "SemanticErrorException.h"
#include "SyntacticErrorException.h"
#include "Utility.h"

class AssignPatternParser : public PatternParserInterface {
public:
    void parse(Query&, Entity&, std::vector<QueryToken>) override;
private:
    bool isWild(std::vector<QueryToken>);
};