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
			&& curr_query_token.token_value == "such "
			&& c != 't') {
			curr_query_token.token_value.pop_back();
			add_query_token(query, curr_query_token);
			curr_query_token.type = QueryToken::WHITESPACE;
		} else if (curr_query_token.type == QueryToken::IDENTIFIER
			&& curr_query_token.token_value == "prog_"
			&& c != 'l') {
			throw SyntacticErrorException("Unexpected symbol \"_\"");
		}


		switch (c) {
		case '(':
			add_query_token(query, curr_query_token);
			curr_query_token.type = QueryToken::PARENTHESIS_OPEN;
			add_query_token(query, curr_query_token);
			parenthesis_validation_stk.push('(');
			break;
		case ')':
			if (!parenthesis_validation_stk.empty() && parenthesis_validation_stk.top() == '(') {
				parenthesis_validation_stk.pop();
			}
			else {
				throw SyntacticErrorException("Unexpected symbol : \')\'");

				//throw std::runtime_error( std::string("Unexpected symbol : \')\'"));
			}
			add_query_token(query, curr_query_token);
			curr_query_token.type = QueryToken::PARENTHESIS_CLOSE;
			add_query_token(query, curr_query_token);
			break;
		case '<':
			add_query_token(query, curr_query_token);
			curr_query_token.type = QueryToken::TUPLE_OPEN;
			add_query_token(query, curr_query_token);
			parenthesis_validation_stk.push('<');
			break;
		case '>':
			if (!parenthesis_validation_stk.empty() && parenthesis_validation_stk.top() == '<') {
				parenthesis_validation_stk.pop();
			}
			else {
				throw SyntacticErrorException("Unexpected symbol : \'>\'");
			}
			add_query_token(query, curr_query_token);
			curr_query_token.type = QueryToken::TUPLE_CLOSE;
			add_query_token(query, curr_query_token);
			break;
		case '"':
			if (!quotation_validation) {
				//No opening quotation marks
				add_query_token(query, curr_query_token);
				curr_query_token.type = QueryToken::QUOTATION_OPEN;
				add_query_token(query, curr_query_token);
				quotation_validation = true;
			}
			else {
				add_query_token(query, curr_query_token);
				curr_query_token.type = QueryToken::QUOTATION_CLOSE;
				add_query_token(query, curr_query_token);
				quotation_validation = false;
			}
			break;
		case ';':
			add_query_token(query, curr_query_token);
			curr_query_token.type = QueryToken::TERMINATOR;
			add_query_token(query, curr_query_token);
			break;
		case '_':
			if (curr_query_token.token_value == "prog") {
				curr_query_token.token_value.push_back(c);
				is_prog_line = true;
			} else {
				add_query_token(query, curr_query_token);
				curr_query_token.type = QueryToken::WILDCARD;
				add_query_token(query, curr_query_token);
			}
			break;

		case ',':
			add_query_token(query, curr_query_token);
			curr_query_token.type = QueryToken::COMMA;
			add_query_token(query, curr_query_token);
			break;
		case ' ':
			// check for "such" in such that
			if (curr_query_token.type == QueryToken::IDENTIFIER
				&& curr_query_token.token_value == "such") {
				curr_query_token.token_value.push_back(c);
				break;
			} else if (curr_query_token.type == QueryToken::IDENTIFIER
				&& curr_query_token.token_value == "such that") { 
				// check for "such that" in such that
				curr_query_token.type = QueryToken::SUCH_THAT;
				curr_query_token.token_value = "";
				add_query_token(query, curr_query_token);
				break;
			} else if (curr_query_token.type == QueryToken::IDENTIFIER
				&& curr_query_token.token_value == "prog_line") { 
				curr_query_token.type = QueryToken::PROG_LINE;
				curr_query_token.token_value = "";
				add_query_token(query, curr_query_token);
				is_prog_line = false;
			}
			add_query_token(query, curr_query_token);
			curr_query_token.type = QueryToken::WHITESPACE;
			break;
		case '\n':
			add_query_token(query, curr_query_token);
			curr_query_token.type = QueryToken::WHITESPACE;
			break;
		case '\t':
			add_query_token(query, curr_query_token);
			curr_query_token.type = QueryToken::WHITESPACE;
			break;
		case '*':
			// must NOT be in quotation brackets
			if (!quotation_validation && curr_query_token.type == QueryToken::IDENTIFIER) {
				if (curr_query_token.token_value == "Follows") {
					curr_query_token.type = QueryToken::FOLLOWS_T;
					curr_query_token.token_value = "";
					add_query_token(query, curr_query_token);
				} else if (curr_query_token.token_value == "Parent") {
					curr_query_token.type = QueryToken::PARENT_T;
					curr_query_token.token_value = "";
					add_query_token(query, curr_query_token);
				} else if (curr_query_token.token_value == "Calls") {
					curr_query_token.type = QueryToken::CALLS_T;
					curr_query_token.token_value = "";
					add_query_token(query, curr_query_token);
				} else if (curr_query_token.token_value == "Next") {
					curr_query_token.type = QueryToken::NEXT_T;
					curr_query_token.token_value = "";
					add_query_token(query, curr_query_token);
				} else if (curr_query_token.token_value == "NextBip") {
					curr_query_token.type = QueryToken::NEXT_BIP_T;
					curr_query_token.token_value = "";
					add_query_token(query, curr_query_token);
				} else if (curr_query_token.token_value == "Affects") {
					curr_query_token.type = QueryToken::AFFECTS_T;
					curr_query_token.token_value = "";
					add_query_token(query, curr_query_token);
				} else if (curr_query_token.token_value == "AffectsBip") {
					curr_query_token.type = QueryToken::AFFECTS_BIP_T;
					curr_query_token.token_value = "";
					add_query_token(query, curr_query_token);
				}

				break;
			}
			else {
				add_query_token(query, curr_query_token);
				curr_query_token.type = QueryToken::MUL;
				add_query_token(query, curr_query_token);
				break;
			}
		case '+':
			add_query_token(query, curr_query_token);
			curr_query_token.type = QueryToken::PLUS;
			add_query_token(query, curr_query_token);
			break;
		case '-':
			add_query_token(query, curr_query_token);
			curr_query_token.type = QueryToken::MINUS;
			add_query_token(query, curr_query_token);
			break;
		case '/':
			add_query_token(query, curr_query_token);
			curr_query_token.type = QueryToken::DIV;
			add_query_token(query, curr_query_token);
			break;
		case '%':
			add_query_token(query, curr_query_token);
			curr_query_token.type = QueryToken::MOD;
			add_query_token(query, curr_query_token);
			break;
		case '=':
			add_query_token(query, curr_query_token);
			curr_query_token.type = QueryToken::EQUAL;
			add_query_token(query, curr_query_token);
			break;
		case '.':
			add_query_token(query, curr_query_token);
			curr_query_token.token_value.push_back(c);
			break;
		case '#':
			if (curr_query_token.token_value == ".stmt") {
				curr_query_token.token_value.push_back(c);
			} else {
				throw SyntacticErrorException("Unexpected symbol : \'#\'");
			}
			break;
		default:
			if (isdigit(c)) {
				handleDigit(query, c, curr_query_token);
			}
			else if (isalpha(c)) {
				if (curr_query_token.type == QueryToken::IDENTIFIER || curr_query_token.type == QueryToken::WHITESPACE) {
					curr_query_token.type = QueryToken::IDENTIFIER;
					curr_query_token.token_value.push_back(c);
				}
				else {
					throw SyntacticErrorException("Invalid Name present");
				}
				
			}
			else {
				throw SyntacticErrorException("Unknown symbol present");
			}
			break;
		}
	}
	add_query_token(query, curr_query_token);

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
	}
	else if (curr_query_token.type == QueryToken::CONSTANT || curr_query_token.type == QueryToken::WHITESPACE) {
		curr_query_token.type = QueryToken::CONSTANT;
		curr_query_token.token_value.push_back(c);
	}
}

