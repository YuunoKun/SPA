#include "NextParser.h"

void NextParser::parse(Query& query, std::vector<QueryToken> token_chain, QueryToken::QueryTokenType& token_type) {
    std::vector<std::vector<QueryToken>> separated_params = Utility::splitTokenChain(2, QueryToken::COMMA, token_chain);

    if (!Utility::isLineRef(query, separated_params[0]) || !Utility::isLineRef(query, separated_params[1])) {
        query.setIsSemanticError("Invalid parameters for Next");
    }

    QueryToken stmt = separated_params[0][0];

    QueryToken stmt2 = separated_params[1][0];

    if (!Utility::checkIsSemanticError(query)) {
        query.addRelation(RelRef(Utility::queryTokenTypeToRelType(token_type),
            Utility::setLineRef(query, stmt),
            Utility::setLineRef(query, stmt2)));
    }

}


