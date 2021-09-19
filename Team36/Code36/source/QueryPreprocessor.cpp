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

QueryPreprocessor::QueryPreprocessor() {
}

Query QueryPreprocessor::parse(std::string str) {
	Query query = Query();
	QueryTokenizer query_tokenizer;
	PatternRelRefValidator validator;

	query_tokenizer.parse_into_query_tokens(str);

	std::vector<QueryToken> v = query_tokenizer.get_query_token_chain();

	// Collection of declared query tokens
	std::vector<QueryToken> output;

	// Collection of selected query tokens
	std::vector<QueryToken> selected;

	// To keep track of parameter content in such that and pattern clauses
	std::vector<QueryToken> parameterClause;

	// To keep track of previous token during declaration, includes COMMA and SEMICOLON
	QueryToken prevToken = QueryToken();

	// To keep track of previous important declaration token, excludes COMMA and SEMICOLON
	QueryToken temp = QueryToken();

	// To keep track of previous token during Selection
	QueryToken prevTokenSelect = QueryToken();

	// To keep track of valid such that clause type, eg: Uses, Modifies, Parent, ParentT
	QueryToken queryParameter = QueryToken();

	// To keep track if selection is in pattern or such that
	QueryToken patternOrSuchThat = QueryToken();

	// To keep track of pattern type entity, eg: ASSIGN
	Entity patternTypeEntity;

	// True when parsing is in selection, false when parsing is in declaration
	bool isSelect = false;

	// True when iterating inside the such that or pattern parameter, false otherwise
	bool isParameter = false;

	bool isExpectingPatternType = false;

	for (QueryToken token : v) {
		// First iteration, set identifier to correct type
		// Check what is my previous token
		if (!isSelect) {
			// temp holds the casted version (can be non-identifier)
			temp = setIdentifierToQueryTokenType(prevToken, temp, token);
			validateDeclarationQuery(prevToken, token);

			Entity ent;
			if (prevToken.type != QueryToken::QueryTokenType::WHITESPACE &&
				prevToken.type != QueryToken::QueryTokenType::TERMINATOR &&
				token.type == QueryToken::QueryTokenType::IDENTIFIER) {
				if (temp.type == QueryToken::QueryTokenType::SELECT) {
					addSelectedToQuery(query, ent, output, selected, token, isSelect);
				}
				else {
					addEntityToQuery(query, ent, output, temp, token);
				}
			}
			prevToken = token;
		}

		else if (isSelect) {
			if (token.type == QueryToken::QueryTokenType::SUCH_THAT) {
				patternOrSuchThat = { QueryToken::QueryTokenType::SUCH_THAT, "" };
			}
			// pattern only valid when previous token is either identifier or )
			else if (!isExpectingPatternType && token.type == QueryToken::QueryTokenType::IDENTIFIER &&
				token.token_value == "pattern") {
				isExpectingPatternType = true;
				patternOrSuchThat = { QueryToken::QueryTokenType::PATTERN, "pattern" };
			}

			else if (isParameter && token.type != QueryToken::QueryTokenType::PARENTHESIS_CLOSE) {
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
				}
			}
			else if (patternOrSuchThat.type == QueryToken::QueryTokenType::PATTERN) {
				if (prevTokenSelect.token_value == "pattern" && token.type == QueryToken::QueryTokenType::IDENTIFIER) {
					QueryPreprocessor::addPatternToQuery(patternTypeEntity, output, token);
				}
				else if (!isParameter && token.type == QueryToken::QueryTokenType::PARENTHESIS_OPEN) {
					isParameter = true;
				}
				else if (isParameter && token.type == QueryToken::QueryTokenType::PARENTHESIS_CLOSE) {
					isExpectingPatternType = false;
					isParameter = false;
					if (patternTypeEntity.getType() == EntityType::ASSIGN) {
						validator.parseParameterPattern(query, patternTypeEntity, parameterClause);
						parameterClause.clear();
					}
					else {
						throw std::runtime_error("Invalid pattern type");
					}
				}
			}
			else {
				throw std::runtime_error("Invalid query");
			}
			prevTokenSelect = token;
		}
	}
	QueryPreprocessor::validateQuery(query);
	return query;
}

