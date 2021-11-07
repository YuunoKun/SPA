#pragma once

#include "PQL/QueryObject/Query.h"
#include "PQL/QueryPreprocessor/QueryToken.h"

class WithParserInterface {
public:
    virtual void parse(Query&, std::vector<QueryToken>) = 0;
};