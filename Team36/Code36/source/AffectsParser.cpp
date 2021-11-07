#include "AffectsParser.h"

// Parses Affects Relationships and its variants
void AffectsParser::parse(Query& query, std::vector<QueryToken> token_chain, QueryToken::QueryTokenType& token_type) {
    // stmtRef , stmtRef //assign stmt only
    if (token_type != QueryToken::AFFECTS &&
        token_type != QueryToken::AFFECTS_T &&
        token_type != QueryToken::AFFECTS_BIP &&
        token_type != QueryToken::AFFECTS_BIP_T) {
        throw std::invalid_argument("Only Affects and its variants for this method");
    }

    std::vector<std::vector<QueryToken>> separated_params = Utility::splitTokenChain(2, QueryToken::COMMA, token_chain);

    if (separated_params[0].size() != 1 || separated_params[1].size() != 1) {
        throw SyntacticErrorException("Invalid parameters for Affects/Affects*/AffectsBip/AffectsBip*");
    }

    if (!Utility::isStmtRef(query, separated_params[0]) || !Utility::isStmtRef(query, separated_params[1])) {
        query.setIsSemanticError("Invalid parameters for Affects/Affects*/AffectsBip/AffectsBip*");
    }

    QueryToken stmt = separated_params[0][0];

    QueryToken stmt2 = separated_params[1][0];

    if (!Utility::checkIsSemanticError(query)) {
        query.addRelation(RelRef(Utility::queryTokenTypeToRelType(token_type),
            Utility::setStmtRef(query, stmt, EntityType::ASSIGN),
            Utility::setStmtRef(query, stmt2, EntityType::ASSIGN)));
    }
}
