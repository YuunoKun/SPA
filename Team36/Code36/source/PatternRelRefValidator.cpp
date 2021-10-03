#include "PatternRelRefValidator.h"

#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include "Entity.h"
#include "QueryToken.h"
#include "QueryTokenizer.h"
#include "Query.h"
#include "Utility.h"

// isStmtRef and isEntRef token_chain params need to br broken up first

//expect stmtref for first element
bool PatternRelRefValidator::isStmtRef(Query& query, std::vector<QueryToken> token_chain) {
    
    // no args found, throw syntax errors
    if (token_chain.size() == 0) {
        throw std::invalid_argument("Invalid StmtRef arguments");
    }

    // more than 1 args found. possible entref
    if (token_chain.size() > 1) {
        // more than 1 args found. possible entref
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
            throw std::runtime_error("Unknown stmtRef");
        }
    }

    //if got quotation?
    // return false
    //TODO remove this?
  /*  if (token.type == QueryToken::QUOTATION_OPEN) {
        return false;
    }*/


}

//expect entref
bool PatternRelRefValidator::isEntRef(Query& query, std::vector<QueryToken> token_chain) {
    // can just throw error, no need return bool

    // no args found, throw syntax errors
    if (token_chain.size() == 0) {
        throw std::invalid_argument("Invalid EntRef arguments");
    }

    // if only 1 arg found
    if (token_chain.size() == 1) {
        QueryToken token = token_chain[0];

        // remove this?
        if (token.type == QueryToken::CONSTANT) {
            return false;
        }

        if (token.type == QueryToken::WILDCARD) {
            return true;
        }

        // check synonym if is EntRef
        else if (token.type == QueryToken::IDENTIFIER) {
            std::unordered_map<std::string, Entity> ent_chain = query.getEntities();
            if (ent_chain.find(token.token_value) != ent_chain.end()) {
                return ent_chain.at(token.token_value).getType() == EntityType::VARIABLE;
            }
        } else {
            return false;
        }
    } else {
        // TODO do checking for " IDENT " ?
        return true;
    }
}

//expectcomma
bool PatternRelRefValidator::isCommaRef(std::vector<QueryToken> token_chain) {
    if (token_chain.size() == 0) {
        throw std::invalid_argument("Invalid argument, no comma found");
    }
    if (token_chain[0].type != QueryToken::COMMA) {
        throw std::invalid_argument("Invalid argument, expected a comma");
    }
}

PatternRelRefValidator::PatternRelRefValidator() {}

Entity PatternRelRefValidator::setStmtRef(Query& query, QueryToken token) {
  // synonym | ‘_’ | INTEGER

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

  throw std::runtime_error("Unknown stmtRef");
}

Entity PatternRelRefValidator::setEntRef(Query& query,
                                    std::vector<QueryToken> token_chain) {
  // entRef : synonym | ‘_’ | ‘"’ IDENT ‘"’

  if (token_chain.size() == 1) {
    // wild card check
    if (token_chain[0].type == QueryToken::WILDCARD) {
      return Entity(EntityType::WILD);
    }

    // synonym check
    std::unordered_map<std::string, Entity> ent_chain = query.getEntities();
    if (ent_chain.find(token_chain[0].token_value) != ent_chain.end()) {
      return ent_chain.at(token_chain[0].token_value);
    }
  }

  // is " "IDENT" "
  // IDENT : LETTER ( LETTER | DIGIT )*
  if (token_chain.size() == 3) {
      if (token_chain[0].type == QueryToken::QUOTATION_OPEN &&
          token_chain[1].type == QueryToken::IDENTIFIER &&
          token_chain[2].type == QueryToken::QUOTATION_CLOSE) {
          return Entity(EntityType::VARIABLE, token_chain[1].token_value);
      }
  }
  

  throw std::runtime_error("Unknown entRef");
}

// takes in a token_chain with only IDENT* (no QUOTATION_OPEN/CLOSE or WILDCARD)
expr PatternRelRefValidator::setExpr(std::vector<QueryToken> token_chain) {
  // expression-spec : ‘"‘ expr’"’ | ‘_’ ‘"’ expr ‘"’ ‘_’ | ‘_’

  // TODO
  // send to expression parser
  if (token_chain.empty()) {
    return "";
  } else if (token_chain.size() == 1 &&
             token_chain[0].type == QueryToken::IDENTIFIER) {
    return token_chain[0].token_value;
  } else if (token_chain.size() == 1 &&
      token_chain[0].type == QueryToken::CONSTANT) {
      return token_chain[0].token_value;
  }

  throw std::runtime_error("Unknown expr");
}

