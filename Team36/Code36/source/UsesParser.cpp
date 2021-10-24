#include "UsesParser.h"

void UsesParser::parse(Query& query, std::vector<QueryToken> token_chain) {
    // stmtRef , entRef
    // entRef, entRef
    std::vector<QueryToken> temp_token_chain_1;
    std::vector<QueryToken> temp_token_chain_2;
    int comma_count = false;
    bool is_USES_S = true;
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
            throw SyntacticErrorException("Invalid parameters for Uses");
        }
    }

    //Validate first param
    if (Utility::isStmtRef(query, temp_token_chain_1)) {
        //is stmRef (WILDCARD incld)
        if (temp_token_chain_1[0].type == QueryToken::WILDCARD) {
            //Throw semantic erros
            throw SemanticErrorException("Invalid parameters for Uses");
        }
    }
    else {
        // Not stmtRef
        if (Utility::isEntRef(query, temp_token_chain_1)) {
            is_USES_S = false;
        }
        else {
            throw SemanticErrorException("Invalid parameters for Uses");
        }

        //check if is PROCEDURE only
        if (!Utility::isCorrectSynEntRef(query, temp_token_chain_1, EntityType::PROCEDURE)) {
            throw SemanticErrorException("Invalid parameters for Modifies");
        }

    }

    //Validate second param
    if (!Utility::isEntRef(query, temp_token_chain_2)) {
        throw SemanticErrorException("Invalid parameters for Uses");
    }
    // check if VARIABLE only
    if (!Utility::isCorrectSynEntRef(query, temp_token_chain_2, EntityType::VARIABLE)) {
        throw SemanticErrorException("Invalid parameters for Modifies");
    }

    if (is_USES_S) {
        QueryToken stmt = temp_token_chain_1[0];

        query.addRelation(RelRef(RelType::USES_S, Utility::setStmtRef(query, stmt),
            Utility::setEntRef(query, temp_token_chain_2, EntityType::VARIABLE)));
    }
    else {
        query.addRelation(RelRef(RelType::USES_P, Utility::setEntRef(query, temp_token_chain_1, EntityType::VARIABLE),
            Utility::setEntRef(query, temp_token_chain_2, EntityType::VARIABLE)));
    }
}
