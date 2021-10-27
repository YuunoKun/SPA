#include "WhilePatternParser.h"

void WhilePatternParser::parse(Query& query, Entity& entity, std::vector<QueryToken> token_chain) {
    std::vector<QueryToken> temp_token_chain_1;
    std::vector<QueryToken> temp_token_chain_2;
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
        }
        else if (comma_count == 1) {
            // 2nd param
            temp_token_chain_2.push_back(token_chain[0]);
            token_chain.erase(token_chain.begin());
        } else {
            throw SyntacticErrorException("Invalid parameters for while pattern");
        }
    }

	if (!parser.isEntRef(query, temp_token_chain_1) || !parser.isWildCard(temp_token_chain_2)) {
		query.setIsSemanticError("Invalid parameters for while pattern");
	}

	if (!parser.isCorrectSynEntRef(query, temp_token_chain_1, EntityType::VARIABLE)) {
		query.setIsSemanticError("Invalid parameters for while");
	}

    query.addPattern(Pattern(entity, Utility::setEntRef(query, temp_token_chain_1, EntityType::VARIABLE)));
}
