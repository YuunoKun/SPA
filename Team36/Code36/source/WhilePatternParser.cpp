#include "WhilePatternParser.h"

void WhilePatternParser::parse(Query& query, Entity& entity, std::vector<QueryToken> token_chain) {
	Utility::isSyntacticValidpattern(token_chain);

	std::vector<std::vector<QueryToken>> separated_params = Utility::splitTokenChain(3, QueryToken::COMMA, token_chain);

	if (!Utility::isSemanticValidEntRef(query, separated_params[0], EntityType::VARIABLE) || 
		!Utility::isWildCard(separated_params[1]) ||
		separated_params[2].size() != 0) {
		query.setIsSemanticError("Invalid parameters for while");
	}

    if (!Utility::checkIsSemanticError(query)) {
        query.addPattern(Pattern(entity, Utility::setEntRef(query, separated_params[0], EntityType::VARIABLE)));
    }
}