QueryToken QueryPreprocessor::setIdentifierToQueryTokenType(QueryToken& prevToken, QueryToken& temp, QueryToken& token) {
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
		// Need to enforce that Select must only come after a terminator
		else if (prevToken.type == QueryToken::QueryTokenType::TERMINATOR && token.token_value == "Select") {
			temp = { QueryToken::QueryTokenType::SELECT, "Select" };
		}
		else {
			throw std::runtime_error("Invalid syntax for declaration or select");
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
		throw std::runtime_error("During declaration, only identifier can exists excluding terminator and whitespace");
	}
	if (prevToken.type == QueryToken::QueryTokenType::COMMA &&
		token.type != QueryToken::QueryTokenType::IDENTIFIER) {
		throw std::runtime_error("During declaration, only identifier is accepted after comma.");
	}
}

void QueryPreprocessor::addEntityToQuery(Query& query, Entity& ent, std::vector<QueryToken>& output, QueryToken& temp, QueryToken& token) {
	// Check if entity name is already used, exists in output, should return error
	for (QueryToken each : output) {
		if (token.token_value == each.token_value) {
			throw std::runtime_error("Name is already used!");
		}
	}

	// Declaring goes into output
	output.push_back({ temp.type, { token.token_value } });
	Synonym synonym;
	synonym.name = token.token_value;
	EntityType entityType = Utility::queryTokenTypeToEntityType(temp.type);
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
		throw std::runtime_error("Pattern type has not been declared");
	}
}

void QueryPreprocessor::addSelectedToQuery(Query& query, Entity& ent, std::vector<QueryToken>& output, std::vector<QueryToken> selected, QueryToken& token, bool& isSelect) {
	bool isValid = false;
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
	if (!isValid) {
		throw std::runtime_error("Select variable content has not been declared");
	}
	isSelect = true;
	query.addSelected(ent);
}

void QueryPreprocessor::setQueryParameter(QueryToken& prevTokenSelect, QueryToken& queryParameter) {
	if (prevTokenSelect.token_value == "Uses" && prevTokenSelect.type == QueryToken::QueryTokenType::IDENTIFIER) {
		queryParameter = { QueryToken::QueryTokenType::USES_S, "Uses" };
	}
	else if (prevTokenSelect.token_value == "Modifies" && prevTokenSelect.type == QueryToken::QueryTokenType::IDENTIFIER) {
		queryParameter = { QueryToken::QueryTokenType::MODIFIES_S, "Modifies" };
	}
	else if (prevTokenSelect.token_value == "Parent" && prevTokenSelect.type == QueryToken::QueryTokenType::IDENTIFIER) {
		queryParameter = { QueryToken::QueryTokenType::PARENT, "Parent" };
	}
	else if (prevTokenSelect.token_value == "Follows" && prevTokenSelect.type == QueryToken::QueryTokenType::IDENTIFIER) {
		queryParameter = { QueryToken::QueryTokenType::FOLLOWS, "Follows" };
	}
	else if (prevTokenSelect.token_value == "" && prevTokenSelect.type == QueryToken::QueryTokenType::PARENT_T) {
		queryParameter = { QueryToken::QueryTokenType::PARENT_T, "" };
	}
	else if (prevTokenSelect.token_value == "" && prevTokenSelect.type == QueryToken::QueryTokenType::FOLLOWS_T) {
		queryParameter = { QueryToken::QueryTokenType::FOLLOWS_T, "" };
	}
}

void QueryPreprocessor::validateQuery(Query& query) {
	if (query.getEntities().size() == 0) {
		throw std::runtime_error("No declaration has been made in your query");
	}
	// TODO: only size 1 is allowed for iteration 1, would allow multiple selects in future iterations
	if (query.getSelected().size() != 1) {
		throw std::runtime_error("There is no selected variable in your query");
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
			ent.second.getType() != EntityType::ASSIGN) {
			throw std::runtime_error("Declaration fails!");
		}
	}
}