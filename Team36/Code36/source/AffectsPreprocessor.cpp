#include "AffectsPreprocessor.h"

bool AffectsPreprocessor::evaluateWildAndWild() {
	if (!cache.isEmpty()) {
		is_non_empty = STATUS_TRUE;
		return true;
	}
	else if (is_non_empty != STATUS_UNKNOWN) {
		return is_non_empty;
	}
	else {
		std::vector<stmt_index> list_of_first_statements{};
		for (proc_name proc : procS_table->getKeys()) {
			list_of_first_statements.push_back(procS_table->getValues(proc)[0]);
		}
		bool is_non_empty_bool = solver.solveIfNonEmpty(list_of_first_statements);
		if (is_non_empty_bool) {
			is_non_empty = STATUS_TRUE;
		}
		else {
			is_non_empty = STATUS_FALSE;
		}
		return is_non_empty_bool;
	}
}

bool AffectsPreprocessor::evaluateConstantAndWild(int index) {
	if (is_affecting[index - 1] != STATUS_UNKNOWN) {
		return is_affecting[index - 1];
	}
	else if (is_fully_populated || calculated_dfs_forward[index - 1] == true) {
		StmtInfo s1 = stmt_info_list[index - 1];
		return cache.containsKey(s1);
	}
	else if (stmt_info_list[index - 1].stmt_type != STMT_ASSIGN) {
		is_affecting[index - 1] = STATUS_FALSE;
		return false;
	}
	else {
		bool is_index_affecting = solver.solveIfAffecting(index);
		if (is_index_affecting) {
			is_affecting[index - 1] = STATUS_TRUE;
		}
		else {
			is_affecting[index - 1] = STATUS_FALSE;
		}
		return is_index_affecting;
	}
}

bool AffectsPreprocessor::evaluateWildAndConstant(int index) {
	if (is_affected[index - 1] != STATUS_UNKNOWN) {
		return is_affected[index - 1];
	}
	else if (is_fully_populated || calculated_dfs_backward[index - 1] == true) {
		StmtInfo s1 = stmt_info_list[index - 1];
		return cache.containsValue(s1);
	}
	else if (stmt_info_list[index - 1].stmt_type != STMT_ASSIGN) {
		is_affected[index - 1] = STATUS_FALSE;
		return false;
	}
	else {
		proc_name proc = procS_table->getKeys(index)[0];
		bool is_index_affected = solver.solveIfAffected(index, getFirstAssignStmt(proc));
		if (is_index_affected) {
			is_affected[index - 1] = STATUS_TRUE;
		}
		else {
			is_affected[index - 1] = STATUS_FALSE;
		}
		return is_index_affected;
	}
}

bool AffectsPreprocessor::evaluateConstantAndConstant(int index1, int index2) {
	StmtInfo s1 = stmt_info_list[index1 - 1];
	StmtInfo s2 = stmt_info_list[index2 - 1];
	if (stmt_info_list[index1 - 1].stmt_type != STMT_ASSIGN || stmt_info_list[index2 - 1].stmt_type != STMT_ASSIGN) {
		calculated_matrix[index1 - 1][index2 - 1] = true;
		return false;
	}
	if (!is_fully_populated && !calculated_matrix[index1 - 1][index2 - 1] &&
		!calculated_dfs_forward[index1 - 1] && !calculated_dfs_backward[index2 - 1]) {
		if (solver.solveIfAffectingAndAffected(index1, index2)) {
			cache.insert(s1, s2);
		}
		calculated_matrix[index1 - 1][index2 - 1] = true;
	}
	return cache.containsPair(s1, s2);
}

std::vector<std::pair<StmtInfo, StmtInfo>> AffectsPreprocessor::evaluateSynonymAndSynonym() {
	fullyPopulate();
	return cache.getPairs();
}

std::vector<StmtInfo> AffectsPreprocessor::evaluateWildAndSynonym() {
	fullyPopulate();
	return cache.getValues();
}

std::vector<StmtInfo> AffectsPreprocessor::evaluateSynonymAndWild() {
	fullyPopulate();
	return cache.getKeys();
}

