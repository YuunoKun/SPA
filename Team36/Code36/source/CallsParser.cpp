#include "CallsParser.h"

void CallsParser::parse(Query& query, std::vector<QueryToken> token_chain) {
    // entRef , entRef
    std::vector<std::vector<QueryToken>> separated_params = Utility::splitTokenChain(2, QueryToken::COMMA, token_chain);

    if (!Utility::isEntRef(query, separated_params[0]) || !Utility::isEntRef(query, separated_params[1])) {
        query.setIsSemanticError("Invalid parameters for Calls");
    }
    // Check if both args are PROCEDURE
    if (!Utility::isCorrectSynEntRef(query, separated_params[0], EntityType::PROCEDURE) ||
        !Utility::isCorrectSynEntRef(query, separated_params[1], EntityType::PROCEDURE)) {
        query.setIsSemanticError("Invalid parameters for Calls");
    }

    if (!Utility::checkIsSemanticError(query)) {
        query.addRelation(RelRef(RelType::CALLS,
            Utility::setEntRef(query, separated_params[0], EntityType::PROCEDURE),
            Utility::setEntRef(query, separated_params[1], EntityType::PROCEDURE)));
    }
}