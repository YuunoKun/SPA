#include <stdexcept>
#include <unordered_map>
#include <algorithm>
#include "DesignExtractor.h"
#include "PKB.h"

using namespace SourceProcessor;


DesignExtractor::DesignExtractor() {
	curr_proc_id = 0;
	curr_stmt_id = 0;
	curr_stmt_list_id = 1;
}


DesignExtractor::~DesignExtractor() {
	for (Procedure* p : de_procedures) {
		delete p;
	}

	for (Statement* s : de_statements) {
		delete s;
	}
}


void DesignExtractor::startNesting() {
	curr_nesting_stk.push(curr_stmt_id);
	curr_stmt_list_id = curr_stmt_id + 1;
}


void DesignExtractor::chopNesting() {
	curr_stmt_list_id = curr_stmt_id + 1;
}


void DesignExtractor::endNesting() {
	if (curr_nesting_stk.empty()) {
		throw std::runtime_error("Failed to end nesting. Current stmt : " + curr_stmt_id);
	}
	
	curr_stmt_list_id = de_statements[curr_nesting_stk.top() - 1]->getStmtList();
	curr_nesting_stk.pop();
}


void DesignExtractor::addProcedure(proc_name name) {
	curr_proc_id++;

	Procedure *proc = new Procedure(name, curr_proc_id);
	curr_stmt_list_id = curr_stmt_id + 1;
	de_procedures.push_back(proc);
}


void DesignExtractor::addStatement(TokenType token_type) {
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


	curr_stmt_id++;
	Statement *stmt = new Statement(curr_stmt_id, stmt_type, de_procedures[curr_proc_id - 1]->getName(), curr_stmt_list_id);
	
	if (!curr_nesting_stk.empty() && curr_nesting_stk.top() != curr_stmt_id) {
		stmt->setDirectParent(curr_nesting_stk.top());
		de_statements[curr_nesting_stk.top() - 1]->addDirectChild(stmt->getIndex());
	}

	de_statements.push_back(stmt);
	de_procedures[curr_proc_id - 1]->addChild(curr_stmt_id);
}


void DesignExtractor::addVariable(var_name name) {
	de_variables.insert(name);
}


void DesignExtractor::addConstant(constant val) {
	de_constants.insert(val);
}


const std::vector<Procedure*>& DesignExtractor::getProcedures() {
	return de_procedures;
}


const std::vector<Statement*>& DesignExtractor::getStatements() {
	return de_statements;
}


const std::unordered_set<var_name>& DesignExtractor::getVariables() {
	return de_variables;
}


const std::unordered_set<constant>& DesignExtractor::getConstants() {
	return de_constants;
}



void DesignExtractor::addStatementUses(var_name name) {
	de_statements[curr_stmt_id - 1]->addUsesVariable(name);

	stmt_index curr = curr_stmt_id;
	stmt_index parent = de_statements[curr - 1]->getDirectParent();
	while (parent) {
		de_statements[parent - 1]->addUsesVariable(name);
		curr = parent;
		parent = de_statements[curr - 1]->getDirectParent();
	}

	de_procedures[curr_proc_id - 1]->addUsesVariable(name);
}


void DesignExtractor::addStatementModifies(var_name name) {
	de_statements[curr_stmt_id - 1]->addModifiesVariable(name);

	stmt_index curr = curr_stmt_id;
	stmt_index parent = de_statements[curr - 1]->getDirectParent();
	while (parent) {
		de_statements[parent - 1]->addModifiesVariable(name);
		curr = parent;
		parent = de_statements[curr - 1]->getDirectParent();
	}

	de_procedures[curr_proc_id - 1]->addModifiesVariable(name);
}


void DesignExtractor::startExpr() {
	expr_builder = "";
}


void DesignExtractor::addExprSegment(std::string segment) {
	if (de_statements[curr_stmt_id - 1]->getType() == StmtType::STMT_ASSIGN) {
		expr_builder.append(segment);
	}
}


void DesignExtractor::endExpr() {
	de_statements[curr_stmt_id - 1]->setExprStr(expr_builder);
	expr_builder.erase();
}


void DesignExtractor::addCallee(proc_name name) {
	de_statements[curr_stmt_id - 1]->setCallee(name);
}


