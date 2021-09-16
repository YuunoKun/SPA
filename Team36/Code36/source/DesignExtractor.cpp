#include <stdexcept>
#include <unordered_map>
#include <algorithm>
#include "DesignExtractor.h"
#include "PKB.h"

using namespace SourceProcessor;


DesignExtractor::DesignExtractor() {
	m_curr_proc_id = 0;
	m_curr_stmt_id = 0;
	m_stmt_list_id = 1;
}


DesignExtractor::~DesignExtractor() {
	for (Procedure* p : m_procedures) {
		delete p;
	}

	for (Statement* s : m_statements) {
		delete s;
	}
}


void DesignExtractor::start_nesting() {
	m_curr_nesting_stk.push(m_curr_stmt_id);
	m_stmt_list_id = m_curr_stmt_id + 1;
}


void DesignExtractor::chop_nesting() {
	m_stmt_list_id = m_curr_stmt_id + 1;
}


void DesignExtractor::end_nesting() {
	if (m_curr_nesting_stk.empty()) {
		throw std::runtime_error("Failed to end nesting. Current stmt : " + m_curr_stmt_id);
	}
	
	m_stmt_list_id = m_statements[m_curr_nesting_stk.top() - 1]->get_stmt_list();
	m_curr_nesting_stk.pop();
}


void DesignExtractor::add_procedure(proc_name name) {
	m_curr_proc_id++;

	Procedure *proc = new Procedure(name, m_curr_proc_id);
	m_stmt_list_id = m_curr_stmt_id + 1;
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


	m_curr_stmt_id++;
	Statement *stmt = new Statement(m_curr_stmt_id, stmt_type, m_procedures[m_curr_proc_id - 1]->get_name(), m_stmt_list_id);
	
	if (!m_curr_nesting_stk.empty() && m_curr_nesting_stk.top() != m_curr_stmt_id) {
		stmt->set_direct_parent(m_curr_nesting_stk.top());
		m_statements[m_curr_nesting_stk.top() - 1]->add_direct_child(stmt->get_index());
	}

	m_statements.push_back(stmt);
	m_procedures[m_curr_proc_id - 1]->add_child(m_curr_stmt_id);
}


void DesignExtractor::add_variable(var_name name) {
	m_variables.insert(name);
}


void DesignExtractor::add_constant(constant val) {
	m_constants.insert(val);
}


const std::vector<Procedure*>& DesignExtractor::get_procedures() {
	return m_procedures;
}


const std::vector<Statement*>& DesignExtractor::get_statements() {
	return m_statements;
}


const std::unordered_set<var_name>& DesignExtractor::get_variables() {
	return m_variables;
}


const std::unordered_set<constant>& DesignExtractor::get_constants() {
	return m_constants;
}



void DesignExtractor::add_statement_uses(var_name name) {
	m_statements[m_curr_stmt_id - 1]->add_uses_variable(name);

	stmt_index curr = m_curr_stmt_id;
	stmt_index parent = m_statements[curr - 1]->get_direct_parent();
	while (parent) {
		m_statements[parent - 1]->add_uses_variable(name);
		curr = parent;
		parent = m_statements[curr - 1]->get_direct_parent();
	}

	m_procedures[m_curr_proc_id - 1]->add_uses_variable(name);
}


void DesignExtractor::add_statement_modifies(var_name name) {
	m_statements[m_curr_stmt_id - 1]->add_modifies_variable(name);

	stmt_index curr = m_curr_stmt_id;
	stmt_index parent = m_statements[curr - 1]->get_direct_parent();
	while (parent) {
		m_statements[parent - 1]->add_modifies_variable(name);
		curr = parent;
		parent = m_statements[curr - 1]->get_direct_parent();
	}

	m_procedures[m_curr_proc_id - 1]->add_modifies_variable(name);
}


void DesignExtractor::start_expr() {
	m_expr_builder = "";
}


void DesignExtractor::add_expr_segment(std::string segment) {
	if (m_statements[m_curr_stmt_id - 1]->get_type() == StmtType::STMT_ASSIGN) {
		m_expr_builder.append(segment);
	}
}


void DesignExtractor::end_expr() {
	m_statements[m_curr_stmt_id - 1]->set_expr_str(m_expr_builder);
	m_expr_builder.erase();
}


void DesignExtractor::add_callee(proc_name name) {
	m_statements[m_curr_stmt_id - 1]->set_callee(name);
}


