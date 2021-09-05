#include <stdexcept>
#include "DesignExtractor.h"


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


void DesignExtractor::add_statement(StmtType type) {
	Statement stmt(m_curr_stmt_id + 1, type, m_procedures[m_curr_proc_id - 1].get_name());
}


void DesignExtractor::add_statement_uses(var_name name);

void DesignExtractor::add_statement_modifies(var_name name);

void DesignExtractor::start_expr() {

}


void DesignExtractor::end_expr() {

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
