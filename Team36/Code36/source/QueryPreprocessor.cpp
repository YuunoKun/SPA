#include "QueryPreprocessor.h"
#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include "Common.h"
#include "Entity.h"
#include "QueryToken.h"
#include "QueryTokenizer.h"
#include "QueryValidator.h"
#include "Query.h"
#include "QueryClauseParser.h"
#include "Utility.h"
#include "SemanticErrorException.h"
#include "SyntacticErrorException.h"

QueryPreprocessor::QueryPreprocessor() {
	this->query = Query();

	// True when parsing is in selection, false when parsing is in declaration
	this->isSelect = false;

	// True when iterating inside the such that or pattern parameter, false otherwise
	this->isParameter = false;

	// True when the next token must be a pattern type
	this->isExpectingPatternType = false;

	// True after current token is a '.', next one must be an attribute
	this->isExpectingAttribute = false;

	// Anything in between a clause eg. such that ..A.. pattern -> A is not endOfCurrentClause
	this->endOfCurrentClauses = true;

	// Collection of declared query tokens
	this->output.clear();

	// To keep track of parameter content in such that and pattern clauses
	this->parameterClause.clear();

	// To keep track of previous token during declaration, includes COMMA and SEMICOLON
	this->prevToken = QueryToken();

	// To keep track of previous important declaration token, excludes COMMA and SEMICOLON
	this->declarationType = QueryToken();

	// To keep track if there another declaration
	this->haveNextDeclaration = false;

	// Keep track if it the end of current declaration by terminator
	this->endOfCurrentDeclaration = true;

	// To keep track of previous token during Selection
	this->prevTokenSelect = QueryToken();

	// To keep track of valid such that clause type, eg: Uses, Modifies, Parent, ParentT
	this->queryParameter = QueryToken();

	// To keep track if selection is in pattern or such that
	this->patternOrSuchThat = QueryToken();

	// To keep track of pattern type entity, eg: ASSIGN
	this->patternTypeEntity = Entity();

	// To keep track of selection, NEUTRAL -> IS_SELECTING -> IS_SELECTING_MULTIPLE_CLAUSE
	this->status = ParseStatus::NEUTRAL;

	// Keep track of parenthesis counter for ExprParser purposes
	this->parenthesis_counter = 0;

	// Peek into next token
	this->nextToken = QueryToken();
}

Query QueryPreprocessor::parse(std::string str) {
	QueryTokenizer query_tokenizer;
	QueryValidator queryValidator = QueryValidator();

	query_tokenizer.tokenize(this->query, str);

	std::vector<QueryToken> tokens = query_tokenizer.get_query_token_chain();

	for (int i = 0; i < tokens.size(); i++) {
		// First iteration, set identifier to correct type
		// Check what is my previous token
		if (!isSelect) {
			handleDeclaration(tokens[i]);
		}
		else if (isSelect) {
			if (i < tokens.size() - 1) {
				nextToken = tokens[i + 1];
			}
			else {
				nextToken = QueryToken();
			}
			handleSelection(tokens[i]);
		}
	}
	queryValidator.validateQuery(query, endOfCurrentClauses);

	return QueryPreprocessor::returnAndResetQuery();
}

void QueryPreprocessor::handleDeclaration(QueryToken& token) {
	if (endOfCurrentDeclaration) {
		setIdentifierToQueryTokenType(token);
		haveNextDeclaration = true;
		endOfCurrentDeclaration = false;
	}
	else if (haveNextDeclaration && token.type == QueryToken::QueryTokenType::IDENTIFIER) {
		addEntityToQuery(token);
		haveNextDeclaration = false;
	}
	else if (!haveNextDeclaration && !endOfCurrentDeclaration && token.type == QueryToken::QueryTokenType::TERMINATOR) {
		endOfCurrentDeclaration = true;
	}
	else if (!haveNextDeclaration && !endOfCurrentDeclaration && token.type == QueryToken::QueryTokenType::COMMA) {
		haveNextDeclaration = true;
	}
	else {
		throw SyntacticErrorException("Invalid declaration");
	}
	prevToken = token;
}

