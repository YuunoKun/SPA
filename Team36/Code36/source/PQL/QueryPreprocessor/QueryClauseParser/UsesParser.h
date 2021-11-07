#pragma once

#include <iostream>
#include <string>
#include <vector>
#include "RelationParserInterface.h"

class UsesParser : public RelationParserInterface {
public:
    void parse(Query&, std::vector<QueryToken>, QueryToken::QueryTokenType&) override;
};