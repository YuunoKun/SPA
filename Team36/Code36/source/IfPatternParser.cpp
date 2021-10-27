#include "IfPatternParser.h"

void IfPatternParser::parse(Query& query, Entity& entity, std::vector<QueryToken> token_chain) {
    std::vector<QueryToken> temp_token_chain_1;
    std::vector<QueryToken> temp_token_chain_2;
    std::vector<QueryToken> temp_token_chain_3;
    int comma_count = 0;
    size_t token_chain_size = token_chain.size();
    for (size_t i = 0; i < token_chain_size; i++) {
        if (token_chain[0].type == QueryToken::COMMA) {
            token_chain.erase(token_chain.begin());
            comma_count ++;
        }
        else if (comma_count == 0) {
            // 1st param
            temp_token_chain_1.push_back(token_chain[0]);
            token_chain.erase(token_chain.begin());
        } else if (comma_count == 1) {
            // 2nd param
            temp_token_chain_2.push_back(token_chain[0]);
            token_chain.erase(token_chain.begin());
        } else if (comma_count == 2) {
            // 3rd param
            temp_token_chain_3.push_back(token_chain[0]);
            token_chain.erase(token_chain.begin());
        } else {
            query.setIsSemanticError("Invalid parameters for if pattern");
        }
    }

    if (!Utility::isEntRef(query, temp_token_chain_1) ||
        !Utility::isWildCard(temp_token_chain_2) ||
        !Utility::isWildCard(temp_token_chain_3)) {
        query.setIsSemanticError("Invalid parameters for if pattern");
    }

    if (!Utility::isCorrectSynEntRef(query, temp_token_chain_1, EntityType::VARIABLE)) {
        query.setIsSemanticError("Invalid parameters for if");
    }

    query.addPattern(Pattern(entity, Utility::setEntRef(query, temp_token_chain_1, EntityType::VARIABLE)));
}