void DesignExtractor::validate() {
	// Validate procedure names
	for (Procedure *p: de_procedures) {
		auto result = proc_name_to_id.insert({p->getName(), p->getIndex()});
		if (!result.second) {
			throw std::runtime_error("Duplicate procedure names detected.");
		}
	}

	// Validate statement callee
	std::vector<std::pair<proc_index, proc_index>> call_cache;
	for (Statement* s : de_statements) {
		if (s->getType() == StmtType::STMT_CALL) {
			proc_name callee = s->getCallee();
			if (proc_name_to_id.find(callee) != proc_name_to_id.end()) {
				call_cache.push_back({ proc_name_to_id[s->getProcName()], proc_name_to_id [callee]});
			}
			else {
				throw std::runtime_error("Call statement calls undefined procedure.");
			}
		}
	}

	// Validate cyclic calls
	std::vector<proc_index> all_procedures(de_procedures.size(), 0);

	for (auto call : call_cache) {
		all_procedures[call.first - 1]++;
	}

	while (call_sequence.size() != all_procedures.size()) {
		proc_index filtered_id = 0;

		for (int i = 0; i < all_procedures.size(); i++) {
			if (all_procedures[i] == 0) {
				filtered_id = i + 1;
				call_sequence.push_back(filtered_id);
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

void DesignExtractor::populatePostValidation() {
	// Procedure uses and modifies, populate call statements too
	for (proc_index proc : call_sequence) {
		Procedure* p = de_procedures[proc - 1];

		auto child_stmt = de_procedures[proc - 1]->getChild();
		for (int i = child_stmt.size() - 1; i >= 0; i--) {
			Statement* s = de_statements[child_stmt[i] - 1];

			if (s->getType() == StmtType::STMT_CALL) {
				Procedure* callee = de_procedures[proc_name_to_id[s->getCallee()] - 1];
				for (auto used_var : callee->getUsedVariable()) {
					s->addUsesVariable(used_var);
				}
				for (auto modified_var : callee->getModifiedVariable()) {
					s->addModifiesVariable(modified_var);
				}
			}

			if(s->getDirectParent() != 0) {
				Statement* parent = de_statements[s->getDirectParent() - 1];
				for (auto used_var : s->getUsedVariable()) {
					parent->addUsesVariable(used_var);
				}
				for (auto modified_var : s->getModifiedVariable()) {
					parent->addModifiesVariable(modified_var);
				}
			}

			for (auto used_var : s->getUsedVariable()) {
				p->addUsesVariable(used_var);
			}
			for (auto modified_var : s->getModifiedVariable()) {
				p->addModifiesVariable(modified_var);
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
	for (Procedure* p : de_procedures) {
		pkb.addProcedure(p->getName());
	}
}


void DesignExtractor::populateStatements(PKB& pkb) {
	for (Statement* s : de_statements) {
		pkb.addStmt(s->getType());

		if (s->getType() == StmtType::STMT_ASSIGN) {
			pkb.addExprTree(s->getIndex(), s->getExprStr());
		}
	}
}


void DesignExtractor::populateVariables(PKB& pkb) {
	for (var_name v : de_variables) {
		pkb.addVariable(v);
	}
}


void DesignExtractor::populateConstants(PKB& pkb) {
	for (constant c : de_constants) {
		pkb.addConstant(c);
	}
}



void DesignExtractor::populateFollows(PKB& pkb) {
	std::unordered_map<int, stmt_index> um;

	for (Statement* s : de_statements) {
		int list_id = s->getStmtList();
		if (um.find(list_id) != um.end()) {
			pkb.addFollows(um[list_id], s->getIndex());
		}
		um[list_id] = s->getIndex();
	}
}


void DesignExtractor::populateParent(PKB& pkb) {
	for (Statement* s : de_statements) {
		for (stmt_index id : s->getDirectChild()) {
			pkb.addParent(s->getIndex(), id);
		}
	}
}


void DesignExtractor::populateUses(PKB& pkb) {
	for (Statement* s : de_statements) {
		for (var_name used_var : s->getUsedVariable()) {
			pkb.addUsesS(s->getIndex(), used_var);
		}
	}

	for (Procedure* p : de_procedures) {
		for (var_name used_var : p->getUsedVariable()) {
			//pkb.addUsesP(p->getIndex(), used_var);
		}
	}
}


void DesignExtractor::populateModifies(PKB& pkb) {
	for (Statement* s : de_statements) {
		for (var_name modified_var : s->getModifiedVariable()) {
			pkb.addModifiesS(s->getIndex(), modified_var);
		}
	}

	for (Procedure* p : de_procedures) {
		for (var_name modified_var : p->getModifiedVariable()) {
			//pkb.addModifiesP(p->getIndex(), modified_var);
		}
	}
}
