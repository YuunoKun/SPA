#pragma once

#include "Common/SemanticErrorException.h"
#include "Common/SyntacticErrorException.h"
#include "PQL/Helper/Utility.h"
#include "PQL/QueryObject/Query.h"
#include "PQL/QueryObject/Entity.h"
#include "PQL/QueryPreprocessor/QueryToken.h"

class PatternParserInterface {
public:
    virtual void parse(Query&, Entity&, std::vector<QueryToken>) = 0;
};
