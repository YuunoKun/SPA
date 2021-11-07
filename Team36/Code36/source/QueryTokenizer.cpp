#include <string>
#include <vector>
#include <stack>
#include <stdexcept>
#include "QueryTokenizer.h"

QueryTokenizer::QueryTokenizer() {
	query_token_cache = {};
}

void QueryTokenizer::tokenize(Query& query, std::string input) {
	QueryToken curr_query_token;
	QueryToken temp_query_token;

	std::stack<char> parenthesis_validation_stk;
	std::stack<char> tuple_validation_stk;

	bool quotation_validation = false;
	bool is_prog_line = false;

	for (char c : input) {
		if (curr_query_token.type == QueryToken::IDENTIFIER
			&& curr_query_token.token_value == SUCH_SPACE_STR
			&& c != T_C) {
			curr_query_token.token_value.pop_back();
			addQueryToken(query, curr_query_token);
			curr_query_token.type = QueryToken::WHITESPACE;
		} else if (curr_query_token.type == QueryToken::IDENTIFIER
			&& curr_query_token.token_value == PROG_UNDERSCORE_STR
			&& c != L_C) {
			throw SyntacticErrorException("Unexpected symbol \"_\"");
		}

		switch (c) {
		case LEFT_PARENTHESIS_C:
			handleChar(query, c, curr_query_token);
			parenthesis_validation_stk.push(LEFT_PARENTHESIS_C);
			break;
		case RIGHT_PARENTHESIS_C:
			if (!parenthesis_validation_stk.empty() && parenthesis_validation_stk.top() == LEFT_PARENTHESIS_C) {
				parenthesis_validation_stk.pop();
			} else {
				throw SyntacticErrorException("Unexpected symbol : \')\'");
			}
			handleChar(query, c, curr_query_token);
			break;
		case TUPLE_OPEN_C:
			handleChar(query, c, curr_query_token);
			parenthesis_validation_stk.push(TUPLE_OPEN_C);
			break;
		case TUPLE_CLOSE_C:
			if (!parenthesis_validation_stk.empty() && parenthesis_validation_stk.top() == TUPLE_OPEN_C) {
				parenthesis_validation_stk.pop();
			} else {
				throw SyntacticErrorException("Unexpected symbol : \'>\'");
			}
			handleChar(query, c, curr_query_token);
			break;
		case QUOTATION_C:
			handleQuotation(query, quotation_validation, curr_query_token);
			break;
		case UNDERSCORE_C:
			if (curr_query_token.token_value == PROG_STR) {
				curr_query_token.token_value.push_back(c);
				is_prog_line = true;
			} else {
				handleChar(query, c, curr_query_token);
			}
			break;
		case SPACE_C:
			handleSpace(query, is_prog_line, curr_query_token);
			break;
		case NEWLINE_C:
		case TABS_C:
			addQueryToken(query, curr_query_token);
			break;
		case ASTERISK_C:
			// must NOT be in quotation brackets
			if (!quotation_validation && curr_query_token.type == QueryToken::IDENTIFIER) {
				handleRelrefStr(query, curr_query_token);
				break;
			} else {
				handleChar(query, c, curr_query_token);
				break;
			}
		case SEMICOLON_C:
		case COMMA_C:
		case PLUS_SIGN_C:
		case MINUS_SIGN_C:
		case SLASH_C:
		case PERCENT_SIGN_C:
		case EQUAL_SIGN_C:
			handleChar(query, c, curr_query_token);
			break;
		case DOT_C:
			addQueryToken(query, curr_query_token);
			curr_query_token.token_value.push_back(c);
			break;
		case HASH_C:
			if (curr_query_token.token_value == DOT_STMT_STR) {
				curr_query_token.token_value.push_back(c);
			} else {
				throw SyntacticErrorException("Unexpected symbol : \'#\'");
			}
			break;
		default:
			if (isdigit(c)) {
				handleDigit(query, c, curr_query_token);
			} else if (isalpha(c)) {
				handleAlpha(query, c, curr_query_token);
			} else {
				throw SyntacticErrorException("Unknown symbol present");
			}
			break;
		}
	}
	addQueryToken(query, curr_query_token);

	// Check for open brackets, open quotation marks, temp token active
	if (!parenthesis_validation_stk.empty()) {
		throw SyntacticErrorException("expected \')\'");
	} else if (quotation_validation) {
		throw SyntacticErrorException("missing terminating \" character");
	} else if (is_prog_line) {
		throw SyntacticErrorException("invalid _");
	}
}

