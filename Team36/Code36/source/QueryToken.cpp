#include <string>
#include "QueryToken.h"

QueryToken::QueryToken() {
	type = QueryTokenType::WHITESPACE;
	token_value = "";
}

QueryToken::QueryToken(QueryTokenType type, std::string token_value) {
	type = type;
	token_value = token_value;
}