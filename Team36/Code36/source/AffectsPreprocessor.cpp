#include "AffectsPreprocessor.h"
#include "RelationsUtility.h"

#include <assert.h>

bool AffectsPreprocessor::evaluateWildAndWild() {
	if (!cache.isEmpty()) {
		return true;
	}
	if (!isNonEmptyCalculated) {
		std::vector<stmt_index> list_of_first_statements{};
		for (proc_name proc : procS_table.getKeys()) {
			list_of_first_statements.push_back(procS_table.getValues(proc)[0]);
		}
		isNonEmpty = solver.solveIfNonEmpty(list_of_first_statements);
		isNonEmptyCalculated = true;
	}
	return isNonEmpty;
}

bool AffectsPreprocessor::evaluateConstantAndWild(int index) {
	if (isAffecting[index - 1] != STATUS_UNKNOWN) {
		return isAffecting[index - 1];
	}
	else {
		bool isAffectingBool = solver.solveIfAffecting(index);
		if (isAffectingBool) {
			isAffecting[index - 1] = STATUS_TRUE;
		}
		else {
			isAffecting[index - 1] = STATUS_FALSE;
		}
		return isAffectingBool;
	}
}

bool AffectsPreprocessor::evaluateWildAndConstant(int index) {
	if (isAffected[index - 1] != STATUS_UNKNOWN) {
		return isAffecting[index - 1];
	}
	else {
		bool isAffectedBool = solver.solveIfAffected(index);
		if (isAffectedBool) {
			isAffected[index - 1] = STATUS_TRUE;
		}
		else {
			isAffected[index - 1] = STATUS_FALSE;
		}
		return isAffectedBool;
	}
}

bool AffectsPreprocessor::evaluateConstantAndConstant(int index1, int index2) {
	StmtInfo s1 = stmt_info_list[index1 - 1];
	StmtInfo s2 = stmt_info_list[index2 - 1];
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
	if (!is_fully_populated) {
		std::vector<stmt_index> list_of_first_statements{};
		for (proc_name proc : procS_table.getKeys()) {
			list_of_first_statements.push_back(procS_table.getValues(proc)[0]);
		}

		auto [visited, res] = solver.solve(list_of_first_statements);
		updateCache(visited, res);
		is_fully_populated = true;
	}
	return cache.getPairs();
}

std::vector<StmtInfo> AffectsPreprocessor::evaluateWildAndSynonym() {
	if (!is_fully_populated) {
		std::vector<stmt_index> list_of_first_statements{};
		for (proc_name proc : procS_table.getKeys()) {
			list_of_first_statements.push_back(procS_table.getValues(proc)[0]);
		}

		auto [visited, res] = solver.solve(list_of_first_statements);
		updateCache(visited, res);
		is_fully_populated = true;
	}
	return cache.getValues();
}

std::vector<StmtInfo> AffectsPreprocessor::evaluateSynonymAndWild() {
	if (!is_fully_populated) {
		std::vector<stmt_index> list_of_first_statements{};
		for (proc_name proc : procS_table.getKeys()) {
			list_of_first_statements.push_back(procS_table.getValues(proc)[0]);
		}

		auto [visited, res] = solver.solve(list_of_first_statements);
		updateCache(visited, res);
		is_fully_populated = true;
	}
	return cache.getKeys();
}

std::vector<StmtInfo> AffectsPreprocessor::evaluateConstantAndSynonym(int index) {
	StmtInfo s1 = stmt_info_list[index - 1];
	if (!is_fully_populated && !calculated_dfs_forward[index - 1]) {
		proc_name proc = procS_table.getKeys(index)[0];
		std::vector<stmt_index> list_of_first_statements{};
		list_of_first_statements.push_back(procS_table.getValues(proc)[0]);
		auto [visited, res] = solver.solve(list_of_first_statements);
		updateCache(visited, res);
	}
	return cache.getValues(s1);
}

std::vector<StmtInfo> AffectsPreprocessor::evaluateSynonymAndConstant(int index) {
	StmtInfo s1 = stmt_info_list[index - 1];
	if (!is_fully_populated && !calculated_dfs_backward[index - 1]) {
		proc_name proc = procS_table.getKeys(index)[0];
		std::vector<stmt_index> list_of_first_statements{};
		list_of_first_statements.push_back(procS_table.getValues(proc)[0]);
		auto [visited, res] = solver.solve(list_of_first_statements);
		updateCache(visited, res);
	}
	return cache.getKeys(s1);
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

AffectsPreprocessor::AffectsPreprocessor(
	const RelationTable<StmtInfo, StmtInfo>& next_table,
	const RelationTable<StmtInfo, var_name>& useS_table,
	const RelationTable<StmtInfo, var_name>& modifiesS_table,
	const RelationTable<proc_name, stmt_index>& procS_table,
	const std::vector<StmtInfo>& v) :
	next_table(next_table),
	modifiesS_table(modifiesS_table),
	useS_table(useS_table),
	procS_table(procS_table),
	stmt_info_list(v) {
	int size = stmt_info_list.size();
	is_cache_initialized = true;
	calculated_matrix.resize(size, std::vector<bool>(size, false));
	calculated_dfs_forward.resize(size, false);
	calculated_dfs_backward.resize(size, false);
}