std::vector<QueryToken> QueryTokenizer::get_query_token_chain() {
	return query_token_cache;
}

void QueryTokenizer::handleDigit(Query& query, char c, QueryToken& curr_query_token) {
	if (curr_query_token.type == QueryToken::IDENTIFIER) {
		curr_query_token.token_value.push_back(c);
	} else if (curr_query_token.type == QueryToken::CONSTANT || curr_query_token.type == QueryToken::WHITESPACE) {
		curr_query_token.type = QueryToken::CONSTANT;
		curr_query_token.token_value.push_back(c);
	}
}

void QueryTokenizer::handleAlpha(Query& query, char c, QueryToken& curr_query_token) {
	if (curr_query_token.type == QueryToken::IDENTIFIER || curr_query_token.type == QueryToken::WHITESPACE) {
		curr_query_token.type = QueryToken::IDENTIFIER;
		curr_query_token.token_value.push_back(c);
	} else {
		throw SyntacticErrorException("Invalid Name present");
	}
}

void QueryTokenizer::handleChar(Query& query, char c, QueryToken& curr_query_token) {
	addQueryToken(query, curr_query_token);

	switch (c) {
	case LEFT_PARENTHESIS_C:
		curr_query_token.type = QueryToken::PARENTHESIS_OPEN;
		break;
	case RIGHT_PARENTHESIS_C:
		curr_query_token.type = QueryToken::PARENTHESIS_CLOSE;
		break;
	case TUPLE_OPEN_C:
		curr_query_token.type = QueryToken::TUPLE_OPEN;
		break;
	case TUPLE_CLOSE_C:
		curr_query_token.type = QueryToken::TUPLE_CLOSE;
		break;
	case SEMICOLON_C:
		curr_query_token.type = QueryToken::TERMINATOR;
		break;
	case UNDERSCORE_C:
		curr_query_token.type = QueryToken::WILDCARD;
		break;
	case COMMA_C:
		curr_query_token.type = QueryToken::COMMA;
		break;
	case PLUS_SIGN_C:
		curr_query_token.type = QueryToken::PLUS;
		break;
	case MINUS_SIGN_C:
		curr_query_token.type = QueryToken::MINUS;
		break;
	case SLASH_C:
		curr_query_token.type = QueryToken::DIV;
		break;
	case ASTERISK_C:
		curr_query_token.type = QueryToken::MUL;
		break;
	case PERCENT_SIGN_C:
		curr_query_token.type = QueryToken::MOD;
		break;
	case EQUAL_SIGN_C:
		curr_query_token.type = QueryToken::EQUAL;
		break;
	}
	addQueryToken(query, curr_query_token);
}

void QueryTokenizer::handleSpace(Query& query, bool& is_prog_line, QueryToken& curr_query_token) {
	// check for "such" in such that
	if (curr_query_token.type == QueryToken::IDENTIFIER
		&& curr_query_token.token_value == SUCH_STR) {
		curr_query_token.token_value.push_back(SPACE_C);
		return;
	} else if (curr_query_token.type == QueryToken::IDENTIFIER
		&& curr_query_token.token_value == SUCH_SPACE_THAT_STR) {
		// check for "such that" in such that
		curr_query_token.type = QueryToken::SUCH_THAT;
		curr_query_token.token_value = EMPTY_S;
	} else if (curr_query_token.type == QueryToken::IDENTIFIER
		&& curr_query_token.token_value == PROG_UNDERSCORE_LINE_STR) {
		curr_query_token.type = QueryToken::PROG_LINE;
		curr_query_token.token_value = EMPTY_S;
		is_prog_line = false;
	}
	addQueryToken(query, curr_query_token);
}

