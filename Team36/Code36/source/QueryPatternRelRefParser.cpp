#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include "QueryPatternRelRefParser.h"

QueryPatternRelRefParser::QueryPatternRelRefParser() {}

bool QueryPatternRelRefParser::isStmtRef(Query& query, std::vector<QueryToken> token_chain) {
    
    // no args found, throw syntax errors
    if (token_chain.size() == 0) {
        throw SyntacticErrorException("Invalid stmtRef arguments");
    }

    // more than 1 args found. possible entref
    if (token_chain.size() > 1) {
        return false;
    }

    QueryToken token = token_chain[0];
    
    //if integer, return true
    if (token.type == QueryToken::CONSTANT) {
        return true;
    }

	if (token.type == QueryToken::WILDCARD) {
		return true;
	}

    // check synonym if is STMT
    if (token.type == QueryToken::IDENTIFIER) {
        std::unordered_map<std::string, Entity> ent_chain = query.getEntities();
        if (ent_chain.find(token.token_value) != ent_chain.end()) {
            return ent_chain.at(token.token_value).getType() == EntityType::STMT || 
                ent_chain.at(token.token_value).getType() == EntityType::READ ||
                ent_chain.at(token.token_value).getType() == EntityType::PRINT ||
                ent_chain.at(token.token_value).getType() == EntityType::CALL ||
                ent_chain.at(token.token_value).getType() == EntityType::WHILE || 
                ent_chain.at(token.token_value).getType() == EntityType::IF || 
                ent_chain.at(token.token_value).getType() == EntityType::ASSIGN;
        } else {
            // Undeclared Syn, Cannot find Entity in Query
            throw SyntacticErrorException("Undeclared stmtRef");
        }
    }

    // false will either be semantic errors OR will go to entRef(for Modifies and Uses)
    return false;
}

bool QueryPatternRelRefParser::isEntRef(Query& query, std::vector<QueryToken> token_chain) {
    // entRef : synonym | �_� | �"� IDENT �"�

    if (token_chain.size() == 0) {
        throw SyntacticErrorException("Invalid entRef arguments");
    } else if (token_chain.size() == 1) {
        QueryToken token = token_chain[0];

        if (token.type == QueryToken::WILDCARD) {
            return true;
        } else if (token.type == QueryToken::IDENTIFIER) {
            // check synonym if is EntRef
            std::unordered_map<std::string, Entity> ent_chain = query.getEntities();

            if (ent_chain.find(token.token_value) != ent_chain.end()) {
                return ent_chain.at(token.token_value).getType() == EntityType::VARIABLE ||
                    ent_chain.at(token.token_value).getType() == EntityType::PROCEDURE;
            } else {
                // Undeclared Syn, Cannot find Entity in Query
                throw SyntacticErrorException("Undeclared entRef");
            }
        } else {
            return false;
        }

    } else if (token_chain.size() == 3) {
        //checking for " IDENT " 
        if (token_chain[0].type == QueryToken::QUOTATION_OPEN &&
            token_chain[1].type == QueryToken::IDENTIFIER &&
            token_chain[2].type == QueryToken::QUOTATION_CLOSE) {
            return true;
        } else {
            throw SyntacticErrorException("Invalid EntRef arguments");
        }
    } else {
        throw SyntacticErrorException("Invalid EntRef arguments");
    }
}

bool QueryPatternRelRefParser::isExpr(std::vector<QueryToken> token_chain) {
    
    size_t token_chain_size = token_chain.size();

    if (token_chain_size == 0) {
        throw SyntacticErrorException("Invalid entRef arguments");

    } else if (token_chain_size == 1) {

        return token_chain[0].type == QueryToken::WILDCARD;

    } else if (token_chain_size != 2 && 
        token_chain[0].type == QueryToken::QUOTATION_OPEN &&
            token_chain[token_chain_size - 1].type == QueryToken::QUOTATION_CLOSE) {
            
        return true;

    } else if (token_chain_size != 4 && 
        token_chain[0].type == QueryToken::WILDCARD &&
        token_chain[1].type == QueryToken::QUOTATION_OPEN &&
        token_chain[token_chain_size - 2].type == QueryToken::QUOTATION_CLOSE &&
        token_chain[token_chain_size - 1].type == QueryToken::WILDCARD) {

        return true;

    } else {
        return false;
    }
}

