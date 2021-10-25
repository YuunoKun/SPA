#include "NextTPreprocessor.h"
#include <assert.h>

bool NextTPreprocessor::evaluateWildAndWild() {
	return !next_table.isEmpty();
}

bool NextTPreprocessor::evaluateConstantAndWild(int index) {
	StmtInfo s1 = stmt_info_list[index - 1];
	return next_table.containsKey(s1);
}

bool NextTPreprocessor::evaluateWildAndConstant(int index) {
	StmtInfo s1 = stmt_info_list[index - 1];
	return next_table.containsValue(s1);
}

bool NextTPreprocessor::evaluateConstantAndConstant(int index1, int index2) {
	checkCache();
	StmtInfo s1 = stmt_info_list[index1 - 1];
	StmtInfo s2 = stmt_info_list[index2 - 1];
	if (is_fully_populated || calculated_matrix[index1][index2]) {
		return cache.containsPair(s1, s2);
	}
	else {
		for (StmtInfo indirect_value : cache.forwardDFS(s1)) {
			if (indirect_value == s2) {
				return true;
			}
		}
		calculated_matrix[index1][index2] = true;
		return cache.containsPair(s1, s2);
	}
}

std::vector<std::pair<StmtInfo, StmtInfo>> NextTPreprocessor::evaluateSynonymAndSynonym() {
	checkCache();
	cache = next_table.findTransitiveClosure();
	is_fully_populated = true;
	return cache.getPairs();
}

std::vector<StmtInfo> NextTPreprocessor::evaluateWildAndSynonym() {
	return next_table.getValues();
}

std::vector<StmtInfo> NextTPreprocessor::evaluateSynonymAndWild() {
	return next_table.getKeys();
}

std::vector<StmtInfo> NextTPreprocessor::evaluateConstantAndSynonym(int index) {
	checkCache();
	StmtInfo s1 = stmt_info_list[index - 1];
	if (is_fully_populated || calculated_dfs_forward[index - 1]) {
		return cache.getValues(s1);
	}
	else {
		std::vector<StmtInfo> res = cache.forwardDFS(s1);
		setDFSForwardTrue(index);
		return res;
	}
}

std::vector<StmtInfo> NextTPreprocessor::evaluateSynonymAndConstant(int index) {
	checkCache();
	StmtInfo s1 = stmt_info_list[index - 1];
	if (is_fully_populated || calculated_dfs_backward[index - 1]) {
		return cache.getKeys(s1);
	}
	else {
		std::vector<StmtInfo> res = cache.backwardDFS(s1);
		setDFSBackwardTrue(index);
		return res;
	}
}

void NextTPreprocessor::checkCache() {
	if (is_cache_initialized == false) {
		cache = next_table.copy();
		is_cache_initialized = true;
	}
}

NextTPreprocessor::NextTPreprocessor(const RelationTable<StmtInfo, StmtInfo>& table, const std::vector<StmtInfo> v) :
	next_table(table) {
	stmt_info_list = v;
	int size = stmt_info_list.size();
	calculated_matrix.resize(size, std::vector<bool>(size, false));
	calculated_dfs_forward.resize(size, false);
	calculated_dfs_backward.resize(size, false);
}