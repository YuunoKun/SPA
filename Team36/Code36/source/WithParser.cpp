#include "WithParser.h"

void WithParser::parse(Query& query, std::vector<QueryToken> token_chain) {
    std::vector<QueryToken> temp_token_chain_1;
    std::vector<QueryToken> temp_token_chain_2;
    EntityType ident_type = EntityType::NONE_ENTITY;
    AttrRef attr_name = AttrRef::NONE;
    std::unordered_map<std::string, Entity> ent_chain = query.getEntities();
    int comma_count = 0;
    size_t token_chain_size = token_chain.size();
    for (size_t i = 0; i < token_chain_size; i++) {
        if (token_chain[0].type == QueryToken::EQUAL) {
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
            throw SyntacticErrorException("Invalid parameters for With");
        }
    }

     if (!Utility::isRef(query, temp_token_chain_1) || !Utility::isRef(query, temp_token_chain_2)) {
        throw SemanticErrorException("Invalid parameters for With");
    }


    if (!isSameRefType(query, temp_token_chain_1, temp_token_chain_2)) {
        throw SemanticErrorException("Invalid parameters for With");
    }

    EntityType ent_type_1 = getEntityType(query, temp_token_chain_1);
    EntityType ent_type_2 = getEntityType(query, temp_token_chain_2);

    if (ent_type_1 == EntityType::NONE_ENTITY &&
        ent_type_2 == EntityType::NONE_ENTITY) {
        ident_type = EntityType::BOOLEAN;
    }
    else if (ent_type_1 == EntityType::NONE_ENTITY) {
        ident_type = ent_type_2;
    }
    else if (ent_type_2 == EntityType::NONE_ENTITY) {
        ident_type = ent_type_1;
    }

    AttrRef attr_name_1 = getAttrName(temp_token_chain_1);
    AttrRef attr_name_2 = getAttrName(temp_token_chain_2);

    if (attr_name_1 == AttrRef::NONE &&
        attr_name_2 == AttrRef::NONE) {
        attr_name = NONE;
    }
    else if (attr_name_1 == AttrRef::NONE) {
        attr_name = attr_name_2;
    }
    else if (attr_name_2 == AttrRef::NONE) {
        attr_name = attr_name_1;
    }

    query.addRelation(RelRef(RelType::WITH,
        Utility::setRef(query, temp_token_chain_1, ident_type, attr_name),
        Utility::setRef(query, temp_token_chain_2, ident_type, attr_name)));

}

bool WithParser::isSameRefType(Query& query, std::vector<QueryToken> token_chain_1, std::vector<QueryToken> token_chain_2) {
    if (Utility::isStringRefType(query, token_chain_1) &&
        Utility::isStringRefType(query, token_chain_2)) {
        return true;
    }
    else if (Utility::isIntRefType(query, token_chain_1) &&
        Utility::isIntRefType(query, token_chain_2)) {
        return true;
    }
    else {
        return false;
    }
}

EntityType WithParser::getEntityType(Query& query, std::vector<QueryToken> token_chain) {
    std::unordered_map<std::string, Entity> ent_chain = query.getEntities();

    if (token_chain.size() == 1) {
        if (token_chain[0].type == QueryToken::CONSTANT) {
            return EntityType::NONE_ENTITY;
        }
        else if (token_chain[0].type == QueryToken::IDENTIFIER) {
            return ent_chain.at(token_chain[0].token_value).getType();
        }
    }
    else if (token_chain.size() == 3) {
        if (token_chain[0].type == QueryToken::QUOTATION_OPEN &&
            token_chain[1].type == QueryToken::IDENTIFIER &&
            token_chain[2].type == QueryToken::QUOTATION_CLOSE) {
            return EntityType::NONE_ENTITY;
        }
        else {
            return ent_chain.at(token_chain[0].token_value).getType();
        }
    }

}

AttrRef WithParser::getAttrName(std::vector<QueryToken> token_chain) {

    if (token_chain.size() == 1) {
        if (token_chain[0].type == QueryToken::CONSTANT) {
            return AttrRef::NONE;
        }
        else if (token_chain[0].type == QueryToken::IDENTIFIER) {
            return AttrRef::STMT_INDEX;
        }
    }
    else if (token_chain.size() == 3) {
        if (token_chain[0].type == QueryToken::QUOTATION_OPEN &&
            token_chain[1].type == QueryToken::IDENTIFIER &&
            token_chain[2].type == QueryToken::QUOTATION_CLOSE) {
            return AttrRef::NONE;
        }
        else {
            switch (token_chain[2].type) {
            case QueryToken::VAR_NAME: return AttrRef::VAR_NAME;
                break;
            case QueryToken::PROC_NAME: return AttrRef::PROC_NAME;
                break;
            case QueryToken::STMT_INDEX: return AttrRef::STMT_INDEX;
                break;
            case QueryToken::VALUE: return AttrRef::VALUE;
                break;
            default: throw SemanticErrorException("Unknown AttrRef for with cl");
                break;
            }

            
        }
    }

}


