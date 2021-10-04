#include "QueryPreprocessor.h"
#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include "Entity.h"
#include "QueryToken.h"
#include "QueryTokenizer.h"
#include "Query.h"
#include "PatternRelRefValidator.h"
#include "Utility.h"
#include "SemanticErrorException.h"
#include "SyntacticErrorException.h"

QueryPreprocessor::QueryPreprocessor() {
}

Query QueryPreprocessor::parse(std::string str) {
	Query query = Query();
	QueryTokenizer query_tokenizer;
	PatternRelRefValidator validator;

	query_tokenizer.parse_into_query_tokens(str);

	std::vector<QueryToken> v = query_tokenizer.get_query_token_chain();

	//std::vector<QueryToken> v;
	//v.push_back({ QueryToken::QueryTokenType::IDENTIFIER, "assign" });
	//v.push_back({ QueryToken::QueryTokenType::IDENTIFIER, "a1" });
	//v.push_back({ QueryToken::QueryTokenType::COMMA, "" });
	//v.push_back({ QueryToken::QueryTokenType::IDENTIFIER, "assign" });
	//v.push_back({ QueryToken::QueryTokenType::IDENTIFIER, "a2" });
	//v.push_back({ QueryToken::QueryTokenType::TERMINATOR, "" });
	//v.push_back({ QueryToken::QueryTokenType::IDENTIFIER, "Select" });
	//v.push_back({ QueryToken::QueryTokenType::TUPLE_OPEN, "" });
	//v.push_back({ QueryToken::QueryTokenType::IDENTIFIER, "a1" });
	//v.push_back({ QueryToken::QueryTokenType::COMMA, "" });
	//v.push_back({ QueryToken::QueryTokenType::IDENTIFIER, "a2" });
	//v.push_back({ QueryToken::QueryTokenType::TUPLE_CLOSE, "" });

	//v.push_back({ QueryToken::QueryTokenType::DOT, "" });
	//v.push_back({ QueryToken::QueryTokenType::VAR_NAME, "" });

	//v.push_back({ QueryToken::QueryTokenType::IDENTIFIER, "stmt" });
	//v.push_back({ QueryToken::QueryTokenType::IDENTIFIER, "s" });
	//v.push_back({ QueryToken::QueryTokenType::TERMINATOR, "" });
	//v.push_back({ QueryToken::QueryTokenType::IDENTIFIER, "constant" });
	//v.push_back({ QueryToken::QueryTokenType::IDENTIFIER, "c" });
	//v.push_back({ QueryToken::QueryTokenType::TERMINATOR, "" });
	//v.push_back({ QueryToken::QueryTokenType::IDENTIFIER, "Select" });
	//v.push_back({ QueryToken::QueryTokenType::IDENTIFIER, "s" });
	//v.push_back({ QueryToken::QueryTokenType::IDENTIFIER, "with" });
	//v.push_back({ QueryToken::QueryTokenType::IDENTIFIER, "s" });
	//v.push_back({ QueryToken::QueryTokenType::DOT, "" });
	//v.push_back({ QueryToken::QueryTokenType::STMT_INDEX, "" });
	//v.push_back({ QueryToken::QueryTokenType::EQUAL, "" });
	//v.push_back({ QueryToken::QueryTokenType::IDENTIFIER, "c" });
	//v.push_back({ QueryToken::QueryTokenType::DOT, "" });
	//v.push_back({ QueryToken::QueryTokenType::VALUE, "" });
	//v.push_back({ QueryToken::QueryTokenType::IDENTIFIER, "and" });
	//v.push_back({ QueryToken::QueryTokenType::IDENTIFIER, "s" });
	//v.push_back({ QueryToken::QueryTokenType::EQUAL, "" });
	//v.push_back({ QueryToken::QueryTokenType::CONSTANT, "10" });

	//v.push_back({ QueryToken::QueryTokenType::IDENTIFIER, "assign" });
	//v.push_back({ QueryToken::QueryTokenType::IDENTIFIER, "a" });
	//v.push_back({ QueryToken::QueryTokenType::TERMINATOR, "" });
	//v.push_back({ QueryToken::QueryTokenType::IDENTIFIER, "Select" });
	//v.push_back({ QueryToken::QueryTokenType::IDENTIFIER, "a" });
	//v.push_back({ QueryToken::QueryTokenType::SUCH_THAT, "" });
	//v.push_back({ QueryToken::QueryTokenType::AFFECTS_T, "" });
	//v.push_back({ QueryToken::QueryTokenType::PARENTHESIS_OPEN, "" });
	//v.push_back({ QueryToken::QueryTokenType::IDENTIFIER, "a" });
	//v.push_back({ QueryToken::QueryTokenType::COMMA, "" });
	//v.push_back({ QueryToken::QueryTokenType::CONSTANT, "10" });
	//v.push_back({ QueryToken::QueryTokenType::PARENTHESIS_CLOSE, "" });

	// Collection of declared query tokens
	std::vector<QueryToken> output;

	// Collection of selected query tokens
	std::vector<QueryToken> selected;

	// To keep track of parameter content in such that and pattern clauses
	std::vector<QueryToken> parameterClause;

	// To keep track of previous token during declaration, includes COMMA and SEMICOLON
	QueryToken prevToken = QueryToken();

	// To keep track of previous important declaration token, excludes COMMA and SEMICOLON
	QueryToken declarationType = QueryToken();

	// To keep track if there another declaration
	bool haveNextDeclaration = false;

	// Keep track if it the end of current declaration by terminator
	bool endOfCurrentDeclaration = true;

	// To keep track of previous token during Selection
	QueryToken prevTokenSelect = QueryToken();

	// To keep track of valid such that clause type, eg: Uses, Modifies, Parent, ParentT
	QueryToken queryParameter = QueryToken();

	// To keep track if selection is in pattern or such that
	QueryToken patternOrSuchThat = QueryToken();

	// To keep track of pattern type entity, eg: ASSIGN
	Entity patternTypeEntity;

	ParseStatus status;

	// True when parsing is in selection, false when parsing is in declaration
	bool isSelect = false;

	// True when iterating inside the such that or pattern parameter, false otherwise
	bool isParameter = false;

	bool isExpectingPatternType = false;

	bool isExpectingAttribute = false;

	bool endOfCurrentClauses = true;

	for (QueryToken token : v) {
		// First iteration, set identifier to correct type
		// Check what is my previous token
		if (!isSelect) {
			if (endOfCurrentDeclaration) {
				declarationType = setIdentifierToQueryTokenType(prevToken, declarationType, token, status, isSelect);
				validateDeclarationQuery(prevToken, token);
				haveNextDeclaration = true;
				endOfCurrentDeclaration = false;
			}
			else if (haveNextDeclaration) {
				Entity ent;
				if (token.type == QueryToken::QueryTokenType::IDENTIFIER) {
					//if (declarationType.type == QueryToken::QueryTokenType::SELECT) {
					//if (status == ParseStatus::IS_SELECTING) {
					//	addSelectedToQuery(query, ent, output, selected, token, isSelect);
					//	haveNextDeclaration = false;
					//}
					//else {
					addEntityToQuery(query, ent, output, declarationType, token);
					haveNextDeclaration = false;
					//}
				}
				else {
					throw SyntacticErrorException("Invalid declaration");
				}
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

		else if (isSelect) {
			if (endOfCurrentClauses) {
				if (token.type == QueryToken::QueryTokenType::SUCH_THAT) {
					patternOrSuchThat = { QueryToken::QueryTokenType::SUCH_THAT, "" };
					endOfCurrentClauses = false;
				}
				// pattern only valid when previous token is either identifier or )
				else if (token.type == QueryToken::QueryTokenType::IDENTIFIER &&
					token.token_value == "pattern") {
					isExpectingPatternType = true;
					patternOrSuchThat = { QueryToken::QueryTokenType::PATTERN, "pattern" };
					endOfCurrentClauses = false;
				}
				else if (token.type == QueryToken::QueryTokenType::IDENTIFIER &&
					token.token_value == "with") {
					patternOrSuchThat = { QueryToken::QueryTokenType::WITH, "" };
					isParameter = true;
					endOfCurrentClauses = false;
				}
				else if (token.type == QueryToken::QueryTokenType::IDENTIFIER &&
					token.token_value == "and") {
					if (patternOrSuchThat.type != QueryToken::QueryTokenType::PATTERN &&
						patternOrSuchThat.type != QueryToken::QueryTokenType::SUCH_THAT &&
						patternOrSuchThat.type != QueryToken::QueryTokenType::WITH) {
						throw SyntacticErrorException("The keyword 'and' should come after pattern/ relations have been initalized previously");
					}
					if (patternOrSuchThat.type == QueryToken::QueryTokenType::PATTERN) {
						isExpectingPatternType = true;
					}
					else if (patternOrSuchThat.type == QueryToken::QueryTokenType::WITH) {
						isParameter = true;
					}
					endOfCurrentClauses = false;
				}
				// Select content
				//else if (declarationType.type == QueryToken::QueryTokenType::SELECT) {
				else if (status == ParseStatus::IS_SELECTING) {
					// if Select has attribute ie. Select p.procName
					if (token.type == QueryToken::QueryTokenType::DOT &&
						prevToken.type == QueryToken::QueryTokenType::IDENTIFIER) {
						isExpectingAttribute = true;
					}
					else if (isExpectingAttribute && prevTokenSelect.type == QueryToken::QueryTokenType::DOT) {
						AttrRef attrRef = Utility::queryTokenTypeToAttrRef(token.type);
						query.setSelectedAttribute(attrRef);
						isExpectingAttribute = false;
					}
					else if (prevToken.token_value == "Select" && token.type == QueryToken::QueryTokenType::TUPLE_OPEN) {
						status = ParseStatus::IS_SELECTING_MULTIPLE_CLAUSE;
					}
					else if (token.type == QueryToken::QueryTokenType::TUPLE_CLOSE) {
						status = ParseStatus::NEUTRAL;
					}
					else if (status != ParseStatus::IS_SELECTING_MULTIPLE_CLAUSE &&
						prevToken.token_value == "Select" &&
						token.type == QueryToken::QueryTokenType::IDENTIFIER) {
						Entity ent;
						addSelectedToQuery(query, ent, output, selected, token, isSelect);
						haveNextDeclaration = false;
					}
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
				else if (isParameter && (((patternOrSuchThat.type == QueryToken::QueryTokenType::PATTERN ||
					patternOrSuchThat.type == QueryToken::QueryTokenType::SUCH_THAT) &&
					token.type != QueryToken::QueryTokenType::PARENTHESIS_CLOSE) ||
					(patternOrSuchThat.type == QueryToken::QueryTokenType::WITH &&
						token.token_value != "and" &&
						token.token_value != "with" &&
						token.token_value != "pattern" &&
						token.type != QueryToken::QueryTokenType::SUCH_THAT))) {
					parameterClause.push_back(token);
				}
				else if (patternOrSuchThat.type == QueryToken::QueryTokenType::SUCH_THAT) {
					if (!isParameter && token.type == QueryToken::QueryTokenType::PARENTHESIS_OPEN) {
						isParameter = true;
						setQueryParameter(prevTokenSelect, queryParameter);
					}
					else if (token.type == QueryToken::QueryTokenType::PARENTHESIS_CLOSE) {
						isParameter = false;
						validator.parseParameterSuchThat(query, queryParameter.type, parameterClause);
						parameterClause.clear();
						endOfCurrentClauses = true;
					}
				}
				else if (patternOrSuchThat.type == QueryToken::QueryTokenType::PATTERN) {
					if (prevTokenSelect.token_value == "pattern" && token.type == QueryToken::QueryTokenType::IDENTIFIER
						&& isExpectingPatternType) {
						QueryPreprocessor::addPatternToQuery(patternTypeEntity, output, token);
						isExpectingPatternType = false;
					}
					else if (!isParameter && token.type == QueryToken::QueryTokenType::PARENTHESIS_OPEN) {
						isParameter = true;
					}
					else if (isParameter && token.type == QueryToken::QueryTokenType::PARENTHESIS_CLOSE) {
						isParameter = false;
						endOfCurrentClauses = true;
						if (patternTypeEntity.getType() == EntityType::ASSIGN ||
							patternTypeEntity.getType() == EntityType::WHILE ||
							patternTypeEntity.getType() == EntityType::IF) {
							validator.parseParameterPattern(query, patternTypeEntity, parameterClause);
							parameterClause.clear();
						}
						else {
							throw SemanticErrorException("Invalid pattern type");
						}
					}
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
	}
	if (!endOfCurrentClauses) {
		throw SyntacticErrorException("Invalid query");
	}
	QueryPreprocessor::validateQuery(query);
	return query;
}

QueryToken QueryPreprocessor::setIdentifierToQueryTokenType(QueryToken& prevToken, QueryToken& temp, QueryToken& token, ParseStatus& status, bool& isSelect) {
	if (prevToken.type == QueryToken::QueryTokenType::WHITESPACE ||
		prevToken.type == QueryToken::QueryTokenType::TERMINATOR) {
		if (token.token_value == "stmt") {
			temp = { QueryToken::QueryTokenType::STMT, "stmt" };
		}
		else if (token.token_value == "procedure") {
			temp = { QueryToken::QueryTokenType::PROCEDURE, "procedure" };
		}
		else if (token.token_value == "read") {
			temp = { QueryToken::QueryTokenType::READ, "read" };
		}
		else if (token.token_value == "print") {
			temp = { QueryToken::QueryTokenType::PRINT, "print" };
		}
		else if (token.token_value == "call") {
			temp = { QueryToken::QueryTokenType::CALL, "call" };
		}
		else if (token.token_value == "if") {
			temp = { QueryToken::QueryTokenType::IF, "if" };
		}
		else if (token.token_value == "while") {
			temp = { QueryToken::QueryTokenType::WHILE, "while" };
		}
		else if (token.token_value == "assign") {
			temp = { QueryToken::QueryTokenType::ASSIGN, "assign" };
		}
		else if (token.token_value == "variable") {
			temp = { QueryToken::QueryTokenType::VARIABLE, "variable" };
		}
		else if (token.token_value == "constant") {
			temp = { QueryToken::QueryTokenType::CONSTANT, "constant" };
		}
		else if (token.token_value == "prog_line") {
			temp = { QueryToken::QueryTokenType::PROG_LINE, "prog_line" };
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
	return temp;
}

void QueryPreprocessor::validateDeclarationQuery(QueryToken& prevToken, QueryToken& token) {
	// Guard clauses to catch semantically wrong input, all usage of token should be temp
	if (token.type == QueryToken::QueryTokenType::IDENTIFIER &&
		(prevToken.type != QueryToken::QueryTokenType::IDENTIFIER &&
			prevToken.type != QueryToken::QueryTokenType::TERMINATOR &&
			prevToken.type != QueryToken::QueryTokenType::COMMA &&
			prevToken.type != QueryToken::QueryTokenType::WHITESPACE)) {
		throw SyntacticErrorException("During declaration, only identifier can exists excluding terminator and whitespace");
	}
	if (prevToken.type == QueryToken::QueryTokenType::COMMA &&
		token.type != QueryToken::QueryTokenType::IDENTIFIER) {
		throw SyntacticErrorException("During declaration, only identifier is accepted after comma.");
	}
}

void QueryPreprocessor::addEntityToQuery(Query& query, Entity& ent, std::vector<QueryToken>& output, QueryToken& type, QueryToken& token) {
	// Check if entity name is already used, exists in output, should return error
	for (QueryToken each : output) {
		if (token.token_value == each.token_value) {
			throw SemanticErrorException("Name is already used!");
		}
	}

	// Declaring goes into output
	output.push_back({ type.type, { token.token_value } });
	Synonym synonym;
	synonym.name = token.token_value;
	EntityType entityType = Utility::queryTokenTypeToEntityType(type.type);
	ent = { entityType, synonym };
	query.addEntity(ent);
}

void QueryPreprocessor::addPatternToQuery(Entity& patternTypeEntity, std::vector<QueryToken>& output, QueryToken& token) {
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

void QueryPreprocessor::addSelectedToQuery(Query& query, Entity& ent, std::vector<QueryToken>& output, std::vector<QueryToken> selected, QueryToken& token, bool& isSelect) {
	bool isValid = false;
	if (token.token_value == "BOOLEAN" && query.getSelected().size() == 0) {
		ent = { EntityType::BOOLEAN };
		isValid = true;
	}
	else {
		for (QueryToken each : output) {
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
	//isSelect = true;
	query.addSelected(ent);
}

void QueryPreprocessor::setQueryParameter(QueryToken& prevTokenSelect, QueryToken& queryParameter) {
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

void QueryPreprocessor::validateQuery(Query& query) {
	if (query.getEntities().size() == 0) {
		throw SyntacticErrorException("No declaration has been made in your query");
	}
	if (query.getSelected().size() == 0) {
		throw SyntacticErrorException("There is no selected variable in your query");
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