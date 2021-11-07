#pragma once

#include <iostream>
#include <string>
#include <vector>
#include "PatternParserInterface.h"
#include "PQL/QueryPreprocessor/QueryTokenizer.h"

class AssignPatternParser : public PatternParserInterface {
public:
    void parse(Query&, Entity&, std::vector<QueryToken>) override;
private:
    bool isWild(std::vector<QueryToken>);
};