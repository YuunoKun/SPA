#pragma once

#include <iostream>
#include <string>
#include <vector>
#include "PatternParserInterface.h"

class WhilePatternParser : public PatternParserInterface {
public:
    void parse(Query&, Entity&, std::vector<QueryToken>) override;
};