void QueryPreprocessor::handleSelection(QueryToken& token) {
	if (endOfCurrentClauses) {
		// Select content
		if (status == ParseStatus::IS_SELECTING) {
			QueryPreprocessor::handleIsSelecting(token);
		}
		else if (status == ParseStatus::IS_SELECTING_MULTIPLE_CLAUSE) {
			QueryPreprocessor::handleSelectingMultipleClause(token);
		}
		else if (token.type == QueryToken::QueryTokenType::SUCH_THAT) {
			QueryPreprocessor::handleSuchThatClause();
		}
		// pattern only valid when previous token is either identifier or )
		else if (token.type == QueryToken::QueryTokenType::IDENTIFIER &&
			token.token_value == PATTERN_STR) {
			QueryPreprocessor::handlePatternClause();
		}
		else if (token.type == QueryToken::QueryTokenType::IDENTIFIER &&
			token.token_value == WITH_STR) {
			QueryPreprocessor::handleWithClause();
		}
		else if (token.type == QueryToken::QueryTokenType::IDENTIFIER &&
			token.token_value == AND_STR) {
			QueryPreprocessor::handleAnd();
		}
		else {
			throw SyntacticErrorException("Invalid query");
		}
	}
	else {
		QueryValidator queryValidator = QueryValidator();
		queryValidator.validateNotSuchThat(token);

		if (isParameter) {
			handleWithinParameter(token);
		}
		else if (patternOrSuchThat.type == QueryToken::QueryTokenType::SUCH_THAT) {
			QueryPreprocessor::setSuchThatParameter(token);
		}
		else if (patternOrSuchThat.type == QueryToken::QueryTokenType::PATTERN) {
			QueryPreprocessor::setPatternParameter(token);
		}
		else {
			throw SyntacticErrorException("Invalid query");
		}
	}
	prevTokenSelect = token;
}

void QueryPreprocessor::handleSuchThatClause() {
	patternOrSuchThat = { QueryToken::QueryTokenType::SUCH_THAT, "" };
	endOfCurrentClauses = false;
	status = ParseStatus::NEUTRAL;
}

void QueryPreprocessor::handlePatternClause() {
	patternOrSuchThat = { QueryToken::QueryTokenType::PATTERN, PATTERN_STR };
	isExpectingPatternType = true;
	endOfCurrentClauses = false;
	status = ParseStatus::NEUTRAL;
}

void QueryPreprocessor::handleWithClause() {
	patternOrSuchThat = { QueryToken::QueryTokenType::WITH, "" };
	isParameter = true;
	endOfCurrentClauses = false;
	status = ParseStatus::NEUTRAL;
}

void QueryPreprocessor::handleAnd() {
	QueryValidator queryValidator = QueryValidator();
	queryValidator.validateAnd(patternOrSuchThat, nextToken, output);
	if (patternOrSuchThat.type == QueryToken::QueryTokenType::PATTERN) {
		isExpectingPatternType = true;
	}
	else if (patternOrSuchThat.type == QueryToken::QueryTokenType::WITH) {
		isParameter = true;
	}
	endOfCurrentClauses = false;
}

void QueryPreprocessor::handleIsSelecting(QueryToken& token) {
	QueryValidator queryValidator = QueryValidator();
	queryValidator.validateSelecting(token, prevTokenSelect);

	// Check next token, if its pattern or such that, go out from is_selecting
	if (nextToken.type != QueryToken::QueryTokenType::WHITESPACE &&
		(nextToken.token_value == PATTERN_STR ||
			nextToken.type == QueryToken::QueryTokenType::SUCH_THAT ||
			nextToken.token_value == WITH_STR)) {
		status = ParseStatus::NEUTRAL;
	}

	// if Select has attribute ie. Select p.procName
	if (token.type == QueryToken::QueryTokenType::DOT &&
		prevToken.type == QueryToken::QueryTokenType::IDENTIFIER) {
		queryValidator.validateAttributeType(query, prevTokenSelect, nextToken);
		isExpectingAttribute = true;
	}
	else if (isExpectingAttribute && prevTokenSelect.type == QueryToken::QueryTokenType::DOT) {
		AttrRef attrRef = Utility::queryTokenTypeToAttrRef(token.type);
		this->query.setSelectedAttribute(attrRef);
		isExpectingAttribute = false;
	}
	else if (prevToken.token_value == SELECT_STR && token.type == QueryToken::QueryTokenType::TUPLE_OPEN) {
		status = ParseStatus::IS_SELECTING_MULTIPLE_CLAUSE;
	}
	else if (prevToken.token_value == SELECT_STR &&
		token.type == QueryToken::QueryTokenType::IDENTIFIER) {
		addSelectedToQuery(token);
		haveNextDeclaration = false;
	}
	else {
		throw SyntacticErrorException("Invalid query during selection parsing");
	}
}

