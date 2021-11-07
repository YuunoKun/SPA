#include "FollowsParser.h"

// Parses Follows Relationships and its variants
void FollowsParser::parse(Query& query, std::vector<QueryToken> token_chain, QueryToken::QueryTokenType& token_type) {
    // stmtRef , stmtRef

    if (token_type != QueryToken::FOLLOWS &&
        token_type != QueryToken::FOLLOWS_T) {
        throw std::invalid_argument("Only Follows and its variants for this method");
    }

    std::vector<std::vector<QueryToken>> separated_params = Utility::splitTokenChain(2, QueryToken::COMMA, token_chain);

    if (separated_params[0].size() != 1 || separated_params[1].size() != 1) {
        throw SyntacticErrorException("Invalid parameters for Follows");
    }

    if (!Utility::isStmtRef(query, separated_params[0]) || !Utility::isStmtRef(query, separated_params[1])) {
        query.setIsSemanticError("Invalid parameters for Follows");
    }

    QueryToken stmt = separated_params[0][0];

    QueryToken stmt2 = separated_params[1][0];

    if (!Utility::checkIsSemanticError(query)) {
        query.addRelation(RelRef(Utility::queryTokenTypeToRelType(token_type),
            Utility::setStmtRef(query, stmt, EntityType::STMT),
            Utility::setStmtRef(query, stmt2, EntityType::STMT)));
    }
}
