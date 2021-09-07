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

	std::stack<char> separator_validation_stk;
	
	bool quotation_validation = false;
	bool is_such = false;
	bool is_such_temp = false;

	for (char c : input) {
		if (is_such) {
			if (c == ' ' && temp_query_token.type == QueryToken::IDENTIFIER
				&& temp_query_token.token_value == "that") {

				is_such_temp = false;

				curr_query_token.type = QueryToken::SUCH_THAT;
				curr_query_token.token_value = "";
				add_query_token(curr_query_token);

				//reset temp_query_token
				temp_query_token.type = QueryToken::WHITESPACE;
				temp_query_token.token_value = "";

				is_such = false;
				continue;
			}

			if (isalpha(c)) {
				is_such_temp = true;

				temp_query_token.type = QueryToken::IDENTIFIER;
				temp_query_token.token_value.push_back(c);
				continue;
			}
			else if (c == ' ' && temp_query_token.type == QueryToken::IDENTIFIER
				&& temp_query_token.token_value == "that") {
				
				curr_query_token.type = QueryToken::SUCH_THAT;
				curr_query_token.token_value = "";
				add_query_token(curr_query_token);
				
				//reset temp_query_token
				temp_query_token.type = QueryToken::WHITESPACE;
				temp_query_token.token_value = "";

				is_such_temp = false;
				is_such = false;
				continue;
			}
			else {
				is_such = false;
				add_query_token(curr_query_token);
				curr_query_token.type = temp_query_token.type;
				curr_query_token.token_value = temp_query_token.token_value;

				//reset temp_query_token
				temp_query_token.type = QueryToken::WHITESPACE;
				temp_query_token.token_value = "";
			}
			
		}

		switch (c) {
		case '(':
			add_query_token(curr_query_token);
			curr_query_token.type = QueryToken::PARENTHESIS_OPEN;
			add_query_token(curr_query_token);
			separator_validation_stk.push('(');
			break;
		case ')':
			if (!separator_validation_stk.empty() && separator_validation_stk.top() == '(') {
				separator_validation_stk.pop();
			}
			else {
				std::runtime_error("Unexpected symbol : \'" + c + '\'');
			}
			add_query_token(curr_query_token);
			curr_query_token.type = QueryToken::PARENTHESIS_CLOSE;
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
			// check for "such" in such that
			if (curr_query_token.type == QueryToken::IDENTIFIER 
				&& curr_query_token.token_value == "such") {
				is_such = true;
				break;
			}
			add_query_token(curr_query_token);
			curr_query_token.type = QueryToken::WHITESPACE;
			break;
		case '\n':
			add_query_token(curr_query_token);
			curr_query_token.type = QueryToken::WHITESPACE;
			break;
		case '*':
			// if got whitespace infront -> then is MUL
			// IF got IDENT infront then is -> asterisk. then parser check if is fully asterisk(parrent/uses infront) or else is MUL  
			//if (curr_query_token.type == QueryToken::CONSTANT) {
			//	//check before if CONSTANT
			//	//part of term/expr
			//	add_query_token(curr_query_token);
			//	curr_query_token.type = QueryToken::MUL;
			//	add_query_token(curr_query_token);
			//} else 
				
			//if (curr_query_token.type == QueryToken::IDENTIFIER) {
			//	// if in front is IDENT, can be for expr/term or for or MUL
			//	add_query_token(curr_query_token);
			//	curr_query_token.type = QueryToken::MUL;
			//	add_query_token(curr_query_token);
			//}
			//else {
			//	std::runtime_error("Unexpected symbol : \'" + c + '\'');
			//}

			break;
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
				curr_query_token.type = QueryToken::IDENTIFIER;
				curr_query_token.token_value.push_back(c);
			}
			else {
				std::runtime_error("Unknown symbol : \'" + c + '\'');
			}
			break;
		}
	}
	add_query_token(curr_query_token);

	// Check for open brackets, open quotation marks, temp token active
	if (!separator_validation_stk.empty()) {
		std::runtime_error("expected \')\'");
	}
	if (quotation_validation) {
		std::runtime_error("missing terminating \" character");
	}
	if (is_such_temp) {
		add_query_token(temp_query_token);
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
	

	query_token_cache.push_back(query_token);
	query_token.type = QueryToken::QueryTokenType::WHITESPACE;
	query_token.token_value = "";
}