#include "NextBipTPreprocessor.h"

bool NextBipTPreprocessor::evaluateWildAndWild() {
	return !cache.isEmpty();
}

bool NextBipTPreprocessor::evaluateConstantAndWild(int index) {
	StmtInfo s1 = stmt_info_list[index - 1];
	return cache.containsKey(s1);
}

bool NextBipTPreprocessor::evaluateWildAndConstant(int index) {
	StmtInfo s1 = stmt_info_list[index - 1];
	return cache.containsValue(s1);
}

bool NextBipTPreprocessor::evaluateConstantAndConstant(int index1, int index2) {
	StmtInfo s1 = stmt_info_list[index1 - 1];
	StmtInfo s2 = stmt_info_list[index2 - 1];
	return cache.containsPair(s1, s2);
}

std::vector<std::pair<StmtInfo, StmtInfo>> NextBipTPreprocessor::evaluateSynonymAndSynonym() {
	return cache.getPairs();
}

std::vector<StmtInfo> NextBipTPreprocessor::evaluateWildAndSynonym() {
	return cache.getValues();
}

std::vector<StmtInfo> NextBipTPreprocessor::evaluateSynonymAndWild() {
	return cache.getKeys();
}

std::vector<StmtInfo> NextBipTPreprocessor::evaluateConstantAndSynonym(int index) {
	StmtInfo s1 = stmt_info_list[index - 1];
	return cache.getValues(s1);
}

std::vector<StmtInfo> NextBipTPreprocessor::evaluateSynonymAndConstant(int index) {
	StmtInfo s1 = stmt_info_list[index - 1];
	return cache.getKeys(s1);
}

void NextBipTPreprocessor::fullyPopulate() {
	// Does nothing, populated through Constructor.
}

NextBipTPreprocessor::NextBipTPreprocessor(const MonotypeRelationTable<StmtInfo>& table, const std::vector<StmtInfo> v) {
	cache = table;
	stmt_info_list = v;
	int size = stmt_info_list.size();
	calculated_matrix.resize(size, std::vector<bool>(size, false));
	calculated_dfs_forward.resize(size, false);
	calculated_dfs_backward.resize(size, false);
}