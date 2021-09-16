#include "FSM.h"


using namespace SourceProcessor;


FSM::FSM(Tokenizer& tokenizer) {
	m_tokenizer = tokenizer;
	m_design_extractor = new DesignExtractor();
}

FSM::FSM(Tokenizer& tokenizer, Extractor *extractor) {
	m_tokenizer = tokenizer;
	m_design_extractor = extractor;
}


Tokenizer& FSM::get_tokenizer() {
	return m_tokenizer;
}


void FSM::build() {
	m_tokenizer.init_token_stack();

	expect_procedure();

	while (optional_token(TokenType::PROCEDURE)) {
		expect_procedure();
	}

	m_design_extractor->populateEntities(PKB::getInstance());
	m_design_extractor->validate();
	m_design_extractor->populate_post_validation();
	m_design_extractor->populateRelations(PKB::getInstance());
}


void FSM::expect_procedure() {
	expect_token_and_pop(TokenType::PROCEDURE);

	Token proc_name_token = expect_token_and_pop(TokenType::IDENTIFIER);
	m_design_extractor->add_procedure(proc_name_token.get_token_value());
	// nesting

	expect_token_and_pop(TokenType::STATEMENT_LIST_OPEN);
	expect_statement_list();
	expect_token_and_pop(TokenType::STATEMENT_LIST_CLOSE);
}


void FSM::expect_statement_list() {
	expect_statement();

	while (!optional_token(TokenType::STATEMENT_LIST_CLOSE)) {
		expect_statement();
	}
}


void FSM::expect_statement() {
	switch (m_tokenizer.peek_token().get_token_type()) {
	case TokenType::READ:
		m_design_extractor->add_statement(TokenType::READ);
		expect_statement_type_read();
		break;
	case TokenType::PRINT:
		m_design_extractor->add_statement(TokenType::PRINT);
		expect_statement_type_print();
		break;
	case TokenType::CALL:
		m_design_extractor->add_statement(TokenType::CALL);
		expect_statement_type_call();
		break;
	case TokenType::WHILE:
		m_design_extractor->add_statement(TokenType::WHILE);
		expect_statement_type_while();
		break;
	case TokenType::IF:
		m_design_extractor->add_statement(TokenType::IF);
		expect_statement_type_if();
		break;
	case TokenType::IDENTIFIER:
		m_design_extractor->add_statement(TokenType::ASSIGN);
		expect_statement_type_assign();
		break;
	default:
		unexpected_token("expect_statement error.");
		break;
	}
}


void FSM::expect_statement_type_read() {
	expect_token_and_pop(TokenType::READ);

	Token var_name_token = expect_token_and_pop(TokenType::IDENTIFIER);
	m_design_extractor->add_variable(var_name_token.get_token_value());
	m_design_extractor->add_statement_modifies(var_name_token.get_token_value());

	expect_token_and_pop(TokenType::TERMINATOR);
}


void FSM::expect_statement_type_print() {
	expect_token_and_pop(TokenType::PRINT);

	Token var_name_token = expect_token_and_pop(TokenType::IDENTIFIER);
	m_design_extractor->add_variable(var_name_token.get_token_value());
	m_design_extractor->add_statement_uses(var_name_token.get_token_value());

	expect_token_and_pop(TokenType::TERMINATOR);
}


void FSM::expect_statement_type_call() {
	expect_token_and_pop(TokenType::CALL);

	Token proc_name_token = expect_token_and_pop(TokenType::IDENTIFIER);
	m_design_extractor->add_callee(proc_name_token.get_token_value());

	expect_token_and_pop(TokenType::TERMINATOR);
}


void FSM::expect_statement_type_while() {
	expect_token_and_pop(TokenType::WHILE);

	expect_token_and_pop(TokenType::PARENTHESIS_OPEN);
	expect_conditional_expression();
	expect_token_and_pop(TokenType::PARENTHESIS_CLOSE);

	m_design_extractor->start_nesting();
	expect_token_and_pop(TokenType::STATEMENT_LIST_OPEN);
	expect_statement_list();
	expect_token_and_pop(TokenType::STATEMENT_LIST_CLOSE);
	m_design_extractor->end_nesting();
}


void FSM::expect_statement_type_if() {
	// make if stmt
	expect_token_and_pop(TokenType::IF);

	expect_token_and_pop(TokenType::PARENTHESIS_OPEN);
	expect_conditional_expression();
	expect_token_and_pop(TokenType::PARENTHESIS_CLOSE);

	expect_token_and_pop(TokenType::THEN);

	m_design_extractor->start_nesting();
	expect_token_and_pop(TokenType::STATEMENT_LIST_OPEN);
	expect_statement_list();
	expect_token_and_pop(TokenType::STATEMENT_LIST_CLOSE);

	expect_token_and_pop(TokenType::ELSE);
	m_design_extractor->chop_nesting();

	expect_token_and_pop(TokenType::STATEMENT_LIST_OPEN);
	expect_statement_list();
	expect_token_and_pop(TokenType::STATEMENT_LIST_CLOSE);
	m_design_extractor->end_nesting();
}


void FSM::expect_statement_type_assign() {
	// make assign stmt
	Token var_name_token = expect_token_and_pop(TokenType::IDENTIFIER);
	m_design_extractor->add_variable(var_name_token.get_token_value());
	m_design_extractor->add_statement_modifies(var_name_token.get_token_value());

	expect_token_and_pop(TokenType::ASSIGN);
	m_design_extractor->start_expr();
	expect_expression();
	m_design_extractor->end_expr();
	expect_token_and_pop(TokenType::TERMINATOR);
}


