#include "NextBipPreprocessor.h"

bool NextBipPreprocessor::evaluateWildAndWild() {
	return !cache.isEmpty();
}

bool NextBipPreprocessor::evaluateConstantAndWild(int index) {
	StmtInfo s1 = stmt_info_list[index - 1];
	return cache.containsKey(s1);
}

bool NextBipPreprocessor::evaluateWildAndConstant(int index) {
	StmtInfo s1 = stmt_info_list[index - 1];
	return cache.containsValue(s1);
}

bool NextBipPreprocessor::evaluateConstantAndConstant(int index1, int index2) {
	StmtInfo s1 = stmt_info_list[index1 - 1];
	StmtInfo s2 = stmt_info_list[index2 - 1];
	return cache.containsPair(s1, s2);
}

std::vector<std::pair<StmtInfo, StmtInfo>> NextBipPreprocessor::evaluateSynonymAndSynonym() {
	return cache.getPairs();
}

std::vector<StmtInfo> NextBipPreprocessor::evaluateWildAndSynonym() {
	return cache.getValues();
}

std::vector<StmtInfo> NextBipPreprocessor::evaluateSynonymAndWild() {
	return cache.getKeys();
}

std::vector<StmtInfo> NextBipPreprocessor::evaluateConstantAndSynonym(int index) {
	StmtInfo s1 = stmt_info_list[index - 1];
	return cache.getValues(s1);
}

std::vector<StmtInfo> NextBipPreprocessor::evaluateSynonymAndConstant(int index) {
	StmtInfo s1 = stmt_info_list[index - 1];
	return cache.getKeys(s1);
}

void NextBipPreprocessor::fullyPopulate() {
	// Does nothing, populated through Constructor.
}

NextBipPreprocessor::NextBipPreprocessor(const MonotypeRelationTable<StmtInfo>& table, const std::vector<StmtInfo> v) {
	cache = table;
	stmt_info_list = v;
	is_fully_populated = true;
	int size = stmt_info_list.size();
	calculated_matrix.resize(size, std::vector<bool>(size, false));
	calculated_dfs_forward.resize(size, false);
	calculated_dfs_backward.resize(size, false);
}