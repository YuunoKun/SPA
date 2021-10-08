#include "Query.h"
#include "QueryToken.h"
#include "QueryValidator.h"
#include "QueryPreprocessor.h"
#include "SemanticErrorException.h"
#include "SyntacticErrorException.h"

QueryValidator::QueryValidator() {
}

void QueryValidator::validateSelectMultipleClauses(QueryToken& token, QueryToken& prevTokenSelect) {
	if (token.type == QueryToken::QueryTokenType::IDENTIFIER &&
		prevTokenSelect.type != QueryToken::QueryTokenType::TUPLE_OPEN &&
		prevTokenSelect.type != QueryToken::QueryTokenType::COMMA) {
		throw SyntacticErrorException("During multiple selects, identifier can only come after '<' or ','");
	}
	else if (token.type == QueryToken::QueryTokenType::DOT &&
		prevTokenSelect.type != QueryToken::QueryTokenType::IDENTIFIER) {
		throw SyntacticErrorException("During multiple selects, '.' can only come after identifier");
	}
	else if ((token.type == QueryToken::QueryTokenType::PROC_NAME ||
		token.type == QueryToken::QueryTokenType::VAR_NAME ||
		token.type == QueryToken::QueryTokenType::VALUE ||
		token.type == QueryToken::QueryTokenType::STMT_INDEX) &&
		prevTokenSelect.type != QueryToken::QueryTokenType::DOT) {
		throw SyntacticErrorException("During multiple selects, attributes can only come after '.'");
	}
	else if (token.type == QueryToken::QueryTokenType::COMMA &&
		(prevTokenSelect.type != QueryToken::QueryTokenType::PROC_NAME &&
			prevTokenSelect.type != QueryToken::QueryTokenType::VAR_NAME &&
			prevTokenSelect.type != QueryToken::QueryTokenType::VALUE &&
			prevTokenSelect.type != QueryToken::QueryTokenType::IDENTIFIER &&
			prevTokenSelect.type != QueryToken::QueryTokenType::STMT_INDEX)) {
		throw SyntacticErrorException("During multiple selects, comma can only come after attributes");
	}
	else if (token.type == QueryToken::QueryTokenType::TUPLE_CLOSE &&
		prevTokenSelect.type != QueryToken::QueryTokenType::PROC_NAME &&
		prevTokenSelect.type != QueryToken::QueryTokenType::VAR_NAME &&
		prevTokenSelect.type != QueryToken::QueryTokenType::VALUE &&
		prevTokenSelect.type != QueryToken::QueryTokenType::IDENTIFIER &&
		prevTokenSelect.type != QueryToken::QueryTokenType::STMT_INDEX) {
		throw SyntacticErrorException("During multiple selects, '>' can only come after identifier or attributes");
	}
}

void QueryValidator::validateSelecting(QueryToken& token, QueryToken& prevTokenSelect) {
	if ((prevTokenSelect.type == QueryToken::QueryTokenType::IDENTIFIER ||
		prevTokenSelect.type == QueryToken::QueryTokenType::PROC_NAME ||
		prevTokenSelect.type == QueryToken::QueryTokenType::VAR_NAME ||
		prevTokenSelect.type == QueryToken::QueryTokenType::STMT_INDEX ||
		prevTokenSelect.type == QueryToken::QueryTokenType::VALUE) &&
		(token.type != QueryToken::QueryTokenType::DOT &&
			token.type != QueryToken::QueryTokenType::SUCH_THAT &&
			token.type != QueryToken::QueryTokenType::PATTERN &&
			token.type != QueryToken::QueryTokenType::WITH)) {
		throw SyntacticErrorException("After selection needs to have such that or pattern clause");
	}
}

void QueryValidator::validateQuery(Query& query, bool& endOfCurrentClauses) {
	if (query.getEntities().size() == 0) {
		throw SyntacticErrorException("No declaration has been made in your query");
	}
	if (query.getSelected().size() == 0) {
		throw SyntacticErrorException("There is no selected variable in your query");
	}
	if (!endOfCurrentClauses) {
		throw SyntacticErrorException("Invalid query");
	}

	// Final check

	for (std::pair<std::string, Entity> ent : query.getEntities()) {
		if (ent.second.getType() != EntityType::STMT &&
			ent.second.getType() != EntityType::PROCEDURE &&
			ent.second.getType() != EntityType::READ &&
			ent.second.getType() != EntityType::PRINT &&
			ent.second.getType() != EntityType::CALL &&
			ent.second.getType() != EntityType::IF &&
			ent.second.getType() != EntityType::WHILE &&
			ent.second.getType() != EntityType::VARIABLE &&
			ent.second.getType() != EntityType::CONSTANT &&
			ent.second.getType() != EntityType::PROG_LINE &&
			ent.second.getType() != EntityType::ASSIGN) {
			throw SyntacticErrorException("Declaration fails!");
		}
	}
}

void QueryValidator::validatePatternType(Entity& patternTypeEntity) {
	if (patternTypeEntity.getType() != EntityType::ASSIGN &&
		patternTypeEntity.getType() != EntityType::WHILE &&
		patternTypeEntity.getType() != EntityType::IF) {
		throw SemanticErrorException("Pattern Type is invalid");
	}
}