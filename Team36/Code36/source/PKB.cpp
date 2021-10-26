#include<stdio.h>

#include <iostream>
#include <string>

#include "PKB.h"
#include "Common.h"
#include "RelationTable.h"
// need to include this .cpp for template classes
#include "RelationTable.cpp"
#include "RelationsUtility.cpp"

PKB& PKB::getInstance() {
	static PKB pkb;
	return pkb;
}

void PKB::addConstant(constant constant) {
	const_table.emplace(constant);
	return;
}

void PKB::addProcedure(proc_name proc_name) {
	for (auto const& element : proc_table) {
		if (element == proc_name) {
			return;
		}
	};
	proc_table.push_back(proc_name);
}

void PKB::addVariable(var_name var_name) {
	for (auto const& element : var_table) {
		if (element == var_name) {
			return;
		}
	};
	var_table.push_back(var_name);
}

void PKB::addStmt(StmtType stmt_type) {
	stmt_index stmt_index = stmt_table.size() + 1;
	StmtInfo new_stmt_info{ stmt_index, stmt_type };
	stmt_table.push_back(new_stmt_info);
}

void PKB::addExprTree(stmt_index stmt_index, expr expr) {
	if (stmt_index <= 0) {
		throw std::invalid_argument("Stmt index must be greater than zero: " + std::to_string(stmt_index));
	}
	else if (stmt_index > stmt_table.size()) {
		throw std::invalid_argument("Invalid stmt index: " + std::to_string(stmt_index));
	}
	else if (stmt_table[stmt_index - 1].stmt_type != STMT_ASSIGN) {
		throw std::invalid_argument("Stmt index does not belong to an assignment statement: " + std::to_string(stmt_index));
	}
	expr_table.insert({ stmt_index, expr });
}

void PKB::addParent(stmt_index parent, stmt_index child) {
	try {
		StmtInfo parent_stmt_info{ parent, stmt_table.at(parent - 1).stmt_type };
		StmtInfo child_stmt_info{ child, stmt_table.at(child - 1).stmt_type };
		StmtType parent_stmt_type = stmt_table[parent - 1].stmt_type;
		if (parent_stmt_type != STMT_WHILE && parent_stmt_type != STMT_IF) {
			throw std::invalid_argument("Parent stmt index does not belong to an while/if statement: " + std::to_string(parent));
		}
		parent_table.insert(parent_stmt_info, child_stmt_info);
	}
	catch (std::out_of_range&) {
		throw std::invalid_argument("addParent: Invalid stmt indexes: [" + std::to_string(parent)
			+ "," + std::to_string(child) + "]");
	}
}

void PKB::addFollows(stmt_index first, stmt_index second) {
	try {
		StmtInfo first_stmt_info{ first, stmt_table.at(first - 1).stmt_type };
		StmtInfo second_stmt_info{ second, stmt_table.at(second - 1).stmt_type };
		follows_table.insert(first_stmt_info, second_stmt_info);
	}
	catch (std::out_of_range&) {
		throw std::invalid_argument("addFollows: Invalid stmt indexes: [" + std::to_string(first)
			+ "," + std::to_string(second) + "]");
	}
}

void PKB::addUsesS(stmt_index user, var_name used) {
	try {
		std::vector<var_name>::iterator it = std::find(var_table.begin(), var_table.end(), used);
		if (it != var_table.end()) {
			StmtType user_stmt_type = stmt_table[user - 1].stmt_type;
			StmtInfo user_stmt_info{ user, user_stmt_type };
			usesS_table.insert(user_stmt_info, used);
			if (user_stmt_type == STMT_PRINT) {
				print_table.insert(user, used);
			}
		}
		else {
			throw std::invalid_argument("addUsesS: Invalid var name: " + used);
		}
	}
	catch (std::out_of_range&) {
		throw std::invalid_argument("addUsesS: Invalid stmt index:" + std::to_string(user));
	}
}