void QueryTokenizer::handleQuotation(Query& query, bool& quotation_validation, QueryToken& curr_query_token) {
	if (!quotation_validation) {
		//No opening quotation marks
		addQueryToken(query, curr_query_token);
		curr_query_token.type = QueryToken::QUOTATION_OPEN;
		addQueryToken(query, curr_query_token);
		quotation_validation = true;
	} else {
		addQueryToken(query, curr_query_token);
		curr_query_token.type = QueryToken::QUOTATION_CLOSE;
		addQueryToken(query, curr_query_token);
		quotation_validation = false;
	}
}

void QueryTokenizer::handleRelrefStr(Query& query, QueryToken& curr_query_token) {
	if (curr_query_token.token_value == FOLLOWS_STR) {
		curr_query_token.type = QueryToken::FOLLOWS_T;
	} else if (curr_query_token.token_value == PARENT_STR) {
		curr_query_token.type = QueryToken::PARENT_T;
	} else if (curr_query_token.token_value == CALLS_STR) {
		curr_query_token.type = QueryToken::CALLS_T;
	} else if (curr_query_token.token_value == NEXT_STR) {
		curr_query_token.type = QueryToken::NEXT_T;
	} else if (curr_query_token.token_value == NEXT_BIP_STR) {
		curr_query_token.type = QueryToken::NEXT_BIP_T;
	} else if (curr_query_token.token_value == AFFECTS_STR) {
		curr_query_token.type = QueryToken::AFFECTS_T;
	} else if (curr_query_token.token_value == AFFECTS_BIP_STR) {
		curr_query_token.type = QueryToken::AFFECTS_BIP_T;
	}
	curr_query_token.token_value = EMPTY_S;
	addQueryToken(query, curr_query_token);
}

void QueryTokenizer::addQueryToken(Query& query, QueryToken& query_token) {
	if (query_token.type == QueryToken::WHITESPACE) {
		//discard whitespace
		return;
	}

	if (query_token.type == QueryToken::IDENTIFIER) {
		if (query_token.token_value == DOT_PROCNAME_STR) {
			query_token.type = QueryToken::DOT;
			query_token.token_value = EMPTY_S;
			query_token_cache.push_back(query_token);
			query_token.type = QueryToken::PROC_NAME;
			query_token.token_value = EMPTY_S;
		} else if (query_token.token_value == DOT_VARNAME_STR) {
			query_token.type = QueryToken::DOT;
			query_token.token_value = EMPTY_S;
			query_token_cache.push_back(query_token);
			query_token.type = QueryToken::VAR_NAME;
			query_token.token_value = EMPTY_S;
		} else if (query_token.token_value == DOT_VALIUE_STR) {
			query_token.type = QueryToken::DOT;
			query_token.token_value = EMPTY_S;
			query_token_cache.push_back(query_token);
			query_token.type = QueryToken::VALUE;
			query_token.token_value = EMPTY_S;
		} else if (query_token.token_value == DOT_STMT_HASH_STR) {
			query_token.type = QueryToken::DOT;
			query_token.token_value = EMPTY_S;
			query_token_cache.push_back(query_token);
			query_token.type = QueryToken::STMT_INDEX;
			query_token.token_value = EMPTY_S;
		}
	} else if (query_token.type == QueryToken::CONSTANT) {
		if (query_token.token_value[0] == ZERO_C && query_token.token_value.length() > 1) {
			query.setIsSemanticError("No leading '0's allowed ");
		}
	}

	query_token_cache.push_back(query_token);
	query_token.type = QueryToken::QueryTokenType::WHITESPACE;
	query_token.token_value = EMPTY_S;
}