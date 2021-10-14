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

	// Collection of selected query tokens
	this->selected.clear();

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

	this->nextToken = QueryToken();
}

Query QueryPreprocessor::parse(std::string str) {
	QueryTokenizer query_tokenizer;
	QueryValidator queryValidator = QueryValidator();

	query_tokenizer.parse_into_query_tokens(str);

	std::vector<QueryToken> tokens = query_tokenizer.get_query_token_chain();

	for (int i = 0; i < tokens.size(); i++) {
		// First iteration, set identifier to correct type
		// Check what is my previous token
		if (!isSelect) {
			handleDeclaration(tokens[i]);
		}
		// assign pattern ; Select pattern pattern pattern (_,_)
		else if (isSelect) {
			if (i < tokens.size() - 1) {
				nextToken = tokens[i + 1];
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
	QueryValidator queryValidator = QueryValidator();
	if (endOfCurrentClauses) {
		// Select content
		//else if (declarationType.type == QueryToken::QueryTokenType::SELECT) {
		if (status == ParseStatus::IS_SELECTING) {
			// Validation
			queryValidator.validateSelecting(token, prevTokenSelect);

			// Check next token, if its pattern or such that, go out from is_selecting
			if (nextToken.type != QueryToken::QueryTokenType::WHITESPACE && (nextToken.token_value == "pattern" || nextToken.type == QueryToken::QueryTokenType::SUCH_THAT)) {
				status = ParseStatus::NEUTRAL;
			}

			// if Select has attribute ie. Select p.procName
			if (token.type == QueryToken::QueryTokenType::DOT &&
				prevToken.type == QueryToken::QueryTokenType::IDENTIFIER) {
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
		else if (status == ParseStatus::IS_SELECTING_MULTIPLE_CLAUSE) {
			queryValidator.validateSelectMultipleClauses(token, prevTokenSelect);
			if (token.type == QueryToken::QueryTokenType::DOT) {
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
		else if (token.type == QueryToken::QueryTokenType::SUCH_THAT) {
			patternOrSuchThat = { QueryToken::QueryTokenType::SUCH_THAT, "" };
			endOfCurrentClauses = false;
			status = ParseStatus::NEUTRAL;
		}
		// pattern only valid when previous token is either identifier or )
		else if (token.type == QueryToken::QueryTokenType::IDENTIFIER &&
			token.token_value == "pattern") {
			isExpectingPatternType = true;
			patternOrSuchThat = { QueryToken::QueryTokenType::PATTERN, "pattern" };
			endOfCurrentClauses = false;
			status = ParseStatus::NEUTRAL;
		}
		else if (token.type == QueryToken::QueryTokenType::IDENTIFIER &&
			token.token_value == "with") {
			patternOrSuchThat = { QueryToken::QueryTokenType::WITH, "" };
			isParameter = true;
			endOfCurrentClauses = false;
			status = ParseStatus::NEUTRAL;
		}
		else if (token.type == QueryToken::QueryTokenType::IDENTIFIER &&
			token.token_value == "and") {
			queryValidator.validateAnd(patternOrSuchThat);
			if (patternOrSuchThat.type == QueryToken::QueryTokenType::PATTERN) {
				isExpectingPatternType = true;
			}
			else if (patternOrSuchThat.type == QueryToken::QueryTokenType::WITH) {
				isParameter = true;
			}
			endOfCurrentClauses = false;
		}

		else {
			throw SyntacticErrorException("Invalid query");
		}
	}
	else {
		if (token.type == QueryToken::QueryTokenType::SUCH_THAT) {
			throw SyntacticErrorException("Invalid query");
		}
		// pattern and such that will have parameter inside brackets
		// with will have parameter as long as the token is not "and", "pattern" or "such that"
		//if (isParameter && (((patternOrSuchThat.type == QueryToken::QueryTokenType::PATTERN ||
		//	patternOrSuchThat.type == QueryToken::QueryTokenType::SUCH_THAT) &&
		//	token.type != QueryToken::QueryTokenType::PARENTHESIS_CLOSE) ||
		//	(patternOrSuchThat.type == QueryToken::QueryTokenType::WITH &&
		//		token.token_value != "and" &&
		//		token.token_value != "with" &&
		//		token.token_value != "pattern" &&
		//		token.type != QueryToken::QueryTokenType::SUCH_THAT))) {
		//	parameterClause.push_back(token);
		//}
		if (isParameter) {
			if (token.type == QueryToken::QueryTokenType::PARENTHESIS_OPEN) {
				parenthesis_counter++;
			}
			if ((((patternOrSuchThat.type == QueryToken::QueryTokenType::PATTERN ||
				patternOrSuchThat.type == QueryToken::QueryTokenType::SUCH_THAT) &&
				(token.type != QueryToken::QueryTokenType::PARENTHESIS_CLOSE || parenthesis_counter > 0)) ||
				(patternOrSuchThat.type == QueryToken::QueryTokenType::WITH &&
					token.token_value != "and" &&
					token.token_value != "with" &&
					token.token_value != "pattern" &&
					token.type != QueryToken::QueryTokenType::SUCH_THAT))) {
				parameterClause.push_back(token);
			}
			if (parenthesis_counter == 0 && token.type == QueryToken::QueryTokenType::PARENTHESIS_CLOSE) {
				if (patternOrSuchThat.type == QueryToken::QueryTokenType::PATTERN) {
					queryValidator.validatePatternType(patternTypeEntity);
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
			if (parenthesis_counter > 0 && token.type == QueryToken::QueryTokenType::PARENTHESIS_CLOSE) {
				parenthesis_counter--;
			}
		}
		else if (patternOrSuchThat.type == QueryToken::QueryTokenType::SUCH_THAT) {
			if (!isParameter && token.type == QueryToken::QueryTokenType::PARENTHESIS_OPEN) {
				isParameter = true;
				setQueryParameter();
			}
			//else if (isParameter && token.type == QueryToken::QueryTokenType::PARENTHESIS_OPEN) {
			//	parenthesis_counter++;
			//}
			//else if (parenthesis_counter == 0 && token.type == QueryToken::QueryTokenType::PARENTHESIS_CLOSE) {
			//	isParameter = false;
			//	validator.parseParameterSuchThat(this->query, queryParameter.type, parameterClause);
			//	parameterClause.clear();
			//	endOfCurrentClauses = true;
			//	parenthesis_counter = 0;
			//}
			//else if (parenthesis_counter > 0 && token.type == QueryToken::QueryTokenType::PARENTHESIS_CLOSE) {
			//	parenthesis_counter--;
			//}
		}
		else if (patternOrSuchThat.type == QueryToken::QueryTokenType::PATTERN) {
			if (prevTokenSelect.token_value == "pattern" && token.type == QueryToken::QueryTokenType::IDENTIFIER
				&& isExpectingPatternType) {
				QueryPreprocessor::addPatternToQuery(token);
				isExpectingPatternType = false;
			}
			else if (!isParameter && token.type == QueryToken::QueryTokenType::PARENTHESIS_OPEN) {
				isParameter = true;
			}
			//else if (isParameter && token.type == QueryToken::QueryTokenType::PARENTHESIS_OPEN) {
			//	parenthesis_counter++;
			//}
			//else if (isParameter && token.type == QueryToken::QueryTokenType::PARENTHESIS_CLOSE) {
			//	queryValidator.validatePatternType(patternTypeEntity);
			//	isParameter = false;
			//	endOfCurrentClauses = true;
			//	validator.parseParameterPattern(this->query, patternTypeEntity, parameterClause);
			//	parameterClause.clear();
			//	parenthesis_counter = 0;
			//}
			//else if (parenthesis_counter > 0 && token.type == QueryToken::QueryTokenType::PARENTHESIS_CLOSE) {
			//	parenthesis_counter--;
			//}
		}
		else if (patternOrSuchThat.type == QueryToken::QueryTokenType::WITH) {
			if (isParameter && (token.token_value == "and" ||
				token.token_value == "pattern" ||
				token.token_value == "with" ||
				token.type == QueryToken::QueryTokenType::SUCH_THAT)) {
				isParameter = false;
				endOfCurrentClauses = true;
				// TODO: call jiyu's method
				// validator.parseWith(query, parameterClause);
				parameterClause.clear();
			}
		}
		else {
			throw SyntacticErrorException("Invalid query");
		}
	}
	prevTokenSelect = token;
}

void QueryPreprocessor::setIdentifierToQueryTokenType(QueryToken& token) {
	if (prevToken.type == QueryToken::QueryTokenType::WHITESPACE ||
		prevToken.type == QueryToken::QueryTokenType::TERMINATOR) {
		if (token.token_value == "stmt") {
			declarationType = { QueryToken::QueryTokenType::STMT, "stmt" };
		}
		else if (token.token_value == "procedure") {
			declarationType = { QueryToken::QueryTokenType::PROCEDURE, "procedure" };
		}
		else if (token.token_value == "read") {
			declarationType = { QueryToken::QueryTokenType::READ, "read" };
		}
		else if (token.token_value == "print") {
			declarationType = { QueryToken::QueryTokenType::PRINT, "print" };
		}
		else if (token.token_value == "call") {
			declarationType = { QueryToken::QueryTokenType::CALL, "call" };
		}
		else if (token.token_value == "if") {
			declarationType = { QueryToken::QueryTokenType::IF, "if" };
		}
		else if (token.token_value == "while") {
			declarationType = { QueryToken::QueryTokenType::WHILE, "while" };
		}
		else if (token.token_value == "assign") {
			declarationType = { QueryToken::QueryTokenType::ASSIGN, "assign" };
		}
		else if (token.token_value == "variable") {
			declarationType = { QueryToken::QueryTokenType::VARIABLE, "variable" };
		}
		else if (token.token_value == "constant") {
			declarationType = { QueryToken::QueryTokenType::CONSTANT, "constant" };
		}
		else if (token.token_value == "prog_line") {
			declarationType = { QueryToken::QueryTokenType::PROG_LINE, "prog_line" };
		}
		// Need to enforce that Select must only come after a terminator
		else if (prevToken.type == QueryToken::QueryTokenType::TERMINATOR && token.token_value == "Select") {
			/*temp = { QueryToken::QueryTokenType::SELECT, "Select" };*/
			isSelect = true;
			status = ParseStatus::IS_SELECTING;
		}
		else {
			//throw std::runtime_error("Invalid syntax for declaration or select");
			throw SyntacticErrorException("Invalid syntax for declaration or select");
		}
	}
}

void QueryPreprocessor::addEntityToQuery(QueryToken& token) {
	// Check if entity name is already used, exists in output, should return error
	Entity ent;
	for (QueryToken each : this->output) {
		if (token.token_value == each.token_value) {
			throw SemanticErrorException("Name is already used!");
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
	bool isValid = false;
	for (QueryToken each : output) {
		if (token.token_value == each.token_value) {
			Synonym synonym;
			synonym.name = token.token_value;
			EntityType entityType = Utility::queryTokenTypeToEntityType(each.type);
			patternTypeEntity = { entityType, synonym };
			isValid = true;
		}
	}
	if (!isValid) {
		throw SemanticErrorException("Pattern type has not been declared");
	}
}

void QueryPreprocessor::addSelectedToQuery(QueryToken& token) {
	Entity ent;
	bool isValid = false;
	if (token.token_value == "BOOLEAN" && this->query.getSelected().size() == 0) {
		ent = { EntityType::BOOLEAN };
		isValid = true;
	}
	else {
		for (QueryToken each : this->output) {
			if (token.token_value == each.token_value) {
				selected.push_back({ each.type, token.token_value });
				Synonym synonym;
				synonym.name = token.token_value;
				EntityType entityType = Utility::queryTokenTypeToEntityType(each.type);
				ent = { entityType, synonym };
				isValid = true;
			}
		}
	}

	if (!isValid) {
		throw SemanticErrorException("Select variable content has not been declared");
	}
	this->query.addSelected(ent);
}

void QueryPreprocessor::setQueryParameter() {
	if (nextToken.type != QueryToken::QueryTokenType::PARENTHESIS_OPEN) {
		throw SyntacticErrorException("After relref keyword must be an open bracket");
	}

	// USES_P to be handled in PatternRelRefValidator
	if (prevTokenSelect.token_value == "Uses" && prevTokenSelect.type == QueryToken::QueryTokenType::IDENTIFIER) {
		queryParameter = { QueryToken::QueryTokenType::USES_S, "Uses" };
	}
	// MODIFIES_P to be handled in PatternRelRefValidator
	else if (prevTokenSelect.token_value == "Modifies" && prevTokenSelect.type == QueryToken::QueryTokenType::IDENTIFIER) {
		queryParameter = { QueryToken::QueryTokenType::MODIFIES_S, "Modifies" };
	}
	else if (prevTokenSelect.token_value == "Parent" && prevTokenSelect.type == QueryToken::QueryTokenType::IDENTIFIER) {
		queryParameter = { QueryToken::QueryTokenType::PARENT, "Parent" };
	}
	else if (prevTokenSelect.token_value == "" && prevTokenSelect.type == QueryToken::QueryTokenType::PARENT_T) {
		queryParameter = { QueryToken::QueryTokenType::PARENT_T, "" };
	}
	else if (prevTokenSelect.token_value == "Follows" && prevTokenSelect.type == QueryToken::QueryTokenType::IDENTIFIER) {
		queryParameter = { QueryToken::QueryTokenType::FOLLOWS, "Follows" };
	}
	else if (prevTokenSelect.token_value == "" && prevTokenSelect.type == QueryToken::QueryTokenType::FOLLOWS_T) {
		queryParameter = { QueryToken::QueryTokenType::FOLLOWS_T, "" };
	}
	else if (prevTokenSelect.token_value == "Calls" && prevTokenSelect.type == QueryToken::QueryTokenType::IDENTIFIER) {
		queryParameter = { QueryToken::QueryTokenType::CALLS, "Calls" };
	}
	else if (prevTokenSelect.token_value == "" && prevTokenSelect.type == QueryToken::QueryTokenType::CALLS_T) {
		queryParameter = { QueryToken::QueryTokenType::CALLS_T, "" };
	}
	else if (prevTokenSelect.token_value == "Next" && prevTokenSelect.type == QueryToken::QueryTokenType::IDENTIFIER) {
		queryParameter = { QueryToken::QueryTokenType::NEXT, "Next" };
	}
	else if (prevTokenSelect.token_value == "" && prevTokenSelect.type == QueryToken::QueryTokenType::NEXT_T) {
		queryParameter = { QueryToken::QueryTokenType::NEXT_T, "" };
	}
	else if (prevTokenSelect.token_value == "Affects" && prevTokenSelect.type == QueryToken::QueryTokenType::IDENTIFIER) {
		queryParameter = { QueryToken::QueryTokenType::AFFECTS, "Affects" };
	}
	else if (prevTokenSelect.token_value == "" && prevTokenSelect.type == QueryToken::QueryTokenType::AFFECTS_T) {
		queryParameter = { QueryToken::QueryTokenType::AFFECTS_T, "" };
	}
}

Query QueryPreprocessor::returnAndResetQuery() {
	Query query_result = this->query;

	// Reset variables
	QueryPreprocessor::resetQuery();

	return query_result;
}

void QueryPreprocessor::resetQuery() {
	// Reset variables
	this->query = Query();
	this->isSelect = false;
	this->isParameter = false;
	this->isExpectingPatternType = false;
	this->isExpectingAttribute = false;
	this->endOfCurrentClauses = true;
	output.clear();
	selected.clear();
	parameterClause.clear();
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