#pragma once

#include "Query.h"
#include "Entity.h"
#include "QueryToken.h"

class WithParserInterface {
public:
    virtual void parse(Query&, std::vector<QueryToken>) = 0;
};