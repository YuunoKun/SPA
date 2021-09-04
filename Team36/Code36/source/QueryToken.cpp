#include <string>
#include "QueryToken.h"

QueryToken::QueryToken() {
	type = QueryTokenType::WHITESPACE;
	token_value = "";
}

QueryToken::QueryToken(QueryTokenType type, std::string token_value) {
	this->type = type;
	this->token_value = token_value;
}