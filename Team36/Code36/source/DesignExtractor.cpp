#include <stdexcept>
#include <unordered_map>
#include <map>
#include <algorithm>
#include <numeric>
#include "DesignExtractor.h"

using namespace SourceProcessor;

DesignExtractor::DesignExtractor(PKBSourceInterface& instance) : Extractor(instance) {
	curr_proc_id = DE_INIT_PROC_NO;
	curr_stmt_id = DE_INIT_STMT_NO;
	curr_stmt_list_id = DE_INIT_STMT_LIST_NO;
	is_cond_expr = false;
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

	curr_stmt_list_id = de_statements[curr_nesting_stk.top() - OFFSET]->getStmtList();
	curr_nesting_stk.pop();
}

void DesignExtractor::setCondExpr(bool flag) {
	is_cond_expr = flag;
}

void DesignExtractor::addProcedure(proc_name name) {
	curr_proc_id++;

	Procedure* proc = new Procedure(name, curr_proc_id);
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
	Statement* stmt = new Statement(curr_stmt_id, stmt_type, de_procedures[curr_proc_id - OFFSET]->getName(), curr_stmt_list_id);

	if (!curr_nesting_stk.empty() && curr_nesting_stk.top() != curr_stmt_id) {
		stmt->setDirectParent(curr_nesting_stk.top());
		de_statements[curr_nesting_stk.top() - OFFSET]->addDirectChild(stmt->getIndex());
	}

	de_statements.push_back(stmt);
	de_procedures[curr_proc_id - OFFSET]->addChild(curr_stmt_id);
}

void DesignExtractor::addVariable(var_name name) {
	de_variables.insert(name);
}

void DesignExtractor::addConstant(constant val) {
	de_constants.insert(val);
}

const std::vector<Procedure*>& DesignExtractor::getProcedures() const {
	return de_procedures;
}

const std::vector<Statement*>& DesignExtractor::getStatements() const {
	return de_statements;
}

const std::unordered_set<var_name>& DesignExtractor::getVariables() const {
	return de_variables;
}

const std::unordered_set<constant>& DesignExtractor::getConstants() const {
	return de_constants;
}

void DesignExtractor::addStatementUses(var_name name) {
	de_statements[curr_stmt_id - OFFSET]->addUsesVariable(name);

	if (is_cond_expr) {
		de_statements[curr_stmt_id - OFFSET]->addUsesCondVariable(name);
	}

	stmt_index curr = curr_stmt_id;
	stmt_index parent = de_statements[curr - OFFSET]->getDirectParent();
	while (parent) {
		de_statements[parent - OFFSET]->addUsesVariable(name);
		curr = parent;
		parent = de_statements[curr - OFFSET]->getDirectParent();
	}

	de_procedures[curr_proc_id - OFFSET]->addUsesVariable(name);
}

void DesignExtractor::addStatementModifies(var_name name) {
	de_statements[curr_stmt_id - OFFSET]->addModifiesVariable(name);

	stmt_index curr = curr_stmt_id;
	stmt_index parent = de_statements[curr - OFFSET]->getDirectParent();
	while (parent) {
		de_statements[parent - OFFSET]->addModifiesVariable(name);
		curr = parent;
		parent = de_statements[curr - OFFSET]->getDirectParent();
	}

	de_procedures[curr_proc_id - OFFSET]->addModifiesVariable(name);
}

void DesignExtractor::startExpr() {
	expr_builder = std::string();
}

void DesignExtractor::addExprSegment(std::string segment) {
	if (de_statements[curr_stmt_id - OFFSET]->getType() == StmtType::STMT_ASSIGN) {
		expr_builder.append(segment);
	}
}

void DesignExtractor::endExpr() {
	de_statements[curr_stmt_id - OFFSET]->setExprStr(expr_builder);
	expr_builder.erase();
}

void DesignExtractor::addCallee(proc_name name) {
	de_statements[curr_stmt_id - OFFSET]->setCallee(name);
}

