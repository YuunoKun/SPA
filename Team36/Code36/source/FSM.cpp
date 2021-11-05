#include "FSM.h"

using namespace SourceProcessor;

FSM::FSM(Tokenizer& t) {
	tokenizer = t;
	design_extractor = new DesignExtractor(PKB::getInstance());
}

FSM::FSM(Tokenizer& t, Extractor *extractor) {
	tokenizer = t;
	design_extractor = extractor;
}

FSM::~FSM() {
	if(design_extractor) design_extractor->~Extractor();
}

Tokenizer& FSM::getTokenizer() {
	return tokenizer;
}

void FSM::build() {
	tokenizer.initTokenStack();

	expectProcedure();

	while (peekToken(TokenType::PROCEDURE)) {
		expectProcedure();
	}

	design_extractor->populateEntities();
	design_extractor->validate();
	design_extractor->populatePostValidation();
	design_extractor->populateRelations();
}

void FSM::expectProcedure() {
	expectTokenAndPop(TokenType::PROCEDURE);

	Token proc_name_token = expectIdentifier();
	design_extractor->addProcedure(proc_name_token.getTokenValue());
	// nesting

	expectTokenAndPop(TokenType::STATEMENT_LIST_OPEN);
	expectStatementList();
	expectTokenAndPop(TokenType::STATEMENT_LIST_CLOSE);
}

void FSM::expectStatementList() {
	expectStatement();

	while (!peekToken(TokenType::STATEMENT_LIST_CLOSE)) {
		expectStatement();
	}
}

void FSM::expectStatement() {
	if (!optionalIdentifier()) {
		unexpectedToken("error starting of statement.");
	}

	if (probeAndPeek(TokenType::ASSIGN)) {
		design_extractor->addStatement(TokenType::ASSIGN);
		expectStatementTypeAssign();
	}
	else if (probeAndPeek(TokenType::PARENTHESIS_OPEN)) {
		switch (tokenizer.peekToken().getTokenType()) {
		case TokenType::WHILE:
			design_extractor->addStatement(TokenType::WHILE);
			expectStatementTypeWhile();
			break;
		case TokenType::IF:
			design_extractor->addStatement(TokenType::IF);
			expectStatementTypeIf();
			break;
		default:
			unexpectedToken("error starting of statement.");
			break;
		}
	}
	else {
		switch (tokenizer.peekToken().getTokenType()) {
		case TokenType::READ:
			design_extractor->addStatement(TokenType::READ);
			expectStatementTypeRead();
			break;
		case TokenType::PRINT:
			design_extractor->addStatement(TokenType::PRINT);
			expectStatementTypePrint();
			break;
		case TokenType::CALL:
			design_extractor->addStatement(TokenType::CALL);
			expectStatementTypeCall();
			break;
		default:
			unexpectedToken("Invalid statement.");
			break;
		}
	}
}

void FSM::expectStatementTypeRead() {
	expectTokenAndPop(TokenType::READ);

	Token var_name_token = expectIdentifier();
	design_extractor->addVariable(var_name_token.getTokenValue());
	design_extractor->addStatementModifies(var_name_token.getTokenValue());

	expectTokenAndPop(TokenType::TERMINATOR);
}

void FSM::expectStatementTypePrint() {
	expectTokenAndPop(TokenType::PRINT);

	Token var_name_token = expectIdentifier();
	design_extractor->addVariable(var_name_token.getTokenValue());
	design_extractor->addStatementUses(var_name_token.getTokenValue());

	expectTokenAndPop(TokenType::TERMINATOR);
}

void FSM::expectStatementTypeCall() {
	expectTokenAndPop(TokenType::CALL);

	Token proc_name_token = expectIdentifier();
	design_extractor->addCallee(proc_name_token.getTokenValue());

	expectTokenAndPop(TokenType::TERMINATOR);
}

void FSM::expectStatementTypeWhile() {
	expectTokenAndPop(TokenType::WHILE);

	expectTokenAndPop(TokenType::PARENTHESIS_OPEN);
	design_extractor->setCondExpr(true);
	expectConditionalExpression();
	design_extractor->setCondExpr(false);
	expectTokenAndPop(TokenType::PARENTHESIS_CLOSE);

	design_extractor->startNesting();
	expectTokenAndPop(TokenType::STATEMENT_LIST_OPEN);
	expectStatementList();
	expectTokenAndPop(TokenType::STATEMENT_LIST_CLOSE);
	design_extractor->endNesting();
}

