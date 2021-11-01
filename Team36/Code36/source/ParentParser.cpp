#include "ParentParser.h"

void ParentParser::parse(Query& query, std::vector<QueryToken> token_chain) {
    // stmtRef , stmtRef
    std::vector<std::vector<QueryToken>> separated_params = Utility::splitTokenChain(2, QueryToken::COMMA, token_chain);

    if (separated_params[0].size() != 1 || separated_params[1].size() != 1) {
        throw SyntacticErrorException("Invalid parameters for Parent");
    }

    if (!Utility::isStmtRef(query, separated_params[0]) || !Utility::isStmtRef(query, separated_params[1])) {
        query.setIsSemanticError("Invalid parameters for Parent");
    }

    QueryToken stmt = separated_params[0][0];

    QueryToken stmt2 = separated_params[1][0];

    if (!Utility::checkIsSemanticError(query)) {
        query.addRelation(RelRef(RelType::PARENT,
            Utility::setStmtRef(query, stmt, EntityType::STMT),
            Utility::setStmtRef(query, stmt2, EntityType::STMT)));
    }
}