void DesignExtractor::validate() {
	// Validate procedure names
	for (Procedure* p : de_procedures) {
		auto result = proc_name_to_id.insert({ p->getName(), p->getIndex() });
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
				call_cache.push_back({ proc_name_to_id[s->getProcName()], proc_name_to_id[callee] });
			}
			else {
				throw std::runtime_error("Call statement calls undefined procedure.");
			}
		}
	}

	// Validate cyclic calls
	const int INIT = 0;
	const int DISQUALIFIED = -1;
	std::vector<int> all_procedures(de_procedures.size(), INIT);

	for (auto call : call_cache) {
		all_procedures[call.first - OFFSET]++;
	}

	while (call_sequence.size() != all_procedures.size()) {
		proc_index filtered_id = NON_EXISTING_PROC_NO;

		for (size_t i = 0; i < all_procedures.size(); i++) {
			if (all_procedures[i] == INIT) {
				filtered_id = i + OFFSET;
				call_sequence.push_back(filtered_id);
				call_cache.erase(std::remove_if(call_cache.begin(), call_cache.end(),
					[filtered_id, &all_procedures](std::pair<proc_index, proc_index> call) {
						if (call.second == filtered_id) {
							all_procedures[call.first - OFFSET]--;
						}
						return call.second == filtered_id;
					}), call_cache.end());
				all_procedures[i] = DISQUALIFIED;
				break;
			}
		}

		if (filtered_id == NON_EXISTING_PROC_NO) {
			throw std::runtime_error("Cyclic procedure calls detected.");
		}
	}
}

