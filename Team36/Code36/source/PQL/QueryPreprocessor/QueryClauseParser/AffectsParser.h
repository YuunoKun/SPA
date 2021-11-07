#pragma once

#include <iostream>
#include <string>
#include <vector>
#include "RelationParserInterface.h"

class AffectsParser : public RelationParserInterface {
public:
    void parse(Query&, std::vector<QueryToken>, QueryToken::QueryTokenType&) override;
};