void PKB::addModifiesS(stmt_index modifier, var_name modified) {
	try {
		StmtInfo modifier_stmt_info{ modifier, stmt_table.at(modifier - 1).stmt_type };
		std::vector<var_name>::iterator it = std::find(var_table.begin(), var_table.end(), modified);
		if (it != var_table.end()) {
			modifiesS_table.insert(modifier_stmt_info, modified);
			StmtType modifier_stmt_type = stmt_table[modifier - 1].stmt_type;
			if (modifier_stmt_type == STMT_ASSIGN) {
				assignment_table.insert(modifier, modified);
			}
			else if (modifier_stmt_type == STMT_READ) {
				read_table.insert(modifier, modified);
			}
		}
		else {
			throw std::invalid_argument("addModifiesS: Invalid var name: " + modified);
		}
	}
	catch (std::out_of_range&) {
		throw std::invalid_argument("addModifiesS: Invalid stmt index:" + std::to_string(modifier));
	}
}

void PKB::addModifiesP(proc_name proc, var_name modified) {
	std::vector<var_name>::iterator it_var = std::find(var_table.begin(), var_table.end(), modified);
	std::vector<proc_name>::iterator it_proc = std::find(proc_table.begin(), proc_table.end(), proc);

	if (it_var == var_table.end()) {
		throw std::invalid_argument("addModifiesP: Invalid var name: " + modified);
	}
	else if (it_proc == proc_table.end()) {
		throw std::invalid_argument("addModifiesP: Invalid proc name: " + proc);
	}
	modifiesP_table.insert(proc, modified);
}

void PKB::addUsesP(proc_name proc, var_name used) {
	std::vector<var_name>::iterator it_var = std::find(var_table.begin(), var_table.end(), used);
	std::vector<proc_name>::iterator it_proc = std::find(proc_table.begin(), proc_table.end(), proc);

	if (it_var == var_table.end()) {
		throw std::invalid_argument("addUsesP: Invalid var name: " + used);
	}
	else if (it_proc == proc_table.end()) {
		throw std::invalid_argument("addUsesP: Invalid proc name: " + proc);
	}
	usesP_table.insert(proc, used);
}

void PKB::addCallsP(proc_name caller_proc_name, proc_name callee_proc_name) {
	std::vector<proc_name>::iterator it_proc_caller = std::find(proc_table.begin(), proc_table.end(), caller_proc_name);
	std::vector<proc_name>::iterator it_proc_callee = std::find(proc_table.begin(), proc_table.end(), callee_proc_name);

	if (it_proc_caller == proc_table.end()) {
		throw std::invalid_argument("addCallsP: Invalid caller proc: " + caller_proc_name);
	}
	else if (it_proc_callee == proc_table.end()) {
		throw std::invalid_argument("addCallsP: Invalid callee proc: " + callee_proc_name);
	}
	callsP_table.insert(caller_proc_name, callee_proc_name);
}

void PKB::addCallsS(stmt_index caller_stmt_index, proc_name callee_proc_name) {
	try {
		if (stmt_table.at(caller_stmt_index - 1).stmt_type != STMT_CALL) {
			throw std::invalid_argument("addCallsS: Stmt index does not belong to an call statement: " + std::to_string(caller_stmt_index));
		}
		std::vector<var_name>::iterator it = std::find(proc_table.begin(), proc_table.end(), callee_proc_name);
		if (it == proc_table.end()) {
			throw std::invalid_argument("addCallsS: Invalid proc name: " + callee_proc_name);
		}
		callsS_table.insert(caller_stmt_index, callee_proc_name);
	}
	catch (std::out_of_range&) {
		throw std::invalid_argument("addCallsS: Invalid stmt index:" + std::to_string(caller_stmt_index));
	}
}