void QueryPreprocessor::handleSelectingMultipleClause(QueryToken& token) {
	QueryValidator queryValidator = QueryValidator();
	queryValidator.validateSelectMultipleClauses(token, prevTokenSelect);

	if (token.type == QueryToken::QueryTokenType::DOT) {
		queryValidator.validateAttributeType(query, prevTokenSelect, nextToken);
		isExpectingAttribute = true;
	}
	else if (isExpectingAttribute) {
		AttrRef attrRef = Utility::queryTokenTypeToAttrRef(token.type);
		this->query.setSelectedAttribute(attrRef);
		isExpectingAttribute = false;
	}
	else if (token.type == QueryToken::QueryTokenType::IDENTIFIER) {
		addSelectedToQuery(token);
		haveNextDeclaration = false;
	}
	else if (token.type == QueryToken::QueryTokenType::TUPLE_CLOSE) {
		status = ParseStatus::NEUTRAL;
	}
}

void QueryPreprocessor::setSuchThatParameter(QueryToken& token) {
	if (!isParameter && token.type == QueryToken::QueryTokenType::PARENTHESIS_OPEN) {
		isParameter = true;
		setQueryParameter();
	}
}

void QueryPreprocessor::setPatternParameter(QueryToken& token) {
	if ((prevTokenSelect.token_value == PATTERN_STR || prevTokenSelect.token_value == AND_STR) && isExpectingPatternType) {
		QueryPreprocessor::addPatternToQuery(token);
		isExpectingPatternType = false;
	}
	else if (!isParameter && token.type == QueryToken::QueryTokenType::PARENTHESIS_OPEN) {
		isParameter = true;
	}
}

void QueryPreprocessor::handleWithinParameter(QueryToken& token) {
	QueryPreprocessor::handleParenthesisOpen(token);
	QueryPreprocessor::handleAddParameterTokensAndParseWith(token);
	QueryPreprocessor::handleValidatePatternAndSuchThat(token);
	QueryPreprocessor::handleParenthesisClose(token);
}

void QueryPreprocessor::handleParenthesisOpen(QueryToken& token) {
	if (token.type == QueryToken::QueryTokenType::PARENTHESIS_OPEN) {
		this->parenthesis_counter++;
	}
}

void QueryPreprocessor::handleAddParameterTokensAndParseWith(QueryToken& token) {
	if ((((patternOrSuchThat.type == QueryToken::QueryTokenType::PATTERN ||
		patternOrSuchThat.type == QueryToken::QueryTokenType::SUCH_THAT) &&
		(token.type != QueryToken::QueryTokenType::PARENTHESIS_CLOSE || parenthesis_counter > 0)) ||
		patternOrSuchThat.type == QueryToken::QueryTokenType::WITH)) {
		parameterClause.push_back(token);
		QueryPreprocessor::checkParseWith(token);
	}
}

void QueryPreprocessor::checkParseWith(QueryToken& token) {
	if (patternOrSuchThat.type == QueryToken::QueryTokenType::WITH &&
		(token.type == QueryToken::QueryTokenType::QUOTATION_CLOSE ||
			token.type == QueryToken::QueryTokenType::CONSTANT ||
			token.type == QueryToken::QueryTokenType::IDENTIFIER ||
			token.type == QueryToken::QueryTokenType::PROC_NAME ||
			token.type == QueryToken::QueryTokenType::VAR_NAME ||
			token.type == QueryToken::QueryTokenType::VALUE ||
			token.type == QueryToken::QueryTokenType::STMT_INDEX) &&
		(this->nextToken.type == QueryToken::QueryTokenType::WHITESPACE ||
			this->nextToken.token_value == AND_STR ||
			this->nextToken.token_value == PATTERN_STR ||
			this->nextToken.type == QueryToken::QueryTokenType::SUCH_THAT)) {
		QueryClauseParser clauseParser;
		clauseParser.parseWith(query, parameterClause);
		parameterClause.clear();
		isParameter = false;
		endOfCurrentClauses = true;
	}
}

