#include "CallsParser.h"

// Parses Calls Relationships and its variants
void CallsParser::parse(Query& query, std::vector<QueryToken> token_chain, QueryToken::QueryTokenType& token_type) {
    // entRef , entRef
    if (token_type != QueryToken::CALLS &&
        token_type != QueryToken::CALLS_T) {
        throw std::invalid_argument("Only Calls and its variants for this method");
    }
    std::vector<std::vector<QueryToken>> separated_params = Utility::splitTokenChain(2, QueryToken::COMMA, token_chain);

    if (!Utility::isEntRef(query, separated_params[0], EntityType::PROCEDURE) || !Utility::isEntRef(query, separated_params[1], EntityType::PROCEDURE)) {
        query.setIsSemanticError("Invalid parameters for Calls");
    }

    if (!Utility::checkIsSemanticError(query)) {
        query.addRelation(RelRef(Utility::queryTokenTypeToRelType(token_type),
            Utility::setEntRef(query, separated_params[0], EntityType::PROCEDURE),
            Utility::setEntRef(query, separated_params[1], EntityType::PROCEDURE)));
    }
}