std::vector<StmtInfo> AffectsPreprocessor::evaluateConstantAndSynonym(int index) {
	if (stmt_info_list[index - 1].stmt_type != STMT_ASSIGN) {
		is_affecting[index - 1] = STATUS_FALSE;
		return std::vector<StmtInfo>{};
	}
	StmtInfo s1 = stmt_info_list[index - 1];
	if (!is_fully_populated && !calculated_dfs_forward[index - 1]) {
		proc_name proc = procS_table->getKeys(index)[0];
		std::vector<stmt_index> first_assign_stmt_of_curr_proc{ getFirstAssignStmt(proc) };
		auto [visited, res] = solver.solve(first_assign_stmt_of_curr_proc);
		updateCache(visited, res);
	}
	return cache.getValues(s1);
}

std::vector<StmtInfo> AffectsPreprocessor::evaluateSynonymAndConstant(int index) {
	if (stmt_info_list[index - 1].stmt_type != STMT_ASSIGN) {
		is_affected[index - 1] = STATUS_FALSE;
		return std::vector<StmtInfo>{};
	}
	StmtInfo s1 = stmt_info_list[index - 1];
	if (!is_fully_populated && !calculated_dfs_backward[index - 1]) {
		proc_name proc = procS_table->getKeys(index)[0];
		std::vector<stmt_index> first_assign_stmt_of_curr_proc{ getFirstAssignStmt(proc) };
		auto [visited, res] = solver.solve(first_assign_stmt_of_curr_proc);
		updateCache(visited, res);
	}
	return cache.getKeys(s1);
}

std::vector<BooleanStatus> AffectsPreprocessor::getAffecting() {
	return is_affecting;
}

std::vector<BooleanStatus> AffectsPreprocessor::getAffected() {
	return is_affected;
}

BooleanStatus AffectsPreprocessor::isNonEmpty() {
	return is_non_empty;
}

void AffectsPreprocessor::updateCache(std::set<stmt_index> visited, std::vector<std::pair<StmtInfo, StmtInfo>> results) {
	for (auto& affects_pair : results) {
		StmtInfo statement_affecting = affects_pair.first;
		StmtInfo statement_affected = affects_pair.second;
		cache.insert(statement_affecting, statement_affected);
	}

	for (auto& v : visited) {
		setDFSForwardTrue(v);
		setDFSBackwardTrue(v);
	}
}

void AffectsPreprocessor::reset() {
	is_fully_populated = false;
	int size = calculated_matrix.size();
	for (int i = 0; i < size; i++) {
		calculated_dfs_forward[i] = false;
		calculated_dfs_backward[i] = false;
		for (int j = 0; j < size; j++) {
			calculated_matrix[i][j] = false;
		}
	}
	solver.reset();
	is_non_empty = STATUS_UNKNOWN;
	std::vector<BooleanStatus> is_affecting{ stmt_info_list.size(), STATUS_UNKNOWN };
	std::vector<BooleanStatus> is_affected{ stmt_info_list.size(), STATUS_UNKNOWN };
	cache = MonotypeRelationTable<StmtInfo>();
}

void AffectsPreprocessor::fullyPopulate() {
	if (!is_fully_populated) {
		std::vector<stmt_index> all_first_assign_stmts{};
		for (auto& p : procS_table->getKeys()) {
			all_first_assign_stmts.push_back(getFirstAssignStmt(p));
		}
		auto [visited, res] = solver.solve(all_first_assign_stmts);
		updateCache(visited, res);
		is_fully_populated = true;
	}
}

stmt_index AffectsPreprocessor::getFirstAssignStmt(proc_name proc) {
	for (stmt_index stmt : procS_table->getValues(proc)) {
		if (stmt_info_list[stmt - 1].stmt_type == STMT_ASSIGN) {
			return stmt;
			break;
		}
	}
}

AffectsPreprocessor::AffectsPreprocessor(
	const MonotypeRelationTable<StmtInfo>& next_table,
	const RelationTable<StmtInfo, var_name>& useS_table,
	const RelationTable<StmtInfo, var_name>& modifiesS_table,
	const RelationTable<proc_name, stmt_index>& procS_table,
	const std::vector<StmtInfo> v) :
	next_table(&next_table),
	modifiesS_table(&modifiesS_table),
	useS_table(&useS_table),
	procS_table(&procS_table) {
	stmt_info_list = v;
	int size = stmt_info_list.size();
	calculated_matrix.resize(size, std::vector<bool>(size, false));
	calculated_dfs_forward.resize(size, false);
	calculated_dfs_backward.resize(size, false);
	is_affecting.resize(size, STATUS_UNKNOWN);
	is_affected.resize(size, STATUS_UNKNOWN);
	solver = IterativeDataflowSolver(next_table, useS_table, modifiesS_table, procS_table, stmt_info_list);
}