void QueryPreprocessor::handleValidatePatternAndSuchThat(QueryToken& token) {
	if (parenthesis_counter == 0 && token.type == QueryToken::QueryTokenType::PARENTHESIS_CLOSE) {
		if (patternOrSuchThat.type == QueryToken::QueryTokenType::PATTERN) {
			QueryValidator queryValidator = QueryValidator();
			queryValidator.validatePatternType(patternTypeEntity, query);
			isParameter = false;
			endOfCurrentClauses = true;
			QueryClauseParser clauseParser;
			clauseParser.parseParameterPattern(this->query, patternTypeEntity, parameterClause);
			parameterClause.clear();
			parenthesis_counter = 0;
		}
		else if (patternOrSuchThat.type == QueryToken::QueryTokenType::SUCH_THAT) {
			isParameter = false;
			QueryClauseParser clauseParser;
			clauseParser.parseParameterSuchThat(this->query, queryParameter.type, parameterClause);
			parameterClause.clear();
			endOfCurrentClauses = true;
			parenthesis_counter = 0;
		}
	}
}

void QueryPreprocessor::handleParenthesisClose(QueryToken& token) {
	if (parenthesis_counter > 0 && token.type == QueryToken::QueryTokenType::PARENTHESIS_CLOSE) {
		parenthesis_counter--;
	}
}

void QueryPreprocessor::setIdentifierToQueryTokenType(QueryToken& token) {
	const auto isFirstToken = prevToken.type == QueryToken::QueryTokenType::WHITESPACE;
	const auto isNewToken = prevToken.type == QueryToken::QueryTokenType::TERMINATOR;
	const auto isStmtToken = token.token_value == STMT_STR;
	const auto isProcedureToken = token.token_value == PROCEDURE_STR;
	const auto isReadToken = token.token_value == READ_STR;
	const auto isPrintToken = token.token_value == PRINT_STR;
	const auto isCallToken = token.token_value == CALL_STR;
	const auto isIfToken = token.token_value == IF_STR;
	const auto isWhileToken = token.token_value == WHILE_STR;
	const auto isAssignToken = token.token_value == ASSIGN_STR;
	const auto isVariableToken = token.token_value == VARIABLE_STR;
	const auto isConstantToken = token.token_value == CONSTANT_STR;
	const auto isProgLineToken = token.type == QueryToken::QueryTokenType::PROG_LINE;
	const auto isSelectCondition = (token.token_value == SELECT_STR) && (prevTokenSelect.type == QueryToken::QueryTokenType::WHITESPACE || prevTokenSelect.type == QueryToken::QueryTokenType::TERMINATOR);

	if (isFirstToken || isNewToken) {
		if (isStmtToken) {
			declarationType = { QueryToken::QueryTokenType::STMT, STMT_STR };
		}
		else if (isProcedureToken) {
			declarationType = { QueryToken::QueryTokenType::PROCEDURE, PROCEDURE_STR };
		}
		else if (isReadToken) {
			declarationType = { QueryToken::QueryTokenType::READ,READ_STR };
		}
		else if (isPrintToken) {
			declarationType = { QueryToken::QueryTokenType::PRINT,PRINT_STR };
		}
		else if (isCallToken) {
			declarationType = { QueryToken::QueryTokenType::CALL, CALL_STR };
		}
		else if (isIfToken) {
			declarationType = { QueryToken::QueryTokenType::IF, IF_STR };
		}
		else if (isWhileToken) {
			declarationType = { QueryToken::QueryTokenType::WHILE, WHILE_STR };
		}
		else if (isAssignToken) {
			declarationType = { QueryToken::QueryTokenType::ASSIGN, ASSIGN_STR };
		}
		else if (isVariableToken) {
			declarationType = { QueryToken::QueryTokenType::VARIABLE, VARIABLE_STR };
		}
		else if (isConstantToken) {
			declarationType = { QueryToken::QueryTokenType::CONSTANT, CONSTANT_STR };
		}
		else if (isProgLineToken) {
			declarationType = { QueryToken::QueryTokenType::PROG_LINE, PROGLINE_STR };
		}
		// Need to enforce that Select must only come after a terminator
		else if (isSelectCondition) {
			isSelect = true;
			status = ParseStatus::IS_SELECTING;
		}
		else {
			throw SyntacticErrorException("Invalid syntax for declaration or select");
		}
	}
}

