#pragma once

#include <iostream>
#include <string>
#include <vector>
#include "WithParserInterface.h"
#include "Entity.h"
#include "Query.h"
#include "QueryToken.h"
#include "SemanticErrorException.h"
#include "SyntacticErrorException.h"
#include "Utility.h"

class WithParser : public WithParserInterface {
public:
    void parse(Query&, std::vector<QueryToken>) override;
private:
    static bool isSameRefType(Query&, std::vector<QueryToken>, std::vector<QueryToken>);
    EntityType getEntityType(Query&, std::vector<QueryToken>);
    AttrRef getAttrName(Query&, std::vector<QueryToken>);
};
