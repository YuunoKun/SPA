#include "IfPatternParser.h"

void IfPatternParser::parse(Query& query, Entity& entity, std::vector<QueryToken> token_chain) {
    std::vector<std::vector<QueryToken>> separated_params = Utility::splitTokenChain(3, QueryToken::COMMA, token_chain);

    if (!Utility::isEntRef(query, separated_params[0]) ||
        !Utility::isWildCard(separated_params[1]) ||
        !Utility::isWildCard(separated_params[2])) {
        query.setIsSemanticError("Invalid parameters for if pattern");
    }

    if (!Utility::isCorrectSynEntRef(query, separated_params[0], EntityType::VARIABLE)) {
        query.setIsSemanticError("Invalid parameters for if");
    }
    if (!Utility::checkIsSemanticError(query)) {
        query.addPattern(Pattern(entity, Utility::setEntRef(query, separated_params[0], EntityType::VARIABLE)));
    }
}