void QueryPreprocessor::addEntityToQuery(QueryToken& token) {
	// Check if entity name is already used, exists in output, should return error
	Entity ent;
	for (QueryToken each : this->output) {
		if (token.token_value == each.token_value) {
			this->query.setIsSemanticError("Name is already used!");
		}
	}

	// Declaring goes into output
	this->output.push_back({ declarationType.type, { token.token_value } });
	Synonym synonym;
	synonym.name = token.token_value;
	EntityType entityType = Utility::queryTokenTypeToEntityType(declarationType.type);
	ent = { entityType, synonym };
	this->query.addEntity(ent);
}

void QueryPreprocessor::addPatternToQuery(QueryToken& token) {
	QueryValidator queryValidator;
	queryValidator.isExpectingIdentifier(token);
	bool isValid = false;

	QueryPreprocessor::updateEntityType(token, patternTypeEntity, isValid);

	if (!isValid) {
		this->query.setIsSemanticError("Pattern type has not been declared");
	}
}

void QueryPreprocessor::addSelectedToQuery(QueryToken& token) {
	Entity ent;
	bool isValid = false;

	QueryPreprocessor::updateEntityType(token, ent, isValid);

	if (!isValid && token.token_value == "BOOLEAN" && this->query.getSelected().size() == 0) {
		ent = { EntityType::BOOLEAN };
		isValid = true;
	}

	if (!isValid) {
		this->query.setIsSemanticError("Select variable content has not been declared");
	}
	else {
		this->query.addSelected(ent);
	}
}

void QueryPreprocessor::updateEntityType(QueryToken& token, Entity& ent, bool& isValid) {
	for (QueryToken declaredToken : this->output) {
		if (token.token_value == declaredToken.token_value) {
			Synonym synonym;
			synonym.name = token.token_value;
			EntityType entityType = Utility::queryTokenTypeToEntityType(declaredToken.type);
			ent = { entityType, synonym };
			isValid = true;
		}
	}
}

