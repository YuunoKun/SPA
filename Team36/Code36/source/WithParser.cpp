#include "WithParser.h"

void WithParser::parse(Query& query, std::vector<QueryToken> token_chain) {
    std::unordered_map<std::string, Entity> ent_chain = query.getEntities();

    std::vector<std::vector<QueryToken>> separated_params = Utility::splitTokenChain(2, QueryToken::EQUAL, token_chain);

     if (!Utility::isRef(query, separated_params[0]) || !Utility::isRef(query, separated_params[1])) {
        query.setIsSemanticError("Invalid parameters for With");
    }


    if (!isSameRefType(query, separated_params[0], separated_params[1])) {
        query.setIsSemanticError("Invalid parameters for With");
    }

    EntityType ident_type = EntityType::NONE_ENTITY;
    AttrRef attr_name = AttrRef::NONE;
    if (!Utility::checkIsSemanticError(query)) {

        EntityType ent_type_1 = getEntityType(query, separated_params[0]);
        EntityType ent_type_2 = getEntityType(query, separated_params[1]);

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

        AttrRef attr_name_1 = getAttrName(query, separated_params[0]);
        AttrRef attr_name_2 = getAttrName(query, separated_params[1]);

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
            Utility::setRef(query, separated_params[0], ident_type, attr_name),
            Utility::setRef(query, separated_params[1], ident_type, attr_name)));
    }
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

AttrRef WithParser::getAttrName(Query& query, std::vector<QueryToken> token_chain) {

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
            default: query.setIsSemanticError("Unknown AttrRef for with cl");
                break;
            }

            
        }
    }

}


