#include "AffectsTParser.h"

void AffectsTParser::parse(Query& query, std::vector<QueryToken> token_chain) {
    // stmtRef , stmtRef //assign stmt only
    std::vector<std::vector<QueryToken>> separated_params = Utility::splitTokenChain(2, QueryToken::COMMA, token_chain);

    if (separated_params[0].size() != 1 || separated_params[1].size() != 1) {
        throw SyntacticErrorException("Invalid parameters for Affects*");
    }

    if (!Utility::isStmtRef(query, separated_params[0], EntityType::ASSIGN) || !Utility::isStmtRef(query, separated_params[1], EntityType::ASSIGN)) {
        query.setIsSemanticError("Invalid parameters for Affects*");
    }

    QueryToken stmt = separated_params[0][0];

    QueryToken stmt2 = separated_params[1][0];

    if (!Utility::checkIsSemanticError(query)) {
        query.addRelation(RelRef(RelType::AFFECT_T,
            Utility::setStmtRef(query, stmt, EntityType::ASSIGN),
            Utility::setStmtRef(query, stmt2, EntityType::ASSIGN)));
    }
}