void DesignExtractor::populatePostValidation() {
	// Procedure uses and modifies, populate call statements too
	for (proc_index proc : call_sequence) {
		Procedure* p = de_procedures[proc - OFFSET];

		auto child_stmt = de_procedures[proc - OFFSET]->getChild();
		for (int i = child_stmt.size() - 1; i >= 0; i--) {
			Statement* s = de_statements[child_stmt[i] - OFFSET];

			if (s->getType() == StmtType::STMT_CALL) {
				Procedure* callee = de_procedures[proc_name_to_id[s->getCallee()] - OFFSET];
				for (auto used_var : callee->getUsedVariable()) {
					s->addUsesVariable(used_var);
				}
				for (auto modified_var : callee->getModifiedVariable()) {
					s->addModifiesVariable(modified_var);
				}
			}

			if (s->getDirectParent() != STMT_DEFAULT_DIRECT_PARENT) {
				Statement* parent = de_statements[s->getDirectParent() - OFFSET];
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

void DesignExtractor::populateEntities() {
	populateProcedures();
	populateStatements();
	populateVariables();
	populateConstants();
}

void DesignExtractor::populateRelations() {
	populateFollows();
	pkb_instance.generateFollowsT();
	populateParent();
	pkb_instance.generateParentT();
	populateUses();
	populateModifies();
	populateCalls();
	pkb_instance.generateCallsPT();
	populateIfs();
	populateWhiles();
	populateNext();
}

void DesignExtractor::populateProcedures() {
	for (Procedure* p : de_procedures) {
		pkb_instance.addProcedure(p->getName());

		for (stmt_index id : p->getChild()) {
			pkb_instance.addProcContains(p->getName(), id);
		}
	}
}

void DesignExtractor::populateStatements() {
	ExprParser expr_parser;
	for (Statement* s : de_statements) {
		pkb_instance.addStmt(s->getType());

		if (s->getType() == StmtType::STMT_ASSIGN) {
			pkb_instance.addExprTree(s->getIndex(), expr_parser.parse(s->getExprStr()));
		}
	}
}

void DesignExtractor::populateVariables() {
	for (var_name v : de_variables) {
		pkb_instance.addVariable(v);
	}
}

void DesignExtractor::populateConstants() {
	for (constant c : de_constants) {
		pkb_instance.addConstant(c);
	}
}

void DesignExtractor::populateFollows() {
	std::unordered_map<int, stmt_index> um;

	for (Statement* s : de_statements) {
		int list_id = s->getStmtList();
		if (um.find(list_id) != um.end()) {
			pkb_instance.addFollows(um[list_id], s->getIndex());
		}
		um[list_id] = s->getIndex();
	}
}

void DesignExtractor::populateParent() {
	for (Statement* s : de_statements) {
		for (stmt_index id : s->getDirectChild()) {
			pkb_instance.addParent(s->getIndex(), id);
		}
	}
}

void DesignExtractor::populateUses() {
	for (Statement* s : de_statements) {
		for (var_name used_var : s->getUsedVariable()) {
			pkb_instance.addUsesS(s->getIndex(), used_var);
		}
	}

	for (Procedure* p : de_procedures) {
		for (var_name used_var : p->getUsedVariable()) {
			pkb_instance.addUsesP(p->getName(), used_var);
		}
	}
}

void DesignExtractor::populateModifies() {
	for (Statement* s : de_statements) {
		for (var_name modified_var : s->getModifiedVariable()) {
			pkb_instance.addModifiesS(s->getIndex(), modified_var);
		}
	}

	for (Procedure* p : de_procedures) {
		for (var_name modified_var : p->getModifiedVariable()) {
			pkb_instance.addModifiesP(p->getName(), modified_var);
		}
	}
}

void DesignExtractor::populateCalls() {
	for (Statement* s : de_statements) {
		if (s->getType() == StmtType::STMT_CALL) {
			pkb_instance.addCallsS(s->getIndex(), s->getCallee());
			pkb_instance.addCallsP(s->getProcName(), s->getCallee());
		}
	}
}

void DesignExtractor::populateNext() {

	std::vector<CFG*> cfgs;
	for (Procedure* p : de_procedures) {
		CFG* cfg = generateCFG(p->getChild());
		std::vector<std::pair<prog_line, prog_line>> nexts = cfg->getNexts();
		for (auto next_rel : nexts) {
			pkb_instance.addNext(next_rel.first, next_rel.second);
		}
		cfgs.push_back(cfg);
	}

	for (proc_index p : call_sequence) {
		for (stmt_index s: de_procedures[p - OFFSET]->getChild()) {
			Statement* stmt = de_statements[s - OFFSET];
			if (stmt->getType() == StmtType::STMT_CALL) {
				cfgs[p - OFFSET]->call(cfgs[proc_name_to_id[stmt->getCallee()] - OFFSET], s);
			}
		}
	}

	for (CFG* cfg: cfgs) {
		if (cfg->getHeadLabelledProgLine().labels.front() == NON_EXISTING_STMT_NO) {
			pkb_instance.addCFGBip(cfg);
		}
	}

	pkb_instance.addCFGsToDestroy(cfgs);
}

void DesignExtractor::populateIfs() {
	for (Statement* s : de_statements) {
		if (s->getType() == StmtType::STMT_IF) {
			for (var_name v : s->getUsedCondVariable()) {
				pkb_instance.addIf(s->getIndex(), v);
			}
		}
	}
}

void DesignExtractor::populateWhiles() {
	for (Statement* s : de_statements) {
		if (s->getType() == StmtType::STMT_WHILE) {
			for (var_name v : s->getUsedCondVariable()) {
				pkb_instance.addWhile(s->getIndex(), v);
			}
		}
	}
}

CFG* DesignExtractor::generateCFG(std::vector<stmt_index> indexes) {
	if (indexes.size() == 0) {
		return nullptr;
	}

	CFG* cfg = new CFG();

	stmt_index parent = de_statements[indexes.front() - OFFSET]->getDirectParent();
	std::vector<stmt_index> main;
	for (stmt_index id: indexes) {
		if (de_statements[id - OFFSET]->getDirectParent() == parent 
			&& de_statements[id - OFFSET]->getStmtList() == indexes.front()) {
			main.push_back(id);
		}
	}

	for (stmt_index id : main) {
		cfg->add(id);
	}

	for (size_t i = 0; i < main.size(); i++) {
		prog_line curr = main[i], next;
		if (i == main.size() - 1) {
			next = indexes.back() + 1;
		}
		else {
			next = main[i + 1];
		}

		if (next - curr <= 1) {
			continue;
		}

		Statement* container = de_statements[curr - OFFSET];
		if (container->getType() == StmtType::STMT_IF) {
			std::vector<stmt_index> children(container->getDirectChild());
			std::set<stmt_index> split;
			for (stmt_index child_id : children) {
				split.insert(de_statements[child_id - OFFSET]->getStmtList());
			}

			if (split.size() != IF_STMT_BRANCH_NO) {
				throw std::runtime_error("CFG build failure. If statement has incorrect number of branches.");
			}

			stmt_index then_start = curr + 1;
			split.erase(then_start);
			stmt_index else_start = *split.begin();

			std::vector<stmt_index> then_scope(else_start - then_start);
			std::iota(then_scope.begin(), then_scope.end(), then_start);
			CFG* cfg_then = generateCFG(then_scope);

			std::vector<stmt_index> else_scope(next - else_start);
			std::iota(else_scope.begin(), else_scope.end(), else_start);
			CFG* cfg_else = generateCFG(else_scope);

			cfg->fork(cfg_then, cfg_else, curr);

			delete cfg_then, cfg_else;
		}
		else if (container->getType() == StmtType::STMT_WHILE) {
			std::vector<stmt_index> scope(next - curr - 1);
			std::iota(scope.begin(), scope.end(), curr + 1);
			CFG* cfg_while = generateCFG(scope);

			cfg->loop(cfg_while, curr);

			delete cfg_while;
		}
		else {
			throw std::runtime_error("CFG build failure. Gap involved non-container statement.");
		}
	}

	return cfg;
}
