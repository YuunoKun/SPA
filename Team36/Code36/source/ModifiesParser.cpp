#include "ModifiesParser.h"

void ModifiesParser::parse(Query& query, std::vector<QueryToken> token_chain) {
	// stmtRef , entRef
	   // entRef, entRef

	bool is_MODIFIES_S = true;
	std::vector<std::vector<QueryToken>> separated_params = Utility::splitTokenChain(2, QueryToken::COMMA, token_chain);

	//Validate first param
	if (Utility::isStmtRef(query, separated_params[0])) {
		if (separated_params[0][0].type == QueryToken::WILDCARD) {
			query.setIsSemanticError("Invalid parameters for Modifies");
		}
	} else {
		// Not stmtRef
		if (Utility::isEntRef(query, separated_params[0], EntityType::PROCEDURE)) {
			is_MODIFIES_S = false;
		} else {
			//Not stmtRef or entRef
			query.setIsSemanticError("Invalid parameters for Modifies");
		}
	}

	//Validate second param
	if (!Utility::isEntRef(query, separated_params[1], EntityType::VARIABLE)) {
		query.setIsSemanticError("Invalid parameters for Modifies");
	}

	if (is_MODIFIES_S) {
		QueryToken stmt = separated_params[0][0];
		if (!Utility::checkIsSemanticError(query)) {
			query.addRelation(RelRef(RelType::MODIFIES_S, Utility::setStmtRef(query, stmt, EntityType::STMT),
				Utility::setEntRef(query, separated_params[1], EntityType::VARIABLE)));
		}
	} else {
		if (!Utility::checkIsSemanticError(query)) {
			query.addRelation(RelRef(RelType::MODIFIES_P, Utility::setEntRef(query, separated_params[0], EntityType::PROCEDURE),
				Utility::setEntRef(query, separated_params[1], EntityType::VARIABLE)));
		}
	}
}