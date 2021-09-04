#include "FSM.h"


using namespace SourceProcessor;


FSM::FSM(Tokenizer& tokenizer) {
	m_tokenizer = tokenizer;
}

void FSM::build() {
	m_tokenizer.init_token_stack();

	expect_procedure();

	while (optional_token(TokenType::PROCEDURE)) {
		expect_procedure();
	}
}


void FSM::expect_procedure() {
	expect_token_and_pop(TokenType::PROCEDURE);
	Token proc_name_token = expect_token_and_pop(TokenType::IDENTIFIER);
	// add proc_name and get handle
	// add direct parent
	expect_token_and_pop(TokenType::STATEMENT_LIST_OPEN);
	expect_statement_list();
	expect_token_and_pop(TokenType::STATEMENT_LIST_CLOSE);
}


void FSM::expect_statement_list() {
	expect_statement();

	while (optional_token(TokenType::PROCEDURE)) {
		expect_statement();
	}
}


void FSM::expect_statement() {
	switch (m_tokenizer.peek_token().get_token_type()) {
	case TokenType::READ:
		expect_statement_type_read();
		break;
	case TokenType::PRINT:
		expect_statement_type_print();
		break;
	case TokenType::CALL:
		expect_statement_type_call();
		break;
	case TokenType::WHILE:
		expect_statement_type_while();
		break;
	case TokenType::IF:
		expect_statement_type_if();
		break;
	case TokenType::IDENTIFIER:
		expect_statement_type_assign();
		break;
	default:
		unexpected_token();
		break;
	}
}


void FSM::expect_statement_type_read() {
	// make read stmt
	expect_token_and_pop(TokenType::READ);
	// add modified var_Name;
	expect_token_and_pop(TokenType::TERMINATOR);
}


void FSM::expect_statement_type_print() {
	// make print stmt
	expect_token_and_pop(TokenType::PRINT);
	// add used var_name;
	expect_token_and_pop(TokenType::TERMINATOR);
}


void FSM::expect_statement_type_call() {
	// make call stmt
	expect_token_and_pop(TokenType::CALL);
	// add proc_name
	expect_token_and_pop(TokenType::TERMINATOR);
}


void FSM::expect_statement_type_while() {
	// make while stmt
	expect_token_and_pop(TokenType::WHILE);

	expect_token_and_pop(TokenType::PARENTHESIS_OPEN);
	expect_conditional_expression();
	expect_token_and_pop(TokenType::PARENTHESIS_CLOSE);

	expect_token_and_pop(TokenType::STATEMENT_LIST_OPEN);
	expect_statement_list();
	expect_token_and_pop(TokenType::STATEMENT_LIST_CLOSE);
}


void FSM::expect_statement_type_if() {
	// make if stmt
	expect_token_and_pop(TokenType::IF);

	expect_token_and_pop(TokenType::PARENTHESIS_OPEN);
	expect_conditional_expression();
	expect_token_and_pop(TokenType::PARENTHESIS_CLOSE);

	expect_token_and_pop(TokenType::THEN);

	expect_token_and_pop(TokenType::STATEMENT_LIST_OPEN);
	expect_statement_list();
	expect_token_and_pop(TokenType::STATEMENT_LIST_CLOSE);

	expect_token_and_pop(TokenType::ELSE);

	expect_token_and_pop(TokenType::STATEMENT_LIST_OPEN);
	expect_statement_list();
	expect_token_and_pop(TokenType::STATEMENT_LIST_CLOSE);
}


void FSM::expect_statement_type_assign() {
	// make assign stmt
	variable_token = expect_token_and_pop(TokenType::IDENTIFIER);
	// add modified var_name
	expect_token_and_pop(TokenType::ASSIGN);
	expect_expression();
	expect_token_and_pop(TokenType::TERMINATOR);
}


void FSM::expect_conditional_expression() {
	switch (m_tokenizer.peek_token().get_token_type()) {
	case TokenType::BOOL_NEGATE:
		expect_token_and_pop(TokenType::BOOL_NEGATE);
		expect_token_and_pop(TokenType::PARENTHESIS_OPEN);
		expect_conditional_expression();
		expect_token_and_pop(TokenType::PARENTHESIS_CLOSE);
		break;
	case TokenType::PARENTHESIS_OPEN:
		expect_token_and_pop(TokenType::PARENTHESIS_OPEN);
		expect_conditional_expression();
		expect_token_and_pop(TokenType::PARENTHESIS_CLOSE);

		switch (m_tokenizer.peek_token().get_token_type()) {
		case TokenType::BOOL_AND:
		case TokenType::BOOL_OR:
			expect_token_and_pop(m_tokenizer.peek_token().get_token_type());
			expect_token_and_pop(TokenType::PARENTHESIS_OPEN);
			expect_conditional_expression();
			expect_token_and_pop(TokenType::PARENTHESIS_CLOSE);
			break;
		default:
			break;
		}
		break;
	default:
		expect_relational_expression();
		break;
	}
}


void FSM::expect_relational_expression() {
	expect_relational_factor();

	switch (m_tokenizer.peek_token().get_token_type()) {
	case TokenType::BOOL_GT:
	case TokenType::BOOL_GTEQ:
	case TokenType::BOOL_LT:
	case TokenType::BOOL_LTEQ:
	case TokenType::BOOL_EQUIV:
	case TokenType::BOOL_NEQUIV:
		expect_token_and_pop(m_tokenizer.peek_token().get_token_type());
		break;
	default:
		unexpected_token();
		break;
	}

	expect_relational_factor();
}


void FSM::expect_relational_factor() {
	switch (m_tokenizer.peek_token().get_token_type()) {
	case TokenType::IDENTIFIER:
		// var_name
		break;
	case TokenType::CONSTANT:
		// add constant
		break;
	default:
		// start of expression
		expect_expression();
		break;
	}
}


void FSM::expect_expression() {
	expect_term();
	switch (m_tokenizer.peek_token().get_token_type()) {
	case TokenType::PLUS:
	case TokenType::MINUS:
		expect_token_and_pop(m_tokenizer.peek_token().get_token_type());
		expect_expression();
		break;
	default:
		break;
	}
}


void FSM::expect_term() {
	expect_factor();
	switch (m_tokenizer.peek_token().get_token_type()) {
	case TokenType::MUL:
	case TokenType::DIV:
	case TokenType::MOD:
		expect_token_and_pop(m_tokenizer.peek_token().get_token_type());
		expect_term();
		break;
	default:
		break;
	}
}


void FSM::expect_factor() {
	switch (m_tokenizer.peek_token().get_token_type()) {
	case TokenType::IDENTIFIER:
		//
		break;
	case TokenType::CONSTANT:
		//
		break;
	case TokenType::PARENTHESIS_OPEN:
		expect_token_and_pop(TokenType::PARENTHESIS_OPEN);
		expect_expression();
		expect_token_and_pop(TokenType::PARENTHESIS_CLOSE);
		break;
	default:
		unexpected_token();
		break;
	}
}


Token& FSM::expect_token_and_pop(TokenType token_type) {
	if (optional_token(token_type)) {
		return *m_tokenizer.pop_token();
	}
	else {
		unexpected_token();
	}
}


void FSM::unexpected_token() {
	throw std::runtime_error("Unexpected token: ");
}


bool FSM::optional_token(TokenType token_type) {
	return m_tokenizer.peek_token().m_token_value == token_type;
}


