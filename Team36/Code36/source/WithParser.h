#pragma once

#include <iostream>
#include <string>
#include <vector>
#include "RelationParserInterface.h"
#include "Entity.h"
#include "Query.h"
#include "QueryToken.h"
#include "SemanticErrorException.h"
#include "SyntacticErrorException.h"
#include "Utility.h"

class WithParser {
public:
    void parse(Query&, std::vector<QueryToken>);
private:
    static bool isSameRefType(Query&, std::vector<QueryToken>, std::vector<QueryToken>);
    EntityType getEntityType(Query&, std::vector<QueryToken>);
    AttrRef getAttrName(Query&, std::vector<QueryToken>);
};