void FSM::expectStatementTypeIf() {
	// make if stmt
	expectTokenAndPop(TokenType::IF);

	expectTokenAndPop(TokenType::PARENTHESIS_OPEN);
	design_extractor->setCondExpr(true);
	expectConditionalExpression();
	design_extractor->setCondExpr(false);
	expectTokenAndPop(TokenType::PARENTHESIS_CLOSE);

	expectTokenAndPop(TokenType::THEN);

	design_extractor->startNesting();
	expectTokenAndPop(TokenType::STATEMENT_LIST_OPEN);
	expectStatementList();
	expectTokenAndPop(TokenType::STATEMENT_LIST_CLOSE);

	expectTokenAndPop(TokenType::ELSE);
	design_extractor->chopNesting();

	expectTokenAndPop(TokenType::STATEMENT_LIST_OPEN);
	expectStatementList();
	expectTokenAndPop(TokenType::STATEMENT_LIST_CLOSE);
	design_extractor->endNesting();
}

void FSM::expectStatementTypeAssign() {
	// make assign stmt
	Token var_name_token = expectIdentifier();
	design_extractor->addVariable(var_name_token.getTokenValue());
	design_extractor->addStatementModifies(var_name_token.getTokenValue());

	expectTokenAndPop(TokenType::ASSIGN);
	design_extractor->startExpr();
	expectExpression();
	design_extractor->endExpr();
	expectTokenAndPop(TokenType::TERMINATOR);
}

void FSM::expectConditionalExpression() {
	if (optionalRelationalExpression()) {
		tokenizer.resetProbe();
		expectRelationalExpression();
		return;
	}

	switch (tokenizer.peekToken().getTokenType()) {
	case TokenType::BOOL_NEGATE:
		expectTokenAndPop(TokenType::BOOL_NEGATE);
		expectTokenAndPop(TokenType::PARENTHESIS_OPEN);
		expectConditionalExpression();
		expectTokenAndPop(TokenType::PARENTHESIS_CLOSE);
		break;
	case TokenType::PARENTHESIS_OPEN:
		expectTokenAndPop(TokenType::PARENTHESIS_OPEN);
		expectConditionalExpression();
		expectTokenAndPop(TokenType::PARENTHESIS_CLOSE);
		
		switch (tokenizer.peekToken().getTokenType()) {
		case TokenType::BOOL_AND:
		case TokenType::BOOL_OR:
			expectTokenAndPop(tokenizer.peekToken().getTokenType());
			expectTokenAndPop(TokenType::PARENTHESIS_OPEN);
			expectConditionalExpression();
			expectTokenAndPop(TokenType::PARENTHESIS_CLOSE);
			break;
		default:
			unexpectedToken("expectConditionalExpression error. Expecting '&&' or '||'.");
			break;
		}
		
		break;
	default:
		unexpectedToken("expectConditionalExpression error.");
		break;
	}
}

void FSM::expectRelationalExpression() {
	expectRelationalFactor();
	switch (tokenizer.peekToken().getTokenType()) {
	case TokenType::BOOL_GT:
	case TokenType::BOOL_GTEQ:
	case TokenType::BOOL_LT:
	case TokenType::BOOL_LTEQ:
	case TokenType::BOOL_EQUIV:
	case TokenType::BOOL_NEQUIV:
		expectTokenAndPop(tokenizer.peekToken().getTokenType());
		break;
	default:
		unexpectedToken("expectRelationalExpression error.");
		break;
	}

	expectRelationalFactor();
}

void FSM::expectRelationalFactor() {
	expectExpression();
}

void FSM::expectExpression() {
	expectTerm();
	switch (tokenizer.peekToken().getTokenType()) {
	case TokenType::PLUS:
	case TokenType::MINUS:
		design_extractor->addExprSegment(tokenizer.peekToken().getTokenValue());
		expectTokenAndPop(tokenizer.peekToken().getTokenType());
		expectExpression();
		break;
	default:
		break;
	}
}

void FSM::expectTerm() {
	expectFactor();
	switch (tokenizer.peekToken().getTokenType()) {
	case TokenType::MUL:
	case TokenType::DIV:
	case TokenType::MOD:
		design_extractor->addExprSegment(tokenizer.peekToken().getTokenValue());
		expectTokenAndPop(tokenizer.peekToken().getTokenType());
		expectTerm();
		break;
	default:
		break;
	}
}

