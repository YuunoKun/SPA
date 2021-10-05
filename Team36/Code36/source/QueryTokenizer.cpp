#include <string>
#include <vector>
#include <stack>
#include <stdexcept>
#include "QueryTokenizer.h"

QueryTokenizer::QueryTokenizer() {
	query_token_cache = {};
}

void QueryTokenizer::parse_into_query_tokens(std::string input) {
	QueryToken curr_query_token;
	QueryToken temp_query_token;

	std::stack<char> parenthesis_validation_stk;
	std::stack<char> tuple_validation_stk;

	bool quotation_validation = false;

	for (char c : input) {

		if (curr_query_token.type == QueryToken::IDENTIFIER
			&& curr_query_token.token_value == "such "
			&& c != 't') {
			curr_query_token.token_value.pop_back();
			add_query_token(curr_query_token);
			curr_query_token.type = QueryToken::WHITESPACE;
		}

		switch (c) {
		case '(':
			add_query_token(curr_query_token);
			curr_query_token.type = QueryToken::PARENTHESIS_OPEN;
			add_query_token(curr_query_token);
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
			add_query_token(curr_query_token);
			curr_query_token.type = QueryToken::PARENTHESIS_CLOSE;
			add_query_token(curr_query_token);
			break;
		case '<':
			add_query_token(curr_query_token);
			curr_query_token.type = QueryToken::TUPLE_OPEN;
			add_query_token(curr_query_token);
			parenthesis_validation_stk.push('<');
			break;
		case '>':
			if (!parenthesis_validation_stk.empty() && parenthesis_validation_stk.top() == '<') {
				parenthesis_validation_stk.pop();
			}
			else {
				throw SyntacticErrorException("Unexpected symbol : \'>\'");
			}
			add_query_token(curr_query_token);
			curr_query_token.type = QueryToken::TUPLE_CLOSE;
			add_query_token(curr_query_token);
			break;
		case '"':
			if (!quotation_validation) {
				//No opening quotation marks
				add_query_token(curr_query_token);
				curr_query_token.type = QueryToken::QUOTATION_OPEN;
				add_query_token(curr_query_token);
				quotation_validation = true;
			}
			else {
				add_query_token(curr_query_token);
				curr_query_token.type = QueryToken::QUOTATION_CLOSE;
				add_query_token(curr_query_token);
				quotation_validation = false;
			}
			break;
		case ';':
			add_query_token(curr_query_token);
			curr_query_token.type = QueryToken::TERMINATOR;
			add_query_token(curr_query_token);
			break;
		case '_':
			add_query_token(curr_query_token);
			curr_query_token.type = QueryToken::WILDCARD;
			add_query_token(curr_query_token);
			break;
		case ',':
			add_query_token(curr_query_token);
			curr_query_token.type = QueryToken::COMMA;
			add_query_token(curr_query_token);
			break;
		case ' ':
			//// check for is_attr_name
			//if (is_attr_name && curr_query_token.type == QueryToken::IDENTIFIER) {
			//	
			//	if (curr_query_token.token_value == "procName") {
			//		curr_query_token.type = QueryToken::PROC_NAME;
			//		curr_query_token.token_value = "";
			//		add_query_token(curr_query_token);
			//	} else if (curr_query_token.token_value == "varName") {
			//		curr_query_token.type = QueryToken::VAR_NAME;
			//		curr_query_token.token_value = "";
			//		add_query_token(curr_query_token);
			//	} else if (curr_query_token.token_value == "value") {
			//		curr_query_token.type = QueryToken::VALUE;
			//		curr_query_token.token_value = "";
			//		add_query_token(curr_query_token);
			//	} else if (curr_query_token.token_value == "stmt#") {
			//		curr_query_token.type = QueryToken::STMT_INDEX;
			//		curr_query_token.token_value = "";
			//		add_query_token(curr_query_token);
			//	}
			//	is_attr_name = false;
			//}

			// check for "such" in such that
			if (curr_query_token.type == QueryToken::IDENTIFIER
				&& curr_query_token.token_value == "such") {
				curr_query_token.token_value.push_back(c);
				break;
			}else if (curr_query_token.type == QueryToken::IDENTIFIER
				&& curr_query_token.token_value == "such that") { 
				// check for "such that" in such that
				curr_query_token.type = QueryToken::SUCH_THAT;
				curr_query_token.token_value = "";
				add_query_token(curr_query_token);
				break;
			}
			add_query_token(curr_query_token);
			curr_query_token.type = QueryToken::WHITESPACE;
			break;
		case '\n':
			add_query_token(curr_query_token);
			curr_query_token.type = QueryToken::WHITESPACE;
			break;
		case '\t':
			add_query_token(curr_query_token);
			curr_query_token.type = QueryToken::WHITESPACE;
			break;
		case '*':
			// must NOT be in quotation brackets
			if (!quotation_validation && curr_query_token.type == QueryToken::IDENTIFIER) {
				if (curr_query_token.token_value == "Follows") {
					curr_query_token.type = QueryToken::FOLLOWS_T;
					curr_query_token.token_value = "";
					add_query_token(curr_query_token);
				} else if (curr_query_token.token_value == "Parent") {
					curr_query_token.type = QueryToken::PARENT_T;
					curr_query_token.token_value = "";
					add_query_token(curr_query_token);
				} else if (curr_query_token.token_value == "Calls") {
					curr_query_token.type = QueryToken::CALLS_T;
					curr_query_token.token_value = "";
					add_query_token(curr_query_token);
				} else if (curr_query_token.token_value == "Next") {
					curr_query_token.type = QueryToken::NEXT_T;
					curr_query_token.token_value = "";
					add_query_token(curr_query_token);
				} else if (curr_query_token.token_value == "Affects") {
					curr_query_token.type = QueryToken::AFFECTS_T;
					curr_query_token.token_value = "";
					add_query_token(curr_query_token);
				} 
				break;
			}
			else {
				add_query_token(curr_query_token);
				curr_query_token.type = QueryToken::MUL;
				add_query_token(curr_query_token);
				break;
			}
		case '+':
			add_query_token(curr_query_token);
			curr_query_token.type = QueryToken::PLUS;
			add_query_token(curr_query_token);
			break;
		case '-':
			add_query_token(curr_query_token);
			curr_query_token.type = QueryToken::MINUS;
			add_query_token(curr_query_token);
			break;
		case '/':
			add_query_token(curr_query_token);
			curr_query_token.type = QueryToken::DIV;
			add_query_token(curr_query_token);
			break;
		case '%':
			add_query_token(curr_query_token);
			curr_query_token.type = QueryToken::MOD;
			add_query_token(curr_query_token);
			break;
		case '=':
			add_query_token(curr_query_token);
			curr_query_token.type = QueryToken::EQUAL;
			add_query_token(curr_query_token);
			break;
		case '.':
			add_query_token(curr_query_token);
			curr_query_token.token_value.push_back(c);
			//curr_query_token.type = QueryToken::DOT;
			//add_query_token(curr_query_token);
			//is_attr_name = true;
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
				if (curr_query_token.type == QueryToken::IDENTIFIER) {
					curr_query_token.token_value.push_back(c);
				}
				else if (curr_query_token.type == QueryToken::WHITESPACE || curr_query_token.type == QueryToken::CONSTANT) {
					curr_query_token.type = QueryToken::CONSTANT;
					curr_query_token.token_value.push_back(c);
				}
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
	add_query_token(curr_query_token);

	// Check for open brackets, open quotation marks, temp token active
	if (!parenthesis_validation_stk.empty()) {
		throw SyntacticErrorException("expected \')\'");
	}
	if (quotation_validation) {
		throw SyntacticErrorException("missing terminating \" character");
	}
	
}

std::vector<QueryToken> QueryTokenizer::get_query_token_chain() {
	return query_token_cache;
}

void QueryTokenizer::add_query_token(QueryToken& query_token) {
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

	query_token_cache.push_back(query_token);
	query_token.type = QueryToken::QueryTokenType::WHITESPACE;
	query_token.token_value = "";
}