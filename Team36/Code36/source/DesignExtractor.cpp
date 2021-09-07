#include <stdexcept>
#include "DesignExtractor.h"
#include "PKB.h"

using namespace SourceProcessor;


DesignExtractor::DesignExtractor() {
	m_curr_proc_id = 0;
	m_curr_stmt_id = 0;
}


void DesignExtractor::start_nesting() {
	m_curr_nesting_stk.push(m_curr_stmt_id);
}


void DesignExtractor::end_nesting() {
	if (m_curr_nesting_stk.empty()) {
		throw std::runtime_error("Failed to end nesting. Current stmt : " + m_curr_stmt_id);
	}
	else {
		m_curr_nesting_stk.pop();
	}
}


void DesignExtractor::add_procedure(proc_name name) {
	Procedure proc(name, m_curr_proc_id + 1);
	m_curr_proc_id++;
	m_procedures.push_back(proc);
}


void DesignExtractor::add_statement(TokenType token_type) {
	StmtType stmt_type;

	switch (token_type) {
	case TokenType::READ:
		stmt_type = StmtType::STMT_READ;
		break;
	case TokenType::PRINT:
		stmt_type = StmtType::STMT_PRINT;
		break;
	case TokenType::CALL:
		stmt_type = StmtType::STMT_CALL;
		break;
	case TokenType::WHILE:
		stmt_type = StmtType::STMT_WHILE;
		break;
	case TokenType::IF:
		stmt_type = StmtType::STMT_IF;
		break;
	case TokenType::ASSIGN:
		stmt_type = StmtType::STMT_ASSIGN;
		break;
	default:
		throw std::runtime_error("Unknown TokenType to StmtType.");
		break;
	}


	Statement stmt(m_curr_stmt_id + 1, stmt_type, m_procedures[m_curr_proc_id - 1].get_name());
	m_curr_stmt_id++;
	m_statements.push_back(stmt);
}


void DesignExtractor::add_variable(var_name name) {
	m_variables.insert(name);
}


void DesignExtractor::add_constant(constant val) {
	m_constants.insert(val);
}



void DesignExtractor::add_statement_uses(var_name name) {

}


void DesignExtractor::add_statement_modifies(var_name name) {


}


void DesignExtractor::start_expr() {
	m_expr_builder = "";
}


void DesignExtractor::end_expr() {
	m_statements[m_curr_stmt_id - 1].set_expr_str(m_expr_builder);
	m_expr_builder.erase();
}



void DesignExtractor::populateEntities() {

	for (Procedure p : m_procedures) {
		PKB::getInstance().addProcedure(p.get_name());
	}

	for (Statement s : m_statements) {
		PKB::getInstance().addStmt(s.get_type());
	}

	for (var_name v : m_variables) {
		PKB::getInstance().addVariable(v);
	}

	for (constant c : m_constants) {
		PKB::getInstance().addConstant(c);
	}
}


void DesignExtractor::populateRelations() {

}



void DesignExtractor::populateFollows() {

}


void DesignExtractor::populateFollowsT() {

}


void DesignExtractor::populateParent() {

}


void DesignExtractor::populateParentT() {

}


void DesignExtractor::populateUses() {

}


void DesignExtractor::populateModifies() {

}
