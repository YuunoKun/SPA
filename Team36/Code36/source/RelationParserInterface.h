#pragma once

#include "Query.h"
#include "Entity.h"
#include "QueryToken.h"

class RelationParserInterface {
public:
    virtual void parse(Query&, Entity&, std::vector<QueryToken>) = 0;
};