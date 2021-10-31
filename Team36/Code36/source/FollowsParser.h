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

class FollowsParser : public RelationParserInterface {
public:
    void parse(Query&, std::vector<QueryToken>) override;
};