void PKB::addIf(stmt_index if_stmt_index, var_name control_var) {
	try {
		if (stmt_table.at(if_stmt_index - 1).stmt_type != STMT_IF) {
			throw std::invalid_argument("addIf: Stmt index does not belong to an if statement: " + std::to_string(if_stmt_index));
		}
		std::vector<var_name>::iterator it = std::find(var_table.begin(), var_table.end(), control_var);
		if (it == var_table.end()) {
			throw std::invalid_argument("addIf: Invalid var name: " + control_var);
		}
		if_table.insert(if_stmt_index, control_var);
	}
	catch (std::out_of_range&) {
		throw std::invalid_argument("addIf: Invalid stmt index:" + std::to_string(if_stmt_index));
	}
}

void PKB::addWhile(stmt_index while_stmt_index, var_name control_var) {
	try {
		if (stmt_table.at(while_stmt_index - 1).stmt_type != STMT_WHILE) {
			throw std::invalid_argument("addWhile: Stmt index does not belong to an while statement: " + std::to_string(while_stmt_index));
		}
		std::vector<var_name>::iterator it = std::find(var_table.begin(), var_table.end(), control_var);
		if (it == var_table.end()) {
			throw std::invalid_argument("addWhile: Invalid var name: " + control_var);
		}
		while_table.insert(while_stmt_index, control_var);
	}
	catch (std::out_of_range&) {
		throw std::invalid_argument("addWhile: Invalid stmt index:" + std::to_string(while_stmt_index));
	}
}

void PKB::addNext(prog_line prog_line1, prog_line prog_line2) {
	try {
		StmtInfo first_stmt_info{ prog_line1, stmt_table.at(prog_line1 - 1).stmt_type };
		StmtInfo second_stmt_info{ prog_line2, stmt_table.at(prog_line2 - 1).stmt_type };
		next_table.insert(first_stmt_info, second_stmt_info);
	}
	catch (std::out_of_range&) {
		throw std::invalid_argument("addNext: Invalid prog line: [" + std::to_string(prog_line1)
			+ "," + std::to_string(prog_line2) + "]");
	}
}

void PKB::addProcContains(proc_name proc, stmt_index index) {
	try {
		std::vector<var_name>::iterator it = std::find(proc_table.begin(), proc_table.end(), proc);
		if (it == proc_table.end()) {
			throw std::invalid_argument("addProcContains: Invalid proc name: " + proc);
		}
		procS_table.insert(proc, index);
	}
	catch (std::out_of_range&) {
		throw std::invalid_argument("addProcContains: Invalid stmt index:" + std::to_string(index));
	}
}

void PKB::generateParentT() {
	parentT_table = RelationsUtility<StmtInfo>::findTransitiveClosure(parent_table);
}

void PKB::generateFollowsT() {
	followsT_table = RelationsUtility<StmtInfo>::findTransitiveClosure(follows_table);
}

void PKB::generateCallsPT() {
	callsPT_table = RelationsUtility<proc_name>::findTransitiveClosure(callsP_table);
}

void PKB::resetCache() {
	const_table.clear();
	proc_table.clear();
	var_table.clear();
	stmt_table.clear();
	assignment_table.clear();
	follows_table.clear();
	parent_table.clear();
	followsT_table.clear();
	parentT_table.clear();
	usesS_table.clear();
	modifiesS_table.clear();
	usesP_table.clear();
	modifiesP_table.clear();
	callsP_table.clear();
	callsS_table.clear();
	callsPT_table.clear();
	if_table.clear();
	while_table.clear();
	next_table.clear();
	read_table.clear();
	print_table.clear();
	expr_table.clear();
	procS_table.clear();
}

void PKB::resetEntities() {
	const_table.clear();
	proc_table.clear();
	stmt_table.clear();
	const_table.clear();
}

const std::vector<proc_name>& PKB::getProcedures() {
	return proc_table;
}

const std::vector<var_name>& PKB::getVariables() {
	return var_table;
}

const std::vector<StmtInfo>& PKB::getStmts() {
	return stmt_table;
}

const StmtInfo PKB::getStmt(stmt_index stmt_index) {
	if (stmt_index <= 0) {
		throw std::invalid_argument("Stmt index must be greater than zero. ");
	}
	else if (stmt_index > stmt_table.size()) {
		throw std::invalid_argument("Invalid stmt index. ");
	}
	return stmt_table[stmt_index - 1];
}