void FSM::expectFactor() {
	if (optionalIdentifier()) {
		Token identifier_token = expectIdentifier();
		design_extractor->addExprSegment(identifier_token.getTokenValue());
		design_extractor->addVariable(identifier_token.getTokenValue());
		design_extractor->addStatementUses(identifier_token.getTokenValue());
		return;
	}

	switch (tokenizer.peekToken().getTokenType()) {
	case TokenType::CONSTANT:
		design_extractor->addExprSegment(tokenizer.peekToken().getTokenValue());
		if (tokenizer.peekToken().getTokenValue().size() > 1 && tokenizer.peekToken().getTokenValue().front() == '0') {
			throw std::runtime_error("Semantic violation. Constant with leading zero detected.");
		}
		design_extractor->addConstant(std::stoul(tokenizer.peekToken().getTokenValue(), nullptr, 0));
		expectTokenAndPop(TokenType::CONSTANT);
		break;
	case TokenType::PARENTHESIS_OPEN:
		design_extractor->addExprSegment(tokenizer.peekToken().getTokenValue());
		expectTokenAndPop(TokenType::PARENTHESIS_OPEN);
		expectExpression();
		design_extractor->addExprSegment(tokenizer.peekToken().getTokenValue());
		expectTokenAndPop(TokenType::PARENTHESIS_CLOSE);
		break;
	default:
		unexpectedToken("expectFactor error.");
		break;
	}
}

Token FSM::expectIdentifier() {
	switch (tokenizer.peekToken().getTokenType()) {
	case TokenType::PROCEDURE:
	case TokenType::READ:
	case TokenType::PRINT:
	case TokenType::CALL:
	case TokenType::IF:
	case TokenType::WHILE:
	case TokenType::THEN:
	case TokenType::ELSE:
	case TokenType::IDENTIFIER:
		break;
	default:
		unexpectedToken("expectIdentifier error.");
		break;
	}

	return expectTokenAndPop(tokenizer.peekToken().getTokenType());
}

bool FSM::optionalRelationalExpression() {
	if (!optionalRelationalFactor()) {
		return false;
	}

	if (!(probeAndPop(TokenType::BOOL_GT)
		|| probeAndPop(TokenType::BOOL_GTEQ)
		|| probeAndPop(TokenType::BOOL_LT)
		|| probeAndPop(TokenType::BOOL_LTEQ)
		|| probeAndPop(TokenType::BOOL_EQUIV)
		|| probeAndPop(TokenType::BOOL_NEQUIV))) {
		return false;
	}

	if (!optionalRelationalFactor()) {
		return false;
	}

	return true;
}

bool FSM::optionalRelationalFactor() {
	return optionalExpression();
}

bool FSM::optionalExpression() {
	if (!optionalTerm()) {
		return false;
	}

	if ((probeAndPop(TokenType::PLUS) || probeAndPop(TokenType::MINUS))) {
		return optionalExpression();
	}

	return true;
}

bool FSM::optionalTerm() {
	if (!optionalFactor()) {
		return false;
	}

	if ((probeAndPop(TokenType::MUL) || probeAndPop(TokenType::DIV) || probeAndPop(TokenType::MOD))) {
		return optionalTerm();
	}

	return true;
}

bool FSM::optionalFactor() {
	if(optionalIdentifier() || probeAndPop(TokenType::CONSTANT)) {
		return true;
	}
	else if (probeAndPop(TokenType::PARENTHESIS_OPEN)) {
		return optionalExpression() && probeAndPop(TokenType::PARENTHESIS_CLOSE);
	}
	else {
		return false;
	}
}

bool FSM::optionalIdentifier() {
	switch (tokenizer.peekProbe().getTokenType()) {
	case TokenType::PROCEDURE:
	case TokenType::READ:
	case TokenType::PRINT:
	case TokenType::CALL:
	case TokenType::IF:
	case TokenType::WHILE:
	case TokenType::THEN:
	case TokenType::ELSE:
	case TokenType::IDENTIFIER:
		probeAndPop(tokenizer.peekProbe().getTokenType());
		return true;
		break;
	default:
		return false;
		break;
	}

	return false;
}

Token FSM::expectTokenAndPop(TokenType token_type) {
	tokenizer.resetProbe();
	if (peekToken(token_type)) {
		return tokenizer.popToken();
	}
	else {
		unexpectedToken("");
		return Token(TokenType::INVAL, TOKENVALUE_PLACEHOLDER);
	}
}

bool FSM::peekToken(TokenType token_type) {
	return tokenizer.peekToken().getTokenType() == token_type;
}

bool FSM::probeAndPop(TokenType token_type) {
	if (probeAndPeek(token_type)) {
		tokenizer.popProbe();
		return true;
	}
	else {
		return false;
	}
}

bool FSM::probeAndPeek(TokenType token_type) {
	return tokenizer.peekProbe().getTokenType() == token_type;
}

void FSM::unexpectedToken(std::string message) {
	throw std::runtime_error(message + std::string("Unexpected token: ") 
		+ tokenTypeStrings[tokenizer.peekToken().getTokenType()] 
		+ " = " + tokenizer.peekToken().getTokenValue());
}