bool QueryPatternRelRefParser::isCorrectSynEntRef(Query& query, std::vector<QueryToken> token_chain, 
    EntityType expected_ent_type) {
    
    std::unordered_map<std::string, Entity> ent_chain = query.getEntities();

    if (token_chain.size() == 1 && token_chain[0].type == QueryToken::IDENTIFIER) {

        QueryToken token = token_chain[0];
        if (ent_chain.find(token.token_value) != ent_chain.end()) {
            return ent_chain.at(token.token_value).getType() == expected_ent_type;
        }

    } else {
        return true;
    }
}

bool QueryPatternRelRefParser::isCommaRef(std::vector<QueryToken> token_chain) {
    if (token_chain.size() == 0) {
        throw std::invalid_argument("Invalid argument, no comma found");
    }
    if (token_chain[0].type != QueryToken::COMMA) {
        throw std::invalid_argument("Invalid argument, expected a comma");
    }
}

Entity QueryPatternRelRefParser::setStmtRef(Query& query, QueryToken token) {
  // synonym | �_� | INTEGER

  // wild card check
  if (token.type == QueryToken::WILDCARD) {
    return Entity(EntityType::WILD);
  }

  // is INTEGER, constant type
  if (token.type == QueryToken::CONSTANT) {
    return Entity(EntityType::CONSTANT, token.token_value);
  }

  // synonym check
  std::unordered_map<std::string, Entity> ent_chain = query.getEntities();
  if (ent_chain.find(token.token_value) != ent_chain.end()) {
    return ent_chain.at(token.token_value);
  }

  throw SemanticErrorException("Unknown stmtRef");
}

Entity QueryPatternRelRefParser::setEntRef(Query& query,
                                    std::vector<QueryToken> token_chain) {
  // entRef : synonym | �_� | �"� IDENT �"�

  if (token_chain.size() == 1) {
    // wild card check
    if (token_chain[0].type == QueryToken::WILDCARD) {
      return Entity(EntityType::WILD);
    }

    // synonym check
    // Can remove?
    std::unordered_map<std::string, Entity> ent_chain = query.getEntities();
    if (ent_chain.find(token_chain[0].token_value) != ent_chain.end()) {
      return ent_chain.at(token_chain[0].token_value);
    }
  }

  // is " "IDENT" "
  // IDENT : LETTER ( LETTER | DIGIT )*
  if (token_chain.size() == 3) {
      return Entity(EntityType::VARIABLE, token_chain[1].token_value);
  }
  

  throw SemanticErrorException("Unknown entRef");
}

Entity QueryPatternRelRefParser::setCallEntRef(Query& query,
    std::vector<QueryToken> token_chain) {
    // entRef : synonym | �_� | �"� IDENT �"�

    if (token_chain.size() == 1) {
        // wild card check
        if (token_chain[0].type == QueryToken::WILDCARD) {
            return Entity(EntityType::WILD);
        }

        // synonym check
        // Can remove?
        std::unordered_map<std::string, Entity> ent_chain = query.getEntities();
        if (ent_chain.find(token_chain[0].token_value) != ent_chain.end()) {
            return ent_chain.at(token_chain[0].token_value);
        }
    }

    // is " "IDENT" "
    // IDENT : LETTER ( LETTER | DIGIT )*
    if (token_chain.size() == 3) {
        return Entity(EntityType::PROCEDURE, token_chain[1].token_value);
    }


    throw SemanticErrorException("Unknown entRef");
}

std::string QueryPatternRelRefParser::setExpr(std::vector<QueryToken> token_chain) {
  // expression-spec : �"� expr�"� | �_� �"� expr �"� �_� | �_�
    std::string result = "";
    if (token_chain.size() == 1 && token_chain[0].type == QueryToken::WILDCARD) {
        return result;
    } else {
        for (size_t i = 0; i < token_chain.size(); i++) {
            if (token_chain[i].type != QueryToken::WILDCARD &&
                token_chain[i].type != QueryToken::QUOTATION_OPEN &&
                token_chain[i].type != QueryToken::QUOTATION_CLOSE) {
                result += token_chain[i].token_value;
            }
        }
        return result;
    }
    
}

