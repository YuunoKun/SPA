#include "UsesParser.h"

void UsesParser::parse(Query& query, std::vector<QueryToken> token_chain, QueryToken::QueryTokenType& token_type) {
    // stmtRef , entRef
    // entRef, entRef

    if(token_type != QueryToken::USES_S ) {
        throw std::invalid_argument("Only Uses and its variants for this method");
    }

    bool is_USES_S = true;

    std::vector<std::vector<QueryToken>> separated_params = Utility::splitTokenChain(2, QueryToken::COMMA, token_chain);

	//Validate first param
	if (Utility::isStmtRef(query, separated_params[0])) {
		if (separated_params[0][0].type == QueryToken::WILDCARD) {
			//Throw semantic erros
			query.setIsSemanticError("Invalid parameters for Uses");
		}
	} else {
		// Not stmtRef
		if (Utility::isEntRef(query, separated_params[0], EntityType::PROCEDURE)) {
			is_USES_S = false;
		} else {
			query.setIsSemanticError("Invalid parameters for Uses");
		}
	}

	//Validate second param
	if (!Utility::isEntRef(query, separated_params[1], EntityType::VARIABLE)) {
		query.setIsSemanticError("Invalid parameters for Uses");
	}

	if (is_USES_S) {
		QueryToken stmt = separated_params[0][0];
		if (!Utility::checkIsSemanticError(query)) {
			query.addRelation(RelRef(RelType::USES_S, Utility::setStmtRef(query, stmt, EntityType::STMT),
				Utility::setEntRef(query, separated_params[1], EntityType::VARIABLE)));
		}
	} else {
		if (!Utility::checkIsSemanticError(query)) {
			query.addRelation(RelRef(RelType::USES_P, Utility::setEntRef(query, separated_params[0], EntityType::PROCEDURE),
				Utility::setEntRef(query, separated_params[1], EntityType::VARIABLE)));
		}
	}
}