void QueryPreprocessor::setQueryParameter() {
	const auto isUsesToken = prevTokenSelect.token_value == USES_STR && prevTokenSelect.type == QueryToken::QueryTokenType::IDENTIFIER;
	const auto isModifiesToken = prevTokenSelect.token_value == MODIFIES_STR && prevTokenSelect.type == QueryToken::QueryTokenType::IDENTIFIER;
	const auto isParentToken = prevTokenSelect.token_value == PARENT_STR && prevTokenSelect.type == QueryToken::QueryTokenType::IDENTIFIER;
	const auto isParentTToken = prevTokenSelect.token_value == PARENTT_STR && prevTokenSelect.type == QueryToken::QueryTokenType::PARENT_T;
	const auto isFollowsToken = prevTokenSelect.token_value == FOLLOWS_STR && prevTokenSelect.type == QueryToken::QueryTokenType::IDENTIFIER;
	const auto isFollowsTToken = prevTokenSelect.token_value == FOLLOWST_STR && prevTokenSelect.type == QueryToken::QueryTokenType::FOLLOWS_T;
	const auto isCallsToken = prevTokenSelect.token_value == CALLS_STR && prevTokenSelect.type == QueryToken::QueryTokenType::IDENTIFIER;
	const auto isCallsTToken = prevTokenSelect.token_value == CALLST_STR && prevTokenSelect.type == QueryToken::QueryTokenType::CALLS_T;
	const auto isNextToken = prevTokenSelect.token_value == NEXT_STR && prevTokenSelect.type == QueryToken::QueryTokenType::IDENTIFIER;
	const auto isNextTToken = prevTokenSelect.token_value == NEXTT_STR && prevTokenSelect.type == QueryToken::QueryTokenType::NEXT_T;
	const auto isAffectsToken = prevTokenSelect.token_value == AFFECTS_STR && prevTokenSelect.type == QueryToken::QueryTokenType::IDENTIFIER;
	const auto isAffectsTToken = prevTokenSelect.token_value == AFFECTST_STR && prevTokenSelect.type == QueryToken::QueryTokenType::AFFECTS_T;
	const auto isAffectsBipToken = prevTokenSelect.token_value == AFFECTS_BIP_STR && prevTokenSelect.type == QueryToken::QueryTokenType::IDENTIFIER;
	const auto isAffectsBipTToken = prevTokenSelect.token_value == AFFECTS_BIPT_STR && prevTokenSelect.type == QueryToken::QueryTokenType::AFFECTS_BIP_T;
	const auto isNextBipToken = prevTokenSelect.token_value == NEXT_BIP_STR && prevTokenSelect.type == QueryToken::QueryTokenType::IDENTIFIER;
	const auto isNextBipTToken = prevTokenSelect.token_value == NEXT_BIPT_STR && prevTokenSelect.type == QueryToken::QueryTokenType::NEXT_BIP_T;

	// USES_P to be handled in PatternRelRefValidator
	if (isUsesToken) {
		queryParameter = { QueryToken::QueryTokenType::USES_S, USES_STR };
	}
	// MODIFIES_P to be handled in PatternRelRefValidator
	else if (isModifiesToken) {
		queryParameter = { QueryToken::QueryTokenType::MODIFIES_S, MODIFIES_STR };
	}
	else if (isParentToken) {
		queryParameter = { QueryToken::QueryTokenType::PARENT, PARENT_STR };
	}
	else if (isParentTToken) {
		queryParameter = { QueryToken::QueryTokenType::PARENT_T, PARENTT_STR };
	}
	else if (isFollowsToken) {
		queryParameter = { QueryToken::QueryTokenType::FOLLOWS, FOLLOWS_STR };
	}
	else if (isFollowsTToken) {
		queryParameter = { QueryToken::QueryTokenType::FOLLOWS_T, FOLLOWST_STR };
	}
	else if (isCallsToken) {
		queryParameter = { QueryToken::QueryTokenType::CALLS, CALLS_STR };
	}
	else if (isCallsTToken) {
		queryParameter = { QueryToken::QueryTokenType::CALLS_T, CALLST_STR };
	}
	else if (isNextToken) {
		queryParameter = { QueryToken::QueryTokenType::NEXT, NEXT_STR };
	}
	else if (isNextTToken) {
		queryParameter = { QueryToken::QueryTokenType::NEXT_T, NEXTT_STR };
	}
	else if (isAffectsToken) {
		queryParameter = { QueryToken::QueryTokenType::AFFECTS, AFFECTS_STR };
	}
	else if (isAffectsTToken) {
		queryParameter = { QueryToken::QueryTokenType::AFFECTS_T, AFFECTST_STR };
	}
	else if (isAffectsBipToken) {
		queryParameter = { QueryToken::QueryTokenType::AFFECTS_BIP, AFFECTS_BIP_STR };
	}
	else if (isAffectsBipTToken) {
		queryParameter = { QueryToken::QueryTokenType::AFFECTS_BIP_T, AFFECTS_BIPT_STR };
	}
	else if (isNextBipToken) {
		queryParameter = { QueryToken::QueryTokenType::NEXT_BIP, NEXT_BIP_STR };
	}
	else if (isNextBipTToken) {
		queryParameter = { QueryToken::QueryTokenType::NEXT_BIP_T, NEXT_BIPT_STR };
	}
}

Query QueryPreprocessor::returnAndResetQuery() {
	Query query_result = this->query;

	QueryPreprocessor::resetQuery();

	return query_result;
}

void QueryPreprocessor::resetQuery() {
	this->query = Query();
	this->isSelect = false;
	this->isParameter = false;
	this->isExpectingPatternType = false;
	this->isExpectingAttribute = false;
	this->endOfCurrentClauses = true;
	this->output.clear();
	this->parameterClause.clear();
	this->prevToken = QueryToken();
	this->declarationType = QueryToken();
	this->haveNextDeclaration = false;
	this->endOfCurrentDeclaration = true;
	this->prevTokenSelect = QueryToken();
	this->queryParameter = QueryToken();
	this->patternOrSuchThat = QueryToken();
	this->patternTypeEntity = Entity();
	this->status = ParseStatus::NEUTRAL;
	this->parenthesis_counter = 0;
}