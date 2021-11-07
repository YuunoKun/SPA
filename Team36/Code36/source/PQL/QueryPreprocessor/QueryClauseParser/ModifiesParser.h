#pragma once

#include <iostream>
#include <string>
#include <vector>
#include "RelationParserInterface.h"

class ModifiesParser : public RelationParserInterface {
public:
    void parse(Query&, std::vector<QueryToken>, QueryToken::QueryTokenType&) override;
};