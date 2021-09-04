#include <string>
#include "QueryToken.h"

QueryToken::QueryToken() {
	type = QueryTokenType::WHITESPACE;
	token_value = "";
}

QueryToken::QueryToken(QueryTokenType type_input, std::string token_value_input) {
	type = type_input;
	token_value = token_value_input;
}