void DesignExtractor::validate() {
	// Validate procedure names
	for (Procedure *p: m_procedures) {
		auto result = m_proc_name_to_id.insert({p->get_name(), p->get_index()});
		if (!result.second) {
			throw std::runtime_error("Duplicate procedure names detected.");
		}
	}

	// Validate statement callee
	std::vector<std::pair<proc_index, proc_index>> call_cache;
	for (Statement* s : m_statements) {
		if (s->get_type() == StmtType::STMT_CALL) {
			proc_name callee = s->get_callee();
			if (m_proc_name_to_id.find(callee) != m_proc_name_to_id.end()) {
				call_cache.push_back({ m_proc_name_to_id[s->get_proc_name()], m_proc_name_to_id [callee]});
			}
			else {
				throw std::runtime_error("Call statement calls undefined procedure.");
			}
		}
	}

	// Validate cyclic calls
	std::vector<proc_index> all_procedures(m_procedures.size(), 0);

	for (auto call : call_cache) {
		all_procedures[call.first - 1]++;
	}

	while (m_call_sequence.size() != all_procedures.size()) {
		proc_index filtered_id = 0;

		for (int i = 0; i < all_procedures.size(); i++) {
			if (all_procedures[i] == 0) {
				filtered_id = i + 1;
				m_call_sequence.push_back(filtered_id);
				call_cache.erase(std::remove_if(call_cache.begin(), call_cache.end(),
					[filtered_id, &all_procedures](std::pair<proc_index, proc_index> call) {
						if (call.second == filtered_id) {
							all_procedures[call.first - 1]--;
						}
						return call.second == filtered_id;
					}), call_cache.end());
				all_procedures[i] = -1;
				break;
			}
		}

		if (filtered_id == 0) {
			throw std::runtime_error("Cyclic procedure calls detected.");
		}
	}
}

void DesignExtractor::populate_post_validation() {
	// Procedure uses and modifies, populate call statements too
	for (proc_index proc : m_call_sequence) {
		Procedure* p = m_procedures[proc - 1];

		auto child_stmt = m_procedures[proc - 1]->get_child();
		for (int i = child_stmt.size() - 1; i >= 0; i--) {
			Statement* s = m_statements[child_stmt[i] - 1];

			if (s->get_type() == StmtType::STMT_CALL) {
				Procedure* callee = m_procedures[m_proc_name_to_id[s->get_callee()] - 1];
				for (auto used_var : callee->get_used_variable()) {
					s->add_uses_variable(used_var);
				}
				for (auto modified_var : callee->get_modified_variable()) {
					s->add_modifies_variable(modified_var);
				}
			}

			if(s->get_direct_parent() != 0) {
				Statement* parent = m_statements[s->get_direct_parent() - 1];
				for (auto used_var : s->get_used_variable()) {
					parent->add_uses_variable(used_var);
				}
				for (auto modified_var : s->get_modified_variable()) {
					parent->add_modifies_variable(modified_var);
				}
			}

			for (auto used_var : s->get_used_variable()) {
				p->add_uses_variable(used_var);
			}
			for (auto modified_var : s->get_modified_variable()) {
				p->add_modifies_variable(modified_var);
			}
		}
	}
}


void DesignExtractor::populateEntities(PKB& pkb) {
	populateProcedures(pkb);
	populateStatements(pkb);
	populateVariables(pkb);
	populateConstants(pkb);
}


void DesignExtractor::populateRelations(PKB& pkb) {
	populateFollows(pkb);
	pkb.generateFollowsT();
	populateParent(pkb);
	pkb.generateParentT();
	populateUses(pkb);
	populateModifies(pkb);
}


void DesignExtractor::populateProcedures(PKB& pkb) {
	for (Procedure* p : m_procedures) {
		pkb.addProcedure(p->get_name());
	}
}


void DesignExtractor::populateStatements(PKB& pkb) {
	for (Statement* s : m_statements) {
		pkb.addStmt(s->get_type());

		if (s->get_type() == StmtType::STMT_ASSIGN) {
			pkb.addExprTree(s->get_index(), s->get_expr_str());
		}
	}
}


void DesignExtractor::populateVariables(PKB& pkb) {
	for (var_name v : m_variables) {
		pkb.addVariable(v);
	}
}


void DesignExtractor::populateConstants(PKB& pkb) {
	for (constant c : m_constants) {
		pkb.addConstant(c);
	}
}



void DesignExtractor::populateFollows(PKB& pkb) {
	std::unordered_map<int, stmt_index> um;

	for (Statement* s : m_statements) {
		int list_id = s->get_stmt_list();
		if (um.find(list_id) != um.end()) {
			pkb.addFollows(um[list_id], s->get_index());
		}
		um[list_id] = s->get_index();
	}
}


void DesignExtractor::populateParent(PKB& pkb) {
	for (Statement* s : m_statements) {
		for (stmt_index id : s->get_direct_child()) {
			pkb.addParent(s->get_index(), id);
		}
	}
}


void DesignExtractor::populateUses(PKB& pkb) {
	for (Statement* s : m_statements) {
		for (var_name used_var : s->get_used_variable()) {
			pkb.addUsesS(s->get_index(), used_var);
		}
	}

	for (Procedure* p : m_procedures) {
		for (var_name used_var : p->get_used_variable()) {
			//pkb.addUsesP(p->get_index(), used_var);
		}
	}
}


void DesignExtractor::populateModifies(PKB& pkb) {
	for (Statement* s : m_statements) {
		for (var_name modified_var : s->get_modified_variable()) {
			pkb.addModifiesS(s->get_index(), modified_var);
		}
	}

	for (Procedure* p : m_procedures) {
		for (var_name modified_var : p->get_modified_variable()) {
			//pkb.addModifiesP(p->get_index(), modified_var);
		}
	}
}
