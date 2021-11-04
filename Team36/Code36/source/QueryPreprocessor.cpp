#include "QueryPreprocessor.h"
#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include "Entity.h"
#include "QueryToken.h"
#include "QueryTokenizer.h"
#include "QueryValidator.h"
#include "Query.h"
#include "QueryPatternRelRefParser.h"
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
			token.token_value == "pattern") {
			QueryPreprocessor::handlePatternClause();
		}
		else if (token.type == QueryToken::QueryTokenType::IDENTIFIER &&
			token.token_value == "with") {
			QueryPreprocessor::handleWithClause();
		}
		else if (token.type == QueryToken::QueryTokenType::IDENTIFIER &&
			token.token_value == "and") {
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
	patternOrSuchThat = { QueryToken::QueryTokenType::PATTERN, "pattern" };
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
	queryValidator.validateAnd(patternOrSuchThat);
	if (patternOrSuchThat.type == QueryToken::QueryTokenType::PATTERN) {
		queryValidator.validateNotAndPattern(nextToken, output);
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
		(nextToken.token_value == "pattern" ||
			nextToken.type == QueryToken::QueryTokenType::SUCH_THAT ||
			nextToken.token_value == "with")) {
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
	else if (prevToken.token_value == "Select" && token.type == QueryToken::QueryTokenType::TUPLE_OPEN) {
		status = ParseStatus::IS_SELECTING_MULTIPLE_CLAUSE;
	}
	else if (prevToken.token_value == "Select" &&
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
	if ((prevTokenSelect.token_value == "pattern" || prevTokenSelect.token_value == "and") && isExpectingPatternType) {
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
			this->nextToken.token_value == "and" ||
			this->nextToken.token_value == "pattern" ||
			this->nextToken.type == QueryToken::QueryTokenType::SUCH_THAT)) {
		QueryPatternRelRefParser validator;
		validator.parseWith(query, parameterClause);
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
			QueryPatternRelRefParser validator;
			validator.parseParameterPattern(this->query, patternTypeEntity, parameterClause);
			parameterClause.clear();
			parenthesis_counter = 0;
		}
		else if (patternOrSuchThat.type == QueryToken::QueryTokenType::SUCH_THAT) {
			isParameter = false;
			QueryPatternRelRefParser validator;
			validator.parseParameterSuchThat(this->query, queryParameter.type, parameterClause);
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
	const auto firstToken = prevToken.type == QueryToken::QueryTokenType::WHITESPACE;
	const auto newToken = prevToken.type == QueryToken::QueryTokenType::TERMINATOR;
	const auto token_stmt = token.token_value == "stmt";
	const auto token_procedure = token.token_value == "procedure";
	const auto token_read = token.token_value == "read";
	const auto token_print = token.token_value == "print";
	const auto token_call = token.token_value == "call";
	const auto token_if = token.token_value == "if";
	const auto token_while = token.token_value == "while";
	const auto token_assign = token.token_value == "assign";
	const auto token_variable = token.token_value == "variable";
	const auto token_constant = token.token_value == "constant";
	const auto token_progline = token.type == QueryToken::QueryTokenType::PROG_LINE;
	const auto condition_select = prevToken.type == QueryToken::QueryTokenType::TERMINATOR && token.token_value == "Select";

	if (firstToken || newToken) {
		if (token_stmt) {
			declarationType = { QueryToken::QueryTokenType::STMT, "stmt" };
		}
		else if (token_procedure) {
			declarationType = { QueryToken::QueryTokenType::PROCEDURE, "procedure" };
		}
		else if (token_read) {
			declarationType = { QueryToken::QueryTokenType::READ, "read" };
		}
		else if (token_print) {
			declarationType = { QueryToken::QueryTokenType::PRINT, "print" };
		}
		else if (token_call) {
			declarationType = { QueryToken::QueryTokenType::CALL, "call" };
		}
		else if (token_if) {
			declarationType = { QueryToken::QueryTokenType::IF, "if" };
		}
		else if (token_while) {
			declarationType = { QueryToken::QueryTokenType::WHILE, "while" };
		}
		else if (token_assign) {
			declarationType = { QueryToken::QueryTokenType::ASSIGN, "assign" };
		}
		else if (token_variable) {
			declarationType = { QueryToken::QueryTokenType::VARIABLE, "variable" };
		}
		else if (token_constant) {
			declarationType = { QueryToken::QueryTokenType::CONSTANT, "constant" };
		}
		else if (token_progline) {
			declarationType = { QueryToken::QueryTokenType::PROG_LINE, "prog_line" };
		}
		// Need to enforce that Select must only come after a terminator
		else if (condition_select) {
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
	const auto token_Uses = prevTokenSelect.token_value == "Uses" && prevTokenSelect.type == QueryToken::QueryTokenType::IDENTIFIER;
	const auto token_Modifies = prevTokenSelect.token_value == "Modifies" && prevTokenSelect.type == QueryToken::QueryTokenType::IDENTIFIER;
	const auto token_Parent = prevTokenSelect.token_value == "Parent" && prevTokenSelect.type == QueryToken::QueryTokenType::IDENTIFIER;
	const auto token_ParentT = prevTokenSelect.token_value == "" && prevTokenSelect.type == QueryToken::QueryTokenType::PARENT_T;
	const auto token_Follows = prevTokenSelect.token_value == "Follows" && prevTokenSelect.type == QueryToken::QueryTokenType::IDENTIFIER;
	const auto token_FollowsT = prevTokenSelect.token_value == "" && prevTokenSelect.type == QueryToken::QueryTokenType::FOLLOWS_T;
	const auto token_Calls = prevTokenSelect.token_value == "Calls" && prevTokenSelect.type == QueryToken::QueryTokenType::IDENTIFIER;
	const auto token_CallsT = prevTokenSelect.token_value == "" && prevTokenSelect.type == QueryToken::QueryTokenType::CALLS_T;
	const auto token_Next = prevTokenSelect.token_value == "Next" && prevTokenSelect.type == QueryToken::QueryTokenType::IDENTIFIER;
	const auto token_NextT = prevTokenSelect.token_value == "" && prevTokenSelect.type == QueryToken::QueryTokenType::NEXT_T;
	const auto token_Affects = prevTokenSelect.token_value == "Affects" && prevTokenSelect.type == QueryToken::QueryTokenType::IDENTIFIER;
	const auto token_AffectsT = prevTokenSelect.token_value == "" && prevTokenSelect.type == QueryToken::QueryTokenType::AFFECTS_T;
	const auto token_AffectsBip = prevTokenSelect.token_value == "AffectsBip" && prevTokenSelect.type == QueryToken::QueryTokenType::IDENTIFIER;
	const auto token_AffectsBipT = prevTokenSelect.token_value == "" && prevTokenSelect.type == QueryToken::QueryTokenType::AFFECTS_BIP_T;
	const auto token_NextBip = prevTokenSelect.token_value == "NextBip" && prevTokenSelect.type == QueryToken::QueryTokenType::IDENTIFIER;
	const auto token_NextBipT = prevTokenSelect.token_value == "" && prevTokenSelect.type == QueryToken::QueryTokenType::NEXT_BIP_T;

	// USES_P to be handled in PatternRelRefValidator
	if (token_Uses) {
		queryParameter = { QueryToken::QueryTokenType::USES_S, "Uses" };
	}
	// MODIFIES_P to be handled in PatternRelRefValidator
	else if (token_Modifies) {
		queryParameter = { QueryToken::QueryTokenType::MODIFIES_S, "Modifies" };
	}
	else if (token_Parent) {
		queryParameter = { QueryToken::QueryTokenType::PARENT, "Parent" };
	}
	else if (token_ParentT) {
		queryParameter = { QueryToken::QueryTokenType::PARENT_T, "" };
	}
	else if (token_Follows) {
		queryParameter = { QueryToken::QueryTokenType::FOLLOWS, "Follows" };
	}
	else if (token_FollowsT) {
		queryParameter = { QueryToken::QueryTokenType::FOLLOWS_T, "" };
	}
	else if (token_Calls) {
		queryParameter = { QueryToken::QueryTokenType::CALLS, "Calls" };
	}
	else if (token_CallsT) {
		queryParameter = { QueryToken::QueryTokenType::CALLS_T, "" };
	}
	else if (token_Next) {
		queryParameter = { QueryToken::QueryTokenType::NEXT, "Next" };
	}
	else if (token_NextT) {
		queryParameter = { QueryToken::QueryTokenType::NEXT_T, "" };
	}
	else if (token_Affects) {
		queryParameter = { QueryToken::QueryTokenType::AFFECTS, "Affects" };
	}
	else if (token_AffectsT) {
		queryParameter = { QueryToken::QueryTokenType::AFFECTS_T, "" };
	}
	else if (token_AffectsBip) {
		queryParameter = { QueryToken::QueryTokenType::AFFECTS_BIP, "AffectsBip" };
	}
	else if (token_AffectsBipT) {
		queryParameter = { QueryToken::QueryTokenType::AFFECTS_BIP_T, "" };
	}
	else if (token_NextBip) {
		queryParameter = { QueryToken::QueryTokenType::NEXT_BIP, "NextBip" };
	}
	else if (token_NextBipT) {
		queryParameter = { QueryToken::QueryTokenType::NEXT_BIP_T, "" };
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