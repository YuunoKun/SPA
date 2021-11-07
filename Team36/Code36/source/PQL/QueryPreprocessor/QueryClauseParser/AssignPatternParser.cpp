#include "AssignPatternParser.h"

bool AssignPatternParser::isWild(std::vector<QueryToken> token_chain) {
	return token_chain[0].type == QueryToken::WILDCARD;
}

void AssignPatternParser::parse(Query& query, Entity& entity, std::vector<QueryToken> token_chain) {
    Utility::isSyntacticValidpattern(token_chain);
    bool is_wild = false;
    std::vector<std::vector<QueryToken>> separated_params = Utility::splitTokenChain(3, QueryToken::COMMA, token_chain);

    if (!Utility::isSemanticValidEntRef(query, separated_params[0], EntityType::VARIABLE) || 
        !Utility::isExpr(separated_params[1]) ||
        separated_params[2].size() != 0) {
        query.setIsSemanticError("Invalid parameters for assign pattern");
    }

	is_wild = isWild(separated_params[1]);
    if (!Utility::checkIsSemanticError(query)) {
        query.addPattern(Pattern(entity, Utility::setEntRef(query, separated_params[0], EntityType::VARIABLE),
            Utility::setExpr(separated_params[1]), is_wild));
    }
}
