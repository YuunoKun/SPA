#include "Common.h"
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
	} else if (token.type == QueryToken::QueryTokenType::DOT &&
		prevTokenSelect.type != QueryToken::QueryTokenType::IDENTIFIER) {
		throw SyntacticErrorException("During multiple selects, '.' can only come after identifier");
	} else if ((token.type == QueryToken::QueryTokenType::PROC_NAME ||
		token.type == QueryToken::QueryTokenType::VAR_NAME ||
		token.type == QueryToken::QueryTokenType::VALUE ||
		token.type == QueryToken::QueryTokenType::STMT_INDEX) &&
		prevTokenSelect.type != QueryToken::QueryTokenType::DOT) {
		throw SyntacticErrorException("During multiple selects, attributes can only come after '.'");
	} else if (token.type == QueryToken::QueryTokenType::COMMA &&
		(prevTokenSelect.type != QueryToken::QueryTokenType::PROC_NAME &&
			prevTokenSelect.type != QueryToken::QueryTokenType::VAR_NAME &&
			prevTokenSelect.type != QueryToken::QueryTokenType::VALUE &&
			prevTokenSelect.type != QueryToken::QueryTokenType::IDENTIFIER &&
			prevTokenSelect.type != QueryToken::QueryTokenType::STMT_INDEX)) {
		throw SyntacticErrorException("During multiple selects, comma can only come after attributes");
	} else if (token.type == QueryToken::QueryTokenType::TUPLE_CLOSE &&
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
			token.token_value != WITH_STR)) {
		throw SyntacticErrorException("After selection needs to have such that or pattern clause or with");
	}
}

void QueryValidator::validateQuery(Query& query, bool& endOfCurrentClauses) {
	if (query.getIsSemanticError() != "") {
		throw SemanticErrorException(query.getIsSemanticError(), query);
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

void QueryValidator::validatePatternType(Entity& patternTypeEntity, Query& query) {
	if (patternTypeEntity.getType() != EntityType::ASSIGN &&
		patternTypeEntity.getType() != EntityType::WHILE &&
		patternTypeEntity.getType() != EntityType::IF) {
		query.setIsSemanticError("Pattern Type is invalid");
	}
}

void QueryValidator::validateAnd(QueryToken& patternOrSuchThat, QueryToken& nextToken, std::vector<QueryToken>& output) {
	if (patternOrSuchThat.type != QueryToken::QueryTokenType::PATTERN &&
		patternOrSuchThat.type != QueryToken::QueryTokenType::SUCH_THAT &&
		patternOrSuchThat.type != QueryToken::QueryTokenType::WITH) {
		throw SyntacticErrorException("The keyword 'and' should come after pattern/ relations have been initalized previously");
	}

	bool patternDeclared = false;
	for (QueryToken declaredToken : output) {
		if (declaredToken.token_value == PATTERN_STR) {
			patternDeclared = true;
		}
	}
	if (nextToken.token_value == PATTERN_STR && !patternDeclared) {
		throw SyntacticErrorException("and pattern is syntactically valid unless pattern is a declared synonym");
	}
}

void QueryValidator::validateAttributeType(Query& query, QueryToken& prevToken, QueryToken& nextToken) {
	std::unordered_map<std::string, Entity> queryEntities = query.getEntities();

	bool isExist = false;
	EntityType entType;
	for (std::pair<std::string, Entity> element : queryEntities) {
		if (element.first == prevToken.token_value) {
			entType = element.second.getType();
			isExist = true;
		}
	}

	if (!isExist) {
		query.setIsSemanticError("Selected Entity has not been declared");
	}

	if (nextToken.type == QueryToken::QueryTokenType::PROC_NAME &&
		(entType != EntityType::PROCEDURE && entType != EntityType::CALL)) {
		query.setIsSemanticError("Only procedure and call can have a procName attribute");
	} else if (nextToken.type == QueryToken::QueryTokenType::VAR_NAME &&
		(entType != EntityType::VARIABLE && entType != EntityType::READ && entType != EntityType::PRINT)) {
		query.setIsSemanticError("Only variable, read and print can have varName attribute");
	} else if (nextToken.type == QueryToken::QueryTokenType::VALUE &&
		entType != EntityType::CONSTANT) {
		query.setIsSemanticError("Only constant can have value attribute");
	} else if (nextToken.type == QueryToken::QueryTokenType::STMT_INDEX &&
		(entType != EntityType::STMT && entType != EntityType::READ && entType != EntityType::PRINT &&
			entType != EntityType::CALL && entType != EntityType::WHILE && entType != EntityType::IF &&
			entType != EntityType::ASSIGN)) {
		query.setIsSemanticError("Entity type for .stmt# attribute is not valid");
	}
}

void QueryValidator::isExpectingIdentifier(QueryToken& nextToken) {
	if (nextToken.type != QueryToken::QueryTokenType::IDENTIFIER) {
		throw SyntacticErrorException("Expected identifier but receives a different token type");
	}
}

void QueryValidator::validateNotSuchThat(QueryToken& token) {
	if (token.type == QueryToken::QueryTokenType::SUCH_THAT) {
		throw SyntacticErrorException("Invalid query, token cannot be such that");
	}
}