void QueryPatternRelRefParser::parseParameterSuchThat(
    Query& query, QueryToken::QueryTokenType token_type,
    std::vector<QueryToken> token_chain) {
    switch (token_type) {
    case QueryToken::MODIFIES_S: {
        // stmtRef , entRef
        // entRef, entRef

        std::vector<QueryToken> temp_token_chain_1;
        std::vector<QueryToken> temp_token_chain_2;
        bool comma_found = false;
        bool is_MODIFIES_S = true;
        size_t token_chain_size = token_chain.size();
        for (size_t i = 0; i < token_chain_size; i++) {
            if (token_chain[0].type == QueryToken::COMMA) {
                token_chain.erase(token_chain.begin());
                comma_found = true;
            }
            else if (!comma_found) {
                // 1st param
                temp_token_chain_1.push_back(token_chain[0]);
                token_chain.erase(token_chain.begin());
            }
            else {
                // 2nd param
                temp_token_chain_2.push_back(token_chain[0]);
                token_chain.erase(token_chain.begin());
            }
        }
        
        //Validate first param
        if (isStmtRef(query, temp_token_chain_1)) {
            if (temp_token_chain_1[0].type == QueryToken::WILDCARD) {
                throw SemanticErrorException("Invalid parameters for Modifies");
            }
        } else {
            // Not stmtRef
            if (isEntRef(query, temp_token_chain_1)) {
                is_MODIFIES_S = false;
            } else {
                //Not stmtRef or entRef
                throw SemanticErrorException("Invalid parameters for Modifies");
            }

            //check if is PROCEDURE only
            if (!isCorrectSynEntRef(query, temp_token_chain_1, EntityType::PROCEDURE)) {
                throw SemanticErrorException("Invalid parameters for Modifies");
            }
        }

        //Validate second param
        if (!isEntRef(query, temp_token_chain_2)) {
            throw SemanticErrorException("Invalid parameters for Modifies");
        }
        // check if VARIABLE only
        if (!isCorrectSynEntRef(query, temp_token_chain_2, EntityType::VARIABLE)) {
            throw SemanticErrorException("Invalid parameters for Modifies");
        }

        if (is_MODIFIES_S) {
            QueryToken stmt = temp_token_chain_1[0];
           
            query.addRelation(RelRef(RelType::MODIFIES_S, setStmtRef(query, stmt),
                setEntRef(query, temp_token_chain_2)));

            break;
        } else {
            query.addRelation(RelRef(RelType::MODIFIES_P, setEntRef(query, temp_token_chain_1),
                setEntRef(query, temp_token_chain_2)));
            break;
        }
    }
    
    case QueryToken::USES_S: {
        // stmtRef , entRef
        // entRef, entRef
        std::vector<QueryToken> temp_token_chain_1;
        std::vector<QueryToken> temp_token_chain_2;
        bool comma_found = false;
        bool is_USES_S = true;
        size_t token_chain_size = token_chain.size();
        for (size_t i = 0; i < token_chain_size; i++) {
            if (token_chain[0].type == QueryToken::COMMA) {
                token_chain.erase(token_chain.begin());
                comma_found = true;
            }
            else if (!comma_found) {
                // 1st param
                temp_token_chain_1.push_back(token_chain[0]);
                token_chain.erase(token_chain.begin());
            }
            else {
                // 2nd param
                temp_token_chain_2.push_back(token_chain[0]);
                token_chain.erase(token_chain.begin());
            }
        }

        //Validate first param
        if (isStmtRef(query, temp_token_chain_1)) {
            //is stmRef (WILDCARD incld)
            if (temp_token_chain_1[0].type == QueryToken::WILDCARD) {
                //Throw semantic erros
                throw SemanticErrorException("Invalid parameters for Uses");
            }
        }
        else {
            // Not stmtRef
            if (isEntRef(query, temp_token_chain_1)) {
                is_USES_S = false;
            }
            else {
                throw SemanticErrorException("Invalid parameters for Uses");
            }

            //check if is PROCEDURE only
            if (!isCorrectSynEntRef(query, temp_token_chain_1, EntityType::PROCEDURE)) {
                throw SemanticErrorException("Invalid parameters for Modifies");
            }

        }

        //Validate second param
        if (!isEntRef(query, temp_token_chain_2)) {
            throw SemanticErrorException("Invalid parameters for Uses");
        }
        // check if VARIABLE only
        if (!isCorrectSynEntRef(query, temp_token_chain_2, EntityType::VARIABLE)) {
            throw SemanticErrorException("Invalid parameters for Modifies");
        }

        if (is_USES_S) {
            QueryToken stmt = temp_token_chain_1[0];

            query.addRelation(RelRef(RelType::USES_S, setStmtRef(query, stmt),
                setEntRef(query, temp_token_chain_2)));

            break;
        }
        else {
            query.addRelation(RelRef(RelType::USES_P, setEntRef(query, temp_token_chain_1),
                setEntRef(query, temp_token_chain_2)));
            break;
        }
    }

    case QueryToken::PARENT: {
        // stmtRef , stmtRef

        std::vector<QueryToken> temp_token_chain_1;
        std::vector<QueryToken> temp_token_chain_2;
        bool comma_found = false;
        size_t token_chain_size = token_chain.size();
        for (size_t i = 0; i < token_chain_size; i++) {
            if (token_chain[0].type == QueryToken::COMMA) {
                token_chain.erase(token_chain.begin());
                comma_found = true;
            }
            else if (!comma_found) {
                // 1st param
                temp_token_chain_1.push_back(token_chain[0]);
                token_chain.erase(token_chain.begin());
            }
            else {
                // 2nd param
                temp_token_chain_2.push_back(token_chain[0]);
                token_chain.erase(token_chain.begin());
            }
        }

        if (!isStmtRef(query, temp_token_chain_1) || !isStmtRef(query, temp_token_chain_2)) {
            throw SemanticErrorException("Invalid parameters for Parent");
        }

        QueryToken stmt = temp_token_chain_1[0];

        QueryToken stmt2 = temp_token_chain_2[0];

        query.addRelation(RelRef(RelType::PARENT,
            setStmtRef(query, stmt),
            setStmtRef(query, stmt2)));
        break;
    }
      
    case QueryToken::PARENT_T: {
        // stmtRef , stmtRef

        std::vector<QueryToken> temp_token_chain_1;
        std::vector<QueryToken> temp_token_chain_2;
        bool comma_found = false;
        size_t token_chain_size = token_chain.size();
        for (size_t i = 0; i < token_chain_size; i++) {
            if (token_chain[0].type == QueryToken::COMMA) {
                token_chain.erase(token_chain.begin());
                comma_found = true;
            }
            else if (!comma_found) {
                // 1st param
                temp_token_chain_1.push_back(token_chain[0]);
                token_chain.erase(token_chain.begin());
            }
            else {
                // 2nd param
                temp_token_chain_2.push_back(token_chain[0]);
                token_chain.erase(token_chain.begin());
            }
        }

        if (!isStmtRef(query, temp_token_chain_1) || !isStmtRef(query, temp_token_chain_2)) {
            throw SemanticErrorException("Invalid parameters for Parent*");
        }

        QueryToken stmt = temp_token_chain_1[0];

        QueryToken stmt2 = temp_token_chain_2[0];

        query.addRelation(RelRef(RelType::PARENT_T,
            setStmtRef(query, stmt),
            setStmtRef(query, stmt2)));
        break;
    }
    
    case QueryToken::FOLLOWS: {
        // stmtRef , stmtRef

        std::vector<QueryToken> temp_token_chain_1;
        std::vector<QueryToken> temp_token_chain_2;
        bool comma_found = false;
        size_t token_chain_size = token_chain.size();
        for (size_t i = 0; i < token_chain_size; i++) {
            if (token_chain[0].type == QueryToken::COMMA) {
                token_chain.erase(token_chain.begin());
                comma_found = true;
            }
            else if (!comma_found) {
                // 1st param
                temp_token_chain_1.push_back(token_chain[0]);
                token_chain.erase(token_chain.begin());
            }
            else {
                // 2nd param
                temp_token_chain_2.push_back(token_chain[0]);
                token_chain.erase(token_chain.begin());
            }
        }

        if (!isStmtRef(query, temp_token_chain_1) || !isStmtRef(query, temp_token_chain_2)) {
            throw SemanticErrorException("Invalid parameters for Follows");
        }

        QueryToken stmt = temp_token_chain_1[0];

        QueryToken stmt2 = temp_token_chain_2[0];

        query.addRelation(RelRef(RelType::FOLLOWS,
            setStmtRef(query, stmt),
            setStmtRef(query, stmt2)));
        break;
    }
    
    case QueryToken::FOLLOWS_T: {
        // stmtRef , stmtRef

        std::vector<QueryToken> temp_token_chain_1;
        std::vector<QueryToken> temp_token_chain_2;
        bool comma_found = false;
        size_t token_chain_size = token_chain.size();
        for (size_t i = 0; i < token_chain_size; i++) {
            if (token_chain[0].type == QueryToken::COMMA) {
                token_chain.erase(token_chain.begin());
                comma_found = true;
            }
            else if (!comma_found) {
                // 1st param
                temp_token_chain_1.push_back(token_chain[0]);
                token_chain.erase(token_chain.begin());
            }
            else {
                // 2nd param
                temp_token_chain_2.push_back(token_chain[0]);
                token_chain.erase(token_chain.begin());
            }
        }

        if (!isStmtRef(query, temp_token_chain_1) || !isStmtRef(query, temp_token_chain_2)) {
            throw SemanticErrorException("Invalid parameters for Follows*");
        }

        QueryToken stmt = temp_token_chain_1[0];

        QueryToken stmt2 = temp_token_chain_2[0];

        query.addRelation(RelRef(RelType::FOLLOWS_T,
            setStmtRef(query, stmt),
            setStmtRef(query, stmt2)));
        break;
    }
   
    case QueryToken::CALLS: {
        // entRef , entRef

        std::vector<QueryToken> temp_token_chain_1;
        std::vector<QueryToken> temp_token_chain_2;
        bool comma_found = false;
        size_t token_chain_size = token_chain.size();
        for (size_t i = 0; i < token_chain_size; i++) {
            if (token_chain[0].type == QueryToken::COMMA) {
                token_chain.erase(token_chain.begin());
                comma_found = true;
            }
            else if (!comma_found) {
                // 1st param
                temp_token_chain_1.push_back(token_chain[0]);
                token_chain.erase(token_chain.begin());
            }
            else {
                // 2nd param
                temp_token_chain_2.push_back(token_chain[0]);
                token_chain.erase(token_chain.begin());
            }
        }

        if (!isEntRef(query, temp_token_chain_1) || !isEntRef(query, temp_token_chain_2)) {
            throw SemanticErrorException("Invalid parameters for Calls");
        }
        // Check if both args are PROCEDURE
        if (!isCorrectSynEntRef(query, temp_token_chain_1, EntityType::PROCEDURE) || 
            !isCorrectSynEntRef(query, temp_token_chain_2, EntityType::PROCEDURE)) {
            throw SemanticErrorException("Invalid parameters for Calls");
        }

        query.addRelation(RelRef(RelType::CALLS,
            setCallEntRef(query, temp_token_chain_1),
            setCallEntRef(query, temp_token_chain_2)));
        break;
    }
    
    case QueryToken::CALLS_T: {
        // entRef , entRef

        std::vector<QueryToken> temp_token_chain_1;
        std::vector<QueryToken> temp_token_chain_2;
        bool comma_found = false;
        size_t token_chain_size = token_chain.size();
        for (size_t i = 0; i < token_chain_size; i++) {
            if (token_chain[0].type == QueryToken::COMMA) {
                token_chain.erase(token_chain.begin());
                comma_found = true;
            }
            else if (!comma_found) {
                // 1st param
                temp_token_chain_1.push_back(token_chain[0]);
                token_chain.erase(token_chain.begin());
            }
            else {
                // 2nd param
                temp_token_chain_2.push_back(token_chain[0]);
                token_chain.erase(token_chain.begin());
            }
        }

        if (!isEntRef(query, temp_token_chain_1) || !isEntRef(query, temp_token_chain_2)) {
            throw SemanticErrorException("Invalid parameters for Calls*");
        }
        // Check if both args are PROCEDUREs
        if (!isCorrectSynEntRef(query, temp_token_chain_1, EntityType::PROCEDURE) ||
            !isCorrectSynEntRef(query, temp_token_chain_2, EntityType::PROCEDURE)) {
            throw SemanticErrorException("Invalid parameters for Calls*");
        }

        query.addRelation(RelRef(RelType::CALLS_T,
            setCallEntRef(query, temp_token_chain_1),
            setCallEntRef(query, temp_token_chain_2)));
        break;
    }

    case QueryToken::AFFECTS: {
        // stmtRef , stmtRef
        if (!isStmtRef(query, token_chain)) {
            throw std::invalid_argument("Invalid parameters for Affects");
        }
        QueryToken stmt = token_chain[0];
        token_chain.erase(token_chain.begin(), token_chain.begin() + 1);

        isCommaRef(token_chain);
        token_chain.erase(token_chain.begin(), token_chain.begin() + 1);

        if (!isStmtRef(query, token_chain)) {
            throw std::invalid_argument("Invalid parameters for Affects");
        }
        QueryToken stmt2 = token_chain[0];
        token_chain.erase(token_chain.begin(), token_chain.begin() + 1);
        if (token_chain.size() != 0) {
            throw std::invalid_argument("Unexpected parameters for Affects");
        }
        //TODO 
        query.addRelation(RelRef(RelType::FOLLOWS,
            setStmtRef(query, stmt),
            setStmtRef(query, stmt2)));
        break;
    }

    case QueryToken::AFFECTS_T: {
        // stmtRef , stmtRef
        if (!isStmtRef(query, token_chain)) {
            throw std::invalid_argument("Invalid parameters for Affects*");
        }
        QueryToken stmt = token_chain[0];
        token_chain.erase(token_chain.begin(), token_chain.begin() + 1);

        isCommaRef(token_chain);
        token_chain.erase(token_chain.begin(), token_chain.begin() + 1);

        if (!isStmtRef(query, token_chain)) {
            throw std::invalid_argument("Invalid parameters for Affects*");
        }
        QueryToken stmt2 = token_chain[0];
        token_chain.erase(token_chain.begin(), token_chain.begin() + 1);
        if (token_chain.size() != 0) {
            throw std::invalid_argument("Unexpected parameters for Affects*");
        }
        //TODO
        query.addRelation(RelRef(RelType::FOLLOWS,
            setStmtRef(query, stmt),
            setStmtRef(query, stmt2)));
        break;
    }
    //TODO
    case QueryToken::NEXT: {
        // stmtRef , stmtRef
        if (!isStmtRef(query, token_chain)) {
            throw std::invalid_argument("Invalid parameters for Follows");
        }
        QueryToken stmt = token_chain[0];
        token_chain.erase(token_chain.begin(), token_chain.begin() + 1);

        isCommaRef(token_chain);
        token_chain.erase(token_chain.begin(), token_chain.begin() + 1);

        if (!isStmtRef(query, token_chain)) {
            throw std::invalid_argument("Invalid parameters for Follows");
        }
        QueryToken stmt2 = token_chain[0];
        token_chain.erase(token_chain.begin(), token_chain.begin() + 1);
        if (token_chain.size() != 0) {
            throw std::invalid_argument("Unexpected parameters for Follows");
        }
        query.addRelation(RelRef(RelType::FOLLOWS,
            setStmtRef(query, stmt),
            setStmtRef(query, stmt2)));
        break;
    }
    //TODO
    case QueryToken::NEXT_T: {
        // stmtRef , stmtRef
        if (!isStmtRef(query, token_chain)) {
            throw std::invalid_argument("Invalid parameters for Follows");
        }
        QueryToken stmt = token_chain[0];
        token_chain.erase(token_chain.begin(), token_chain.begin() + 1);

        isCommaRef(token_chain);
        token_chain.erase(token_chain.begin(), token_chain.begin() + 1);

        if (!isStmtRef(query, token_chain)) {
            throw std::invalid_argument("Invalid parameters for Follows");
        }
        QueryToken stmt2 = token_chain[0];
        token_chain.erase(token_chain.begin(), token_chain.begin() + 1);
        if (token_chain.size() != 0) {
            throw std::invalid_argument("Unexpected parameters for Follows");
        }
        query.addRelation(RelRef(RelType::FOLLOWS,
            setStmtRef(query, stmt),
            setStmtRef(query, stmt2)));
        break;
    }

    default:
        throw std::runtime_error("Unknown RelRef query token type : \'" + token_type +
                            '\'');
    }
}

void QueryPatternRelRefParser::parseParameterPattern(
    Query& query, Entity& synonym_ent, std::vector<QueryToken> token_chain) {
    bool wild = false;
    switch (synonym_ent.getType()) {
    case ASSIGN: {
        AssignPatternParser parser;
        parser.parseParameterAssign(query, synonym_ent, token_chain);
        }
    }

}