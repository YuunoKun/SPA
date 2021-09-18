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

//expect stmtref
bool PatternRelRefValidator::isStmtRef(std::vector<QueryToken> token_chain) {
    if (token_chain.size() == 0) {
        throw std::invalid_argument("Invalid argument, no StmtRef found");
    }
    //TODO
    // check if size more than 2

    //if got quotation?
    // return false

    // if wild return true

    // if no quotation
    // need check query if the synonym is proc or statement

}

//expect entref
bool PatternRelRefValidator::isEntRef(std::vector<QueryToken> token_chain) {
    if (token_chain.size() == 0) {
        throw std::invalid_argument("Invalid argument, no EntRef found");
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
  if (token_chain[0].type == QueryToken::QUOTATION_OPEN &&
      token_chain[1].type == QueryToken::IDENTIFIER &&
      token_chain[2].type == QueryToken::QUOTATION_CLOSE) {
    return Entity(EntityType::VARIABLE, token_chain[1].token_value);
  }

  throw std::runtime_error("Unknown stmtRef");
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
      
      isStmtRef(token_chain);
      QueryToken stmt = token_chain[0];
      if (stmt.type == QueryToken::WILDCARD) {
          throw std::runtime_error("Invalid parameters for Modifies");
      }
      token_chain.erase(token_chain.begin(), token_chain.begin() + 1);
      
      isCommaRef(token_chain);
      token_chain.erase(token_chain.begin(), token_chain.begin() + 1);

      isEntRef(token_chain);
      query.addRelation(RelRef(RelType::MODIFIES_S, setStmtRef(query, stmt),
          setEntRef(query, token_chain)));

      break;
    }
      
    case QueryToken::USES_S:
      // stmtRef , entRef
      if (token_chain[1].type == QueryToken::COMMA) {
        QueryToken stmt = token_chain[0];

        if (stmt.type == QueryToken::WILDCARD) {
            throw std::runtime_error("Invalid parameters for Uses");
        }

        token_chain.erase(token_chain.begin(), token_chain.begin() + 2);
        query.addRelation(RelRef(RelType::USES_S, setStmtRef(query, stmt),
                                 setEntRef(query, token_chain)));
      }
      else {
          throw std::runtime_error("Unexpected parameters for Uses");
      }
      break;

    case QueryToken::PARENT:
      // stmtRef , stmtRef
      if (token_chain[1].type == QueryToken::COMMA) {
        query.addRelation(RelRef(RelType::PARENT,
                                 setStmtRef(query, token_chain[0]),
                                 setStmtRef(query, token_chain[2])));
      }
      else {
          throw std::runtime_error("Unexpected parameters for Parent");
      }
      break;
    case QueryToken::PARENT_T:
      // stmtRef , stmtRef
      if (token_chain[1].type == QueryToken::COMMA) {
        query.addRelation(RelRef(RelType::PARENT_T,
                                 setStmtRef(query, token_chain[0]),
                                 setStmtRef(query, token_chain[2])));
      }
      else {
          throw std::runtime_error("Unexpected parameters for Parent*");
      }

      break;
    case QueryToken::FOLLOWS:
      // stmtRef , stmtRef
      if (token_chain[1].type == QueryToken::COMMA) {
        query.addRelation(RelRef(RelType::FOLLOWS,
                                 setStmtRef(query, token_chain[0]),
                                 setStmtRef(query, token_chain[2])));
      }
      else {
          throw std::runtime_error("Unexpected parameters for Follows");
      }

      break;
    case QueryToken::FOLLOWS_T:
      // stmtRef , stmtRef
      if (token_chain[1].type == QueryToken::COMMA) {
        query.addRelation(RelRef(RelType::FOLLOWS_T,
                                 setStmtRef(query, token_chain[0]),
                                 setStmtRef(query, token_chain[2])));
      }
      else {
          throw std::runtime_error("Unexpected parameters for Follows*");
      }

      break;

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
      std::vector<QueryToken> temp_token_chain;
      for (int i = 0; i < token_chain.size(); i++) {
        if (token_chain[0].type != QueryToken::COMMA) {
          temp_token_chain.push_back(token_chain[0]);
          token_chain.erase(token_chain.begin());
        } else {
          break;
        }
      }

      // check if the curr token_chain starts with comma
      if (token_chain[0].type == QueryToken::COMMA) {
        token_chain.erase(token_chain.begin());
      } else {
        // if no comma found means invalid parameters
        throw std::runtime_error("Unexpected parameters for Pattern");
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

      query.addPattern(Pattern(synonym_ent, setEntRef(query, temp_token_chain),
                               setExpr(temp_token_chain_2), wild));
  }
}