void QueryTokenizer::add_query_token(Query& query, QueryToken& query_token) {
	if (query_token.type == QueryToken::WHITESPACE) {
		//discard whitespace
		return;
	}

	if (query_token.type == QueryToken::IDENTIFIER) {

		if (query_token.token_value == ".procName") {
			query_token.type = QueryToken::DOT;
			query_token.token_value = "";
			query_token_cache.push_back(query_token);
			query_token.type = QueryToken::PROC_NAME;
			query_token.token_value = "";
		}
		else if (query_token.token_value == ".varName") {
			query_token.type = QueryToken::DOT;
			query_token.token_value = "";
			query_token_cache.push_back(query_token);
			query_token.type = QueryToken::VAR_NAME;
			query_token.token_value = "";
		}
		else if (query_token.token_value == ".value") {
			query_token.type = QueryToken::DOT;
			query_token.token_value = "";
			query_token_cache.push_back(query_token);
			query_token.type = QueryToken::VALUE;
			query_token.token_value = "";
		}
		else if (query_token.token_value == ".stmt#") {
			query_token.type = QueryToken::DOT;
			query_token.token_value = "";
			query_token_cache.push_back(query_token);
			query_token.type = QueryToken::STMT_INDEX;
			query_token.token_value = "";
		}
	}
	else if (query_token.type == QueryToken::CONSTANT) {
		if (query_token.token_value[0] == '0' && query_token.token_value.length() > 1) {
			query.setIsSemanticError("No leading '0's allowed ");
		}
	}


	query_token_cache.push_back(query_token);
	query_token.type = QueryToken::QueryTokenType::WHITESPACE;
	query_token.token_value = "";
}