void FSM::expect_conditional_expression() {
	if (optional_relational_expression()) {
		expect_relational_expression();
		return;
	}

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
			unexpected_token("expect_conditional_expression error. Expecting '&&' or '||'.");
			break;
		}
		
		break;
	default:
		unexpected_token("expect_conditional_expression error.");
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
		unexpected_token("expect_relational_expression error.");
		break;
	}

	expect_relational_factor();
}


void FSM::expect_relational_factor() {
	expect_expression();
	//switch (m_tokenizer.peek_token().get_token_type()) {
	//case TokenType::IDENTIFIER:
	//	m_design_extractor.add_variable(expect_token_and_pop(TokenType::IDENTIFIER).get_token_value());
	//	break;
	//case TokenType::CONSTANT:
	//	m_design_extractor.add_constant(std::stoul(expect_token_and_pop(TokenType::CONSTANT).get_token_value(), nullptr, 0));
	//	break;
	//default:
	//	// start of expression
	//	expect_expression();
	//	break;
	//}
}


void FSM::expect_expression() {
	expect_term();
	switch (m_tokenizer.peek_token().get_token_type()) {
	case TokenType::PLUS:
	case TokenType::MINUS:
		m_design_extractor->add_expr_segment(m_tokenizer.peek_token().get_token_value());
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
		m_design_extractor->add_expr_segment(m_tokenizer.peek_token().get_token_value());
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
		m_design_extractor->add_expr_segment(m_tokenizer.peek_token().get_token_value());
		m_design_extractor->add_variable(m_tokenizer.peek_token().get_token_value());
		m_design_extractor->add_statement_uses(m_tokenizer.peek_token().get_token_value());
		expect_token_and_pop(TokenType::IDENTIFIER);
		break;
	case TokenType::CONSTANT:
		m_design_extractor->add_expr_segment(m_tokenizer.peek_token().get_token_value());
		m_design_extractor->add_constant(std::stoul(expect_token_and_pop(TokenType::CONSTANT).get_token_value(), nullptr, 0));
		break;
	case TokenType::PARENTHESIS_OPEN:
		m_design_extractor->add_expr_segment(m_tokenizer.peek_token().get_token_value());
		expect_token_and_pop(TokenType::PARENTHESIS_OPEN);
		expect_expression();
		m_design_extractor->add_expr_segment(m_tokenizer.peek_token().get_token_value());
		expect_token_and_pop(TokenType::PARENTHESIS_CLOSE);
		break;
	default:
		unexpected_token("expect_factor error.");
		break;
	}
}


bool FSM::optional_relational_expression() {
	if (!optional_relational_factor()){
		return false;
	}

	if (!(probe_and_pop(TokenType::BOOL_GT)
		|| probe_and_pop(TokenType::BOOL_GTEQ)
		|| probe_and_pop(TokenType::BOOL_LT)
		|| probe_and_pop(TokenType::BOOL_LTEQ)
		|| probe_and_pop(TokenType::BOOL_EQUIV)
		|| probe_and_pop(TokenType::BOOL_NEQUIV))) {
		return false;
	}

	if (!optional_relational_factor()) {
		return false;
	}
}


bool FSM::optional_relational_factor() {
	return optional_expression();
	//switch (m_tokenizer.peek_token().get_token_type()) {
	//case TokenType::IDENTIFIER:
	//	m_design_extractor.add_variable(expect_token_and_pop(TokenType::IDENTIFIER).get_token_value());
	//	break;
	//case TokenType::CONSTANT:
	//	m_design_extractor.add_constant(std::stoul(expect_token_and_pop(TokenType::CONSTANT).get_token_value(), nullptr, 0));
	//	break;
	//default:
	//	// start of expression
	//	expect_expression();
	//	break;
	//}
}


bool FSM::optional_expression() {
	if (!optional_term()) {
		return false;
	}

	if ((probe_and_pop(TokenType::PLUS) || probe_and_pop(TokenType::MINUS))) {
		if (!optional_expression()) {
			return false;
		}
	}

	return true;
}


bool FSM::optional_term() {
	if (!optional_factor()) {
		return false;
	}

	if ((probe_and_pop(TokenType::MUL) || probe_and_pop(TokenType::DIV) || probe_and_pop(TokenType::MOD))) {
		if (!optional_term()) {
			return false;
		}
	}

	return true;
}


bool FSM::optional_factor() {
	if ((probe_and_pop(TokenType::IDENTIFIER) || probe_and_pop(TokenType::CONSTANT))) {
		return true;
	}
	else if (probe_and_pop(TokenType::PARENTHESIS_OPEN)) {
		return optional_expression() && probe_and_pop(TokenType::PARENTHESIS_CLOSE);
	}
	else {
		return false;
	}
}


Token& FSM::expect_token_and_pop(TokenType token_type) {
	if (optional_token(token_type)) {
		return m_tokenizer.pop_token();
	}
	else {
		unexpected_token("");
	}
}


bool FSM::optional_token(TokenType token_type) {
	return m_tokenizer.peek_token().get_token_type() == token_type;
}


bool FSM::probe_and_pop(TokenType token_type) {
	if (probe_and_peek(token_type)) {
		m_tokenizer.pop_probe();
		return true;
	}
	else {
		return false;
	}
}


bool FSM::probe_and_peek(TokenType token_type) {
	return m_tokenizer.peek_probe().get_token_type() == token_type;
}


void FSM::unexpected_token(std::string message) {
	throw std::runtime_error(message + std::string(" Unexpected token: ") + tokenTypeStrings[m_tokenizer.peek_token().get_token_type()]);
}