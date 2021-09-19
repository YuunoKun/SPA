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
    // can just throw error, no need return bool
    if (token_chain.size() ==0) {
        throw std::invalid_argument("Invalid StmtRef arguments");
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
                ent_chain.at(token.token_value).getType() == EntityType::WHILE || 
                ent_chain.at(token.token_value).getType() == EntityType::IF || 
                ent_chain.at(token.token_value).getType() == EntityType::ASSIGN;
        }
    }

    //if got quotation?
    // return false
    if (token.type == QueryToken::QUOTATION_OPEN) {
        return false;
    }


}

//expect entref
bool PatternRelRefValidator::isEntRef(Query& query, std::vector<QueryToken> token_chain) {
    // can just throw error, no need return bool

    if (token_chain.size() == 0) {
        throw std::invalid_argument("Invalid EntRef arguments");
    }

    if (token_chain.size() == 1) {
        QueryToken token = token_chain[0];

        if (token.type == QueryToken::CONSTANT) {
            return false;
        }

        if (token.type == QueryToken::WILDCARD) {
            return true;
        }

        // check synonym if is EntRef
        if (token.type == QueryToken::IDENTIFIER) {
            std::unordered_map<std::string, Entity> ent_chain = query.getEntities();
            if (ent_chain.find(token.token_value) != ent_chain.end()) {
                return ent_chain.at(token.token_value).getType() == EntityType::VARIABLE;
            }
        }
    }

    return true;


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
  case QueryToken::MODIFIES_S: {
      // stmtRef , entRef
      
      if (!isStmtRef(query, token_chain)) {
          throw std::invalid_argument("Invalid parameters for Modifies");
      }
      QueryToken stmt = token_chain[0];
      if (stmt.type == QueryToken::WILDCARD) {
          throw std::invalid_argument("Invalid parameters for Modifies");
      }
      token_chain.erase(token_chain.begin(), token_chain.begin() + 1);
      
      isCommaRef(token_chain);
      token_chain.erase(token_chain.begin(), token_chain.begin() + 1);

      
      if (!isEntRef(query, token_chain)) {
          throw std::invalid_argument("Invalid parameters for Modifies");
      }
      query.addRelation(RelRef(RelType::MODIFIES_S, setStmtRef(query, stmt),
          setEntRef(query, token_chain)));

      break;
    }
      
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
        if (!isStmtRef(query, token_chain)) {
            throw std::invalid_argument("Invalid parameters for Parent");
        }

        QueryToken stmt = token_chain[0];
        token_chain.erase(token_chain.begin(), token_chain.begin() + 1);

        isCommaRef(token_chain);
        token_chain.erase(token_chain.begin(), token_chain.begin() + 1);

        if (!isStmtRef(query, token_chain)) {
            throw std::invalid_argument("Invalid parameters for Parent");
        }
        QueryToken stmt2 = token_chain[0];
        token_chain.erase(token_chain.begin(), token_chain.begin() + 1);
        if (token_chain.size() != 0) {
            throw std::invalid_argument("Unexpected parameters for Parent");
        }
        query.addRelation(RelRef(RelType::PARENT,
            setStmtRef(query, stmt),
            setStmtRef(query, stmt2)));
        break;
    }
      
    case QueryToken::PARENT_T: {
        // stmtRef , stmtRef
        if (!isStmtRef(query, token_chain)) {
            throw std::invalid_argument("Invalid parameters for Parent*");
        }        
        QueryToken stmt = token_chain[0];
        token_chain.erase(token_chain.begin(), token_chain.begin() + 1);

        isCommaRef(token_chain);
        token_chain.erase(token_chain.begin(), token_chain.begin() + 1);

        if (!isStmtRef(query, token_chain)) {
            throw std::invalid_argument("Invalid parameters for Parent*");
        }
        QueryToken stmt2 = token_chain[0];
        token_chain.erase(token_chain.begin(), token_chain.begin() + 1);
        if (token_chain.size() != 0) {
            throw std::invalid_argument("Unexpected parameters for Parent*");
        }
        query.addRelation(RelRef(RelType::PARENT_T,
            setStmtRef(query, stmt),
            setStmtRef(query, stmt2)));
        break;
    }
    case QueryToken::FOLLOWS: {
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
    case QueryToken::FOLLOWS_T: {
        // stmtRef , stmtRef
        if (!isStmtRef(query, token_chain)) {
            throw std::invalid_argument("Invalid parameters for Follows*");
        }
        QueryToken stmt = token_chain[0];
        token_chain.erase(token_chain.begin(), token_chain.begin() + 1);

        isCommaRef(token_chain);
        token_chain.erase(token_chain.begin(), token_chain.begin() + 1);

        if (!isStmtRef(query, token_chain)) {
            throw std::invalid_argument("Invalid parameters for Follows*");
        }
        QueryToken stmt2 = token_chain[0];
        token_chain.erase(token_chain.begin(), token_chain.begin() + 1);
        if (token_chain.size() != 0) {
            throw std::invalid_argument("Unexpected parameters for Follows*");
        }
        query.addRelation(RelRef(RelType::FOLLOWS_T,
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
    case ASSIGN:
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
        } else {
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
      } else {
        // if no comma found means invalid parameters
        throw std::runtime_error("Unexpected parameters for Pattern");
      }
      if (token_chain.size() == 0) {
          throw std::invalid_argument("Invalid argument, no expr found");
      }
      // check second parameter if is WILDCARD
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
        if (token_chain[0].type == QueryToken::QUOTATION_OPEN) {
          token_chain.erase(token_chain.begin());

          for (int i = 0; i < token_chain.size(); i++) {
            if (token_chain[0].type != QueryToken::QUOTATION_CLOSE) {
              temp_token_chain_2.push_back(token_chain[0]);
              token_chain.erase(token_chain.begin());
            } else {
              break;
            }
          }
        } else if (wild && token_chain[0].type == QueryToken::WILDCARD) {
          // 2 WILDCARDS in a row
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