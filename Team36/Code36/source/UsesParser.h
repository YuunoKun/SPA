#pragma once

#include <iostream>
#include <string>
#include <vector>
#include "RelationParserInterface.h"
#include "Entity.h"
#include "Query.h"
#include "QueryToken.h"
#include "QueryTokenizer.h"
#include "SemanticErrorException.h"
#include "SyntacticErrorException.h"
#include "Utility.h"

class UsesParser : public RelationParserInterface {
public:
    void parse(Query&, std::vector<QueryToken>) override;
};