void PatternRelRefValidator::parseParameterSuchThat(
    Query& query, QueryToken::QueryTokenType token_type,
    std::vector<QueryToken> token_chain) {
    switch (token_type) {
    //TODO
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
            //is stmRef (WILDCARD incld)
            if (temp_token_chain_1[0].type == QueryToken::WILDCARD) {
                //Throw semantic erros
                throw std::invalid_argument("Invalid parameters for Modifies");
            }
        } else {
            // Not stmtRef
            if (isEntRef(query, temp_token_chain_1)) {
                is_MODIFIES_S = false;
            } else {
                throw std::invalid_argument("Invalid parameters for Modifies");
            }
        }

        //Validate second param
        if (!isEntRef(query, temp_token_chain_2)) {
            throw std::invalid_argument("Invalid parameters for Modifies");
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
    //TODO
    case QueryToken::USES_S: {
        // stmtRef , entRef

        if (!isStmtRef(query, token_chain)) {
            throw std::invalid_argument("Invalid parameters for Uses");
        }
        QueryToken stmt = token_chain[0];
        if (stmt.type == QueryToken::WILDCARD) {
            throw std::invalid_argument("Invalid parameters for Uses");
        }
        token_chain.erase(token_chain.begin(), token_chain.begin() + 1);

        isCommaRef(token_chain);
        token_chain.erase(token_chain.begin(), token_chain.begin() + 1);

        if (!isEntRef(query, token_chain)) {
            throw std::invalid_argument("Invalid parameters for Uses");
        }
        query.addRelation(RelRef(RelType::USES_S, setStmtRef(query, stmt),
            setEntRef(query, token_chain)));

        break;
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
            throw std::invalid_argument("Invalid parameters for Parent");
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
            throw std::invalid_argument("Invalid parameters for Parent*");
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
            throw std::invalid_argument("Invalid parameters for Follows");
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
            throw std::invalid_argument("Invalid parameters for Follows*");
        }

        QueryToken stmt = temp_token_chain_1[0];

        QueryToken stmt2 = temp_token_chain_2[0];

        query.addRelation(RelRef(RelType::FOLLOWS_T,
            setStmtRef(query, stmt),
            setStmtRef(query, stmt2)));
        break;
    }
    //TODO
    case QueryToken::CALLS: {
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
    case QueryToken::CALLS_T: {
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

void PatternRelRefValidator::parseParameterPattern(
    Query& query, Entity& synonym_ent, std::vector<QueryToken> token_chain) {
    bool wild = false;
    switch (synonym_ent.getType()) {
    case ASSIGN: {
        // find comma
        // every QueryToken before comma token is sent to temp_token_chain
        // Expect entRef token_chain
        if (token_chain.size() == 0) {
            throw std::invalid_argument("Invalid argument, no entRef found");
        }
        std::vector<QueryToken> temp_token_chain;
        for (int i = 0; i < token_chain.size(); i++) {
            if (token_chain[0].type != QueryToken::COMMA) {
                temp_token_chain.push_back(token_chain[0]);
                token_chain.erase(token_chain.begin());
            }
            else {
                break;
            }
        }
        if (temp_token_chain.size() == 0) {
            throw std::invalid_argument("Invalid argument, no entRef found");
        }
        if (token_chain.size() == 0) {
            throw std::invalid_argument("Invalid argument, no comma found");
        }

        // check if the curr token_chain starts with comma
        if (token_chain[0].type == QueryToken::COMMA) {
            token_chain.erase(token_chain.begin());
        }
        else {
            // if no comma found means invalid parameters
            throw std::runtime_error("Unexpected parameters for Pattern");
        }
        if (token_chain.size() == 0) {
            throw std::invalid_argument("Invalid argument, no expr found");
        }
        // check second parameter if is WILDCARD 
        // TODO check after u erase.
        if (token_chain[0].type == QueryToken::WILDCARD) {
            wild = true;
            // if first element is WILDCARD token, last element must be WILDCARD for
            // last 2 cases below expression-spec : ‘"‘ expr’"’ | ‘_’ ‘"’ expr ‘"’
            // ‘_’ | ‘_’
            if (token_chain[token_chain.size() - 1].type != QueryToken::WILDCARD) {
                throw std::runtime_error("Unexpected parameters for Pattern");
            }
            // remove first WILDCARD token
            token_chain.erase(token_chain.begin());
        }

        std::vector<QueryToken> temp_token_chain_2;

        if (token_chain.size() != 0) {
            //one or more token
            if (token_chain[0].type == QueryToken::QUOTATION_OPEN) {
                token_chain.erase(token_chain.begin());


                // run thru all until quotation_close
                for (int i = 0; i < token_chain.size(); i++) {
                    if (token_chain[i].type != QueryToken::QUOTATION_CLOSE) {
                        temp_token_chain_2.push_back(token_chain[i]);
                    }
                    else {
                        token_chain.erase(token_chain.begin(), token_chain.begin() + i + 1);
                        // if is wild need remove the last wildcard
                        if (wild && token_chain[0].type == QueryToken::WILDCARD) {
                            token_chain.erase(token_chain.begin());
                        }

                        // go out of loop once hit quotation close
                        break;
                    }
                }
            }

            //} else if (wild && token_chain[0].type == QueryToken::WILDCARD) {
            //  // 2 WILDCARDS in a row
            //  throw std::runtime_error("Unexpected parameters for Pattern");
            //}

            // if still got things means wrong params
            if (token_chain.size() != 0) {

                throw std::runtime_error("Unexpected parameters for Pattern");
            }

        }

        if (temp_token_chain.size() == 0) {
            throw std::invalid_argument("Invalid argument, no entRef found");
        }

        query.addPattern(Pattern(synonym_ent, setEntRef(query, temp_token_chain),
            setExpr(temp_token_chain_2), wild));
        }

    }

}