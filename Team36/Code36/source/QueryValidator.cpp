#include "QueryToken.h"
#include "QueryValidator.h"
#include "QueryPreprocessor.h"
#include "SemanticErrorException.h"
#include "SyntacticErrorException.h"

QueryValidator::QueryValidator() {
}

void QueryValidator::validateSelectMultipleClauses(QueryToken& token, QueryToken& prevTokenSelect) {
	if (token.type == QueryToken::QueryTokenType::IDENTIFIER &&
		prevTokenSelect.type != QueryToken::QueryTokenType::TUPLE_OPEN &&
		prevTokenSelect.type != QueryToken::QueryTokenType::COMMA) {
		throw SyntacticErrorException("During multiple selects, identifier can only come after '<' or ','");
	}
	else if (token.type == QueryToken::QueryTokenType::DOT &&
		prevTokenSelect.type != QueryToken::QueryTokenType::IDENTIFIER) {
		throw SyntacticErrorException("During multiple selects, '.' can only come after identifier");
	}
	else if ((token.type == QueryToken::QueryTokenType::PROC_NAME ||
		token.type == QueryToken::QueryTokenType::VAR_NAME ||
		token.type == QueryToken::QueryTokenType::VALUE ||
		token.type == QueryToken::QueryTokenType::STMT_INDEX) &&
		prevTokenSelect.type != QueryToken::QueryTokenType::DOT) {
		throw SyntacticErrorException("During multiple selects, attributes can only come after '.'");
	}
	else if (token.type == QueryToken::QueryTokenType::COMMA &&
		(prevTokenSelect.type != QueryToken::QueryTokenType::PROC_NAME &&
			prevTokenSelect.type != QueryToken::QueryTokenType::VAR_NAME &&
			prevTokenSelect.type != QueryToken::QueryTokenType::VALUE &&
			prevTokenSelect.type != QueryToken::QueryTokenType::IDENTIFIER &&
			prevTokenSelect.type != QueryToken::QueryTokenType::STMT_INDEX)) {
		throw SyntacticErrorException("During multiple selects, comma can only come after attributes");
	}
	else if (token.type == QueryToken::QueryTokenType::TUPLE_CLOSE &&
		prevTokenSelect.type != QueryToken::QueryTokenType::PROC_NAME &&
		prevTokenSelect.type != QueryToken::QueryTokenType::VAR_NAME &&
		prevTokenSelect.type != QueryToken::QueryTokenType::VALUE &&
		prevTokenSelect.type != QueryToken::QueryTokenType::IDENTIFIER &&
		prevTokenSelect.type != QueryToken::QueryTokenType::STMT_INDEX) {
		throw SyntacticErrorException("During multiple selects, '>' can only come after identifier or attributes");
	}
}