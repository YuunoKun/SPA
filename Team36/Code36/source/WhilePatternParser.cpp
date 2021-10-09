#include "WhilePatternParser.h"
#include "QueryPatternRelRefParser.h"


WhilePatternParser::WhilePatternParser() {}

void WhilePatternParser::parseWhile(Query& query, Entity& entity, std::vector<QueryToken> token_chain) {
    QueryPatternRelRefParser parser;
    std::vector<QueryToken> temp_token_chain_1;
    std::vector<QueryToken> temp_token_chain_2;
    bool comma_found = false;
    size_t token_chain_size = token_chain.size();
    for (size_t i = 0; i < token_chain_size; i++) {
        if (token_chain[0].type == QueryToken::COMMA) {
            token_chain.erase(token_chain.begin());
            comma_found = true;
        }
        else if (!comma_found) {
            // 1st param
            temp_token_chain_1.push_back(token_chain[0]);
            token_chain.erase(token_chain.begin());
        }
        else {
            // 2nd param
            temp_token_chain_2.push_back(token_chain[0]);
            token_chain.erase(token_chain.begin());
        }
    }

    if (!parser.isEntRef(query, temp_token_chain_1) || !parser.isWildCard(temp_token_chain_2)) {
        throw SemanticErrorException("Invalid parameters for while pattern");
    }

    if (!parser.isCorrectSynEntRef(query, temp_token_chain_1, EntityType::VARIABLE)) {
        throw SemanticErrorException("Invalid parameters for while");
    }

    query.addPattern(Pattern(entity, parser.setEntRef(query, temp_token_chain_1),
        "", true));
}
