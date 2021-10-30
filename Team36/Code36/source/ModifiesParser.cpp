#include "ModifiesParser.h"

void ModifiesParser::parse(Query& query, std::vector<QueryToken> token_chain) {
    // stmtRef , entRef
       // entRef, entRef

    std::vector<QueryToken> temp_token_chain_1;
    std::vector<QueryToken> temp_token_chain_2;
    int comma_count = 0;
    bool is_MODIFIES_S = true;
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
            throw SyntacticErrorException("Invalid parameters for Modifies");
        }
    }

    //Validate first param
    if (Utility::isStmtRef(query, temp_token_chain_1)) {
        if (temp_token_chain_1[0].type == QueryToken::WILDCARD) {
            query.setIsSemanticError("Invalid parameters for Modifies");
        }
    }
    else {
        // Not stmtRef
        if (Utility::isEntRef(query, temp_token_chain_1)) {
            is_MODIFIES_S = false;
        }
        else {
            //Not stmtRef or entRef
            query.setIsSemanticError("Invalid parameters for Modifies");
        }

        //check if is PROCEDURE only
        if (!Utility::isCorrectSynEntRef(query, temp_token_chain_1, EntityType::PROCEDURE)) {
            query.setIsSemanticError("Invalid parameters for Modifies");
        }
    }

    //Validate second param
    if (!Utility::isEntRef(query, temp_token_chain_2)) {
        query.setIsSemanticError("Invalid parameters for Modifies");
    }
    // check if VARIABLE only
    if (!Utility::isCorrectSynEntRef(query, temp_token_chain_2, EntityType::VARIABLE)) {
        query.setIsSemanticError("Invalid parameters for Modifies");
    }

    if (is_MODIFIES_S) {
        QueryToken stmt = temp_token_chain_1[0];
        if (!Utility::checkIsSemanticError(query)) {
            query.addRelation(RelRef(RelType::MODIFIES_S, Utility::setStmtRef(query, stmt, EntityType::STMT),
                Utility::setEntRef(query, temp_token_chain_2, EntityType::VARIABLE)));
        }
    }
    else {
        if (!Utility::checkIsSemanticError(query)) {
            query.addRelation(RelRef(RelType::MODIFIES_P, Utility::setEntRef(query, temp_token_chain_1, EntityType::VARIABLE),
                Utility::setEntRef(query, temp_token_chain_2, EntityType::VARIABLE)));
        }
    }
}