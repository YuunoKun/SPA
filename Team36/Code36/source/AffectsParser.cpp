#include "AffectsParser.h"

void AffectsParser::parse(Query& query, std::vector<QueryToken> token_chain) {
    // stmtRef , stmtRef //assign stmt only

    std::vector<QueryToken> temp_token_chain_1;
    std::vector<QueryToken> temp_token_chain_2;
    int comma_count = false;
    size_t token_chain_size = token_chain.size();
    for (size_t i = 0; i < token_chain_size; i++) {
        if (token_chain[0].type == QueryToken::COMMA) {
            token_chain.erase(token_chain.begin());
            comma_count++;
        }
        else if (comma_count == 0) {
            // 1st param
            temp_token_chain_1.push_back(token_chain[0]);
            token_chain.erase(token_chain.begin());
        }
        else if (comma_count == 1) {
            // 2nd param
            temp_token_chain_2.push_back(token_chain[0]);
            token_chain.erase(token_chain.begin());
        }
        else {
            throw SyntacticErrorException("Invalid parameters for Affects");
        }
    }

    if (temp_token_chain_1.size() != 1 || temp_token_chain_2.size() != 1) {
        throw SyntacticErrorException("Invalid parameters for Affects");
    }

    if (!Utility::isStmtRef(query, temp_token_chain_1, EntityType::ASSIGN) || !Utility::isStmtRef(query, temp_token_chain_2, EntityType::ASSIGN)) {
        query.setIsSemanticError("Invalid parameters for Affects");
    }

    QueryToken stmt = temp_token_chain_1[0];

    QueryToken stmt2 = temp_token_chain_2[0];

    if (!Utility::checkIsSemanticError(query)) {
        query.addRelation(RelRef(RelType::AFFECT,
            Utility::setStmtRef(query, stmt, EntityType::ASSIGN),
            Utility::setStmtRef(query, stmt2, EntityType::ASSIGN)));
    }
}
