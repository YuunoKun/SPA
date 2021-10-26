#include "AffectsPreprocessor.h"
#include "RelationsUtility.h"

#include <assert.h>

bool AffectsPreprocessor::evaluateWildAndWild() {
	return !next_table.isEmpty();
}

bool AffectsPreprocessor::evaluateConstantAndWild(int index) {
	StmtInfo s1 = stmt_info_list[index - 1];
	return next_table.containsKey(s1);
}

bool AffectsPreprocessor::evaluateWildAndConstant(int index) {
	StmtInfo s1 = stmt_info_list[index - 1];
	return next_table.containsValue(s1);
}

bool AffectsPreprocessor::evaluateConstantAndConstant(int index1, int index2) {
	StmtInfo s1 = stmt_info_list[index1 - 1];
	StmtInfo s2 = stmt_info_list[index2 - 1];
	if (is_fully_populated || calculated_matrix[index1][index2]) {
		return cache.containsPair(s1, s2);
	}
	else {
		for (StmtInfo indirect_value : RelationsUtility<StmtInfo>::forwardDFS(cache, s1)) {
			cache.insert(s1, indirect_value);
			if (indirect_value == s2) {
				return true;
			}
		}
		calculated_matrix[index1][index2] = true;
		return cache.containsPair(s1, s2);
	}
}

std::vector<std::pair<StmtInfo, StmtInfo>> AffectsPreprocessor::evaluateSynonymAndSynonym() {
	if (!is_fully_populated) {
		std::vector<stmt_index> worklist_stmts{};
		for (proc_name proc : procS_table.getKeys()) {
			worklist_stmts.push_back(procS_table.getValues(proc)[0]);
		}

		std::vector<std::pair<StmtInfo, StmtInfo>> res = solver.solve(worklist_stmts);
		updateCache(solver.getVisited(), res);
		is_fully_populated = true;
	}
	return cache.getPairs();
}

std::vector<StmtInfo> AffectsPreprocessor::evaluateWildAndSynonym() {
	return next_table.getValues();
}

std::vector<StmtInfo> AffectsPreprocessor::evaluateSynonymAndWild() {
	return next_table.getKeys();
}

std::vector<StmtInfo> AffectsPreprocessor::evaluateConstantAndSynonym(int index) {
	StmtInfo s1 = stmt_info_list[index - 1];
	if (!is_fully_populated && !calculated_dfs_forward[index - 1]) {
		proc_name proc = procS_table.getKeys(index)[0];
		std::vector<stmt_index> worklist_stmts{};
		worklist_stmts.push_back(procS_table.getValues(proc)[0]);
		solver.solve(worklist_stmts);
	}
	return cache.getValues(s1);
}

std::vector<StmtInfo> AffectsPreprocessor::evaluateSynonymAndConstant(int index) {
	StmtInfo s1 = stmt_info_list[index - 1];
	if (!is_fully_populated && !calculated_dfs_backward[index - 1]) {
		proc_name proc = procS_table.getKeys(index)[0];
		std::vector<stmt_index> worklist_stmts{};
		worklist_stmts.push_back(procS_table.getValues(proc)[0]);
		std::vector<std::pair<StmtInfo, StmtInfo>> res = solver.solve(worklist_stmts);
		updateCache(solver.getVisited(), res);
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