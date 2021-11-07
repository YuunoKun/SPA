#pragma once

#include <iostream>
#include <string>
#include <vector>
#include "PatternParserInterface.h"

class IfPatternParser : public PatternParserInterface {
public:
    void parse(Query&, Entity&, std::vector<QueryToken>) override;
};