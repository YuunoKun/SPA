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
    // entRef : synonym | ‘_’ | ‘"’ IDENT ‘"’

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

bool QueryPatternRelRefParser::isLineRef(Query& query, std::vector<QueryToken> token_chain) {

    // no args found, throw syntax errors
    if (token_chain.size() == 0) {
        throw SyntacticErrorException("Invalid lineRef arguments");
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

    // check synonym if is program lines
    if (token.type == QueryToken::IDENTIFIER) {
        std::unordered_map<std::string, Entity> ent_chain = query.getEntities();
        if (ent_chain.find(token.token_value) != ent_chain.end()) {
            return ent_chain.at(token.token_value).getType() == EntityType::STMT ||
                ent_chain.at(token.token_value).getType() == EntityType::READ ||
                ent_chain.at(token.token_value).getType() == EntityType::PRINT ||
                ent_chain.at(token.token_value).getType() == EntityType::CALL ||
                ent_chain.at(token.token_value).getType() == EntityType::PROG_LINE ||
                ent_chain.at(token.token_value).getType() == EntityType::WHILE ||
                ent_chain.at(token.token_value).getType() == EntityType::IF ||
                ent_chain.at(token.token_value).getType() == EntityType::ASSIGN;
        }
        else {
            // Undeclared Syn, Cannot find Entity in Query
            throw SyntacticErrorException("Undeclared lineRef");
        }
    }
    return false;
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

bool QueryPatternRelRefParser::isWildCard(std::vector<QueryToken> token_chain) {
    if (token_chain.size() == 0) {
        throw SyntacticErrorException("Invalid arguments, wildcards only");
    } else if (token_chain.size() > 1) {
        throw SyntacticErrorException("Invalid arguments, wildcards only");
    } else if (token_chain[0].type == QueryToken::WILDCARD) {
        return true;
    } else {
        return false;
    }
}

Entity QueryPatternRelRefParser::setStmtRef(Query& query, QueryToken token) {
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

  throw SemanticErrorException("Unknown stmtRef");
}

Entity QueryPatternRelRefParser::setEntRef(Query& query,
                                    std::vector<QueryToken> token_chain, EntityType ident_type) {
  // entRef : synonym | ‘_’ | ‘"’ IDENT ‘"’

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
      return Entity(ident_type, token_chain[1].token_value);
  }
  

  throw SemanticErrorException("Unknown entRef");
}

Entity QueryPatternRelRefParser::setLineRef(Query& query, QueryToken token) {
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

    throw SemanticErrorException("Unknown lineRef");
}

std::string QueryPatternRelRefParser::setExpr(std::vector<QueryToken> token_chain) {
  // expression-spec : ‘"‘ expr’"’ | ‘_’ ‘"’ expr ‘"’ ‘_’ | ‘_’
    std::string result = "";
    if (token_chain.size() == 1 && token_chain[0].type == QueryToken::WILDCARD) {
        return result;
    } else {
        for (size_t i = 0; i < token_chain.size(); i++) {
            if (token_chain[i].type == QueryToken::IDENTIFIER|| token_chain[i].type == QueryToken::CONSTANT) {
                result += token_chain[i].token_value;
            } else if (token_chain[i].type == QueryToken::PLUS) {
                result += "+";
            } else if (token_chain[i].type == QueryToken::MINUS) {
                result += "-";
            } else if (token_chain[i].type == QueryToken::MUL) {
                result += "*";
            } else if (token_chain[i].type == QueryToken::DIV) {
                result += "/";
            } else if (token_chain[i].type == QueryToken::MOD) {
                result += "%";
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
        int comma_count = 0;
        bool is_MODIFIES_S = true;
        size_t token_chain_size = token_chain.size();
        for (size_t i = 0; i < token_chain_size; i++) {
            if (token_chain[0].type == QueryToken::COMMA) {
                token_chain.erase(token_chain.begin());
                comma_count ++;
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
            } else {
                throw SyntacticErrorException("Invalid parameters for Modifies");
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
                setEntRef(query, temp_token_chain_2, EntityType::VARIABLE)));

            break;
        } else {
            query.addRelation(RelRef(RelType::MODIFIES_P, setEntRef(query, temp_token_chain_1, EntityType::VARIABLE),
                setEntRef(query, temp_token_chain_2, EntityType::VARIABLE)));
            break;
        }
    }
    
    case QueryToken::USES_S: {
        UsesParser usesParser;
        usesParser.parse(query, token_chain);
        break;
    }

    case QueryToken::PARENT: {
        // stmtRef , stmtRef

        std::vector<QueryToken> temp_token_chain_1;
        std::vector<QueryToken> temp_token_chain_2;
        int comma_count = 0;
        size_t token_chain_size = token_chain.size();
        for (size_t i = 0; i < token_chain_size; i++) {
            if (token_chain[0].type == QueryToken::COMMA) {
                token_chain.erase(token_chain.begin());
                comma_count ++;
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
            } else {
                throw SyntacticErrorException("Invalid parameters for Parent");
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
        int comma_count = 0;
        size_t token_chain_size = token_chain.size();
        for (size_t i = 0; i < token_chain_size; i++) {
            if (token_chain[0].type == QueryToken::COMMA) {
                token_chain.erase(token_chain.begin());
                comma_count ++;
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
            } else {
                throw SyntacticErrorException("Invalid parameters for Parent*");
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
        int comma_count = false;
        size_t token_chain_size = token_chain.size();
        for (size_t i = 0; i < token_chain_size; i++) {
            if (token_chain[0].type == QueryToken::COMMA) {
                token_chain.erase(token_chain.begin());
                comma_count ++;
            }
            else if (comma_count == 0) {
                // 1st param
                temp_token_chain_1.push_back(token_chain[0]);
                token_chain.erase(token_chain.begin());
            }
            else if (comma_count == 1){
                // 2nd param
                temp_token_chain_2.push_back(token_chain[0]);
                token_chain.erase(token_chain.begin());
            } else {
                throw SyntacticErrorException("Invalid parameters for Follows");
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
        int comma_count = false;
        size_t token_chain_size = token_chain.size();
        for (size_t i = 0; i < token_chain_size; i++) {
            if (token_chain[0].type == QueryToken::COMMA) {
                token_chain.erase(token_chain.begin());
                comma_count ++;
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
            } else {
                throw SyntacticErrorException("Invalid parameters for Follows*");
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
        int comma_count = 0;
        size_t token_chain_size = token_chain.size();
        for (size_t i = 0; i < token_chain_size; i++) {
            if (token_chain[0].type == QueryToken::COMMA) {
                token_chain.erase(token_chain.begin());
                comma_count ++;
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
            } else {
                throw SyntacticErrorException("Invalid parameters for Calls");
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
            setEntRef(query, temp_token_chain_1, EntityType::PROCEDURE),
            setEntRef(query, temp_token_chain_2, EntityType::PROCEDURE)));
        break;
    }
    
    case QueryToken::CALLS_T: {
        // entRef , entRef

        std::vector<QueryToken> temp_token_chain_1;
        std::vector<QueryToken> temp_token_chain_2;
        int comma_count = 0;
        size_t token_chain_size = token_chain.size();
        for (size_t i = 0; i < token_chain_size; i++) {
            if (token_chain[0].type == QueryToken::COMMA) {
                token_chain.erase(token_chain.begin());
                comma_count ++;
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
            } else {
                throw SyntacticErrorException("Invalid parameters for Calls*");
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
            setEntRef(query, temp_token_chain_1, EntityType::PROCEDURE),
            setEntRef(query, temp_token_chain_2, EntityType::PROCEDURE)));
        break;
    }

    case QueryToken::AFFECTS: {
        // stmtRef , stmtRef

        break;
    }

    case QueryToken::AFFECTS_T: {
        // stmtRef , stmtRef

        break;
    }

    case QueryToken::NEXT: {
        NextParser nextParser;
        nextParser.parse(query, token_chain);
        break;
    }
    
    case QueryToken::NEXT_T: {
        NextTParser nextTParser;
        nextTParser.parse(query, token_chain);
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
        parser.parse(query, synonym_ent, token_chain);
        break;
        }
    case IF: {
        IfPatternParser parser;
        parser.parse(query, synonym_ent, token_chain);
        break;
    }
    case WHILE: {
        WhilePatternParser parser;
        parser.parse(query, synonym_ent, token_chain);
        break;
    }
    }

}