const var_name PKB::getAssignment(stmt_index stmt_index) {
	if (stmt_index <= 0) {
		throw std::invalid_argument("Stmt index must be greater than zero: " + std::to_string(stmt_index));
	}
	else if (stmt_index > stmt_table.size()) {
		throw std::invalid_argument("Invalid stmt index: " + std::to_string(stmt_index));
	}
	else if (stmt_table[stmt_index - 1].stmt_type != STMT_ASSIGN) {
		throw std::invalid_argument("Stmt index does not belong to an assignment statement: " + std::to_string(stmt_index));
	}
	else if (!assignment_table.containsKey(stmt_index)) {
		throw std::invalid_argument("Stmt-related assignment has not been initiated: " + std::to_string(stmt_index));
	}
	return assignment_table.getValues(stmt_index)[0];
}

expr PKB::getExpression(stmt_index stmt_index) {
	if (stmt_index <= 0) {
		throw std::invalid_argument("Stmt index must be greater than zero: " + std::to_string(stmt_index));
	}
	else if (stmt_index > stmt_table.size()) {
		throw std::invalid_argument("Invalid stmt index: " + std::to_string(stmt_index));
	}
	else if (stmt_table[stmt_index - 1].stmt_type != STMT_ASSIGN) {
		throw std::invalid_argument("Stmt index does not belong to an assignment statement: " + std::to_string(stmt_index));
	}
	else if (expr_table.find(stmt_index) == expr_table.end()) {
		throw std::invalid_argument("Stmt-related expression has not been initiated: " + std::to_string(stmt_index));
	}
	return expr_table.at(stmt_index);
}

const std::vector<constant> PKB::getConstants() {
	std::vector<constant> v(const_table.begin(), const_table.end());
	return v;
}

const bool PKB::inSameProc(stmt_index index1, stmt_index index2) {
	std::vector<proc_name> v = procS_table.getKeys(index1);
	return procS_table.containsPair(v[0], index2);
}

const UniqueRelationTable<stmt_index, var_name>& PKB::getAssigns() {
	return assignment_table;
}

const std::unordered_map<stmt_index, expr>& PKB::getExpr() {
	return expr_table;
}

const UniqueRelationTable<StmtInfo, StmtInfo>& PKB::getFollows() {
	return follows_table;
}

const RelationTable<StmtInfo, StmtInfo>& PKB::getParent() {
	return parent_table;
}

const RelationTable<StmtInfo, StmtInfo>& PKB::getFollowsT() {
	return followsT_table;
}

const RelationTable<StmtInfo, StmtInfo>& PKB::getParentT() {
	return parentT_table;
}

const RelationTable<StmtInfo, var_name>& PKB::getUsesS() {
	return usesS_table;
}

const RelationTable<StmtInfo, var_name>& PKB::getModifiesS() {
	return modifiesS_table;
}

const RelationTable<proc_name, var_name>& PKB::getUsesP() {
	return usesP_table;
}

const RelationTable<proc_name, var_name>& PKB::getModifiesP() {
	return modifiesP_table;
}

const RelationTable<proc_name, proc_name>& PKB::getCallsP() {
	return callsP_table;
}

const RelationTable<proc_name, proc_name>& PKB::getCallsPT() {
	return callsPT_table;
}

const UniqueRelationTable<stmt_index, var_name>& PKB::getRead() {
	return read_table;
}

const UniqueRelationTable<stmt_index, var_name>& PKB::getPrint() {
	return print_table;
}

const RelationTable<stmt_index, proc_name>& PKB::getCallsS() {
	return callsS_table;
}

const RelationTable<stmt_index, var_name>& PKB::getIf() {
	return if_table;
}

const RelationTable<stmt_index, var_name>& PKB::getWhile() {
	return while_table;
}

const RelationTable<StmtInfo, StmtInfo>& PKB::getNext() {
	return next_table;
}

const RelationTable<proc_name, stmt_index>& PKB::getProcContains() {
	return procS_table;
}