#include "NextBipTPreprocessor.h"

bool NextBipTPreprocessor::evaluateWildAndWild() {
	return !next_bip_table->isEmpty();
}

bool NextBipTPreprocessor::evaluateConstantAndWild(int index) {
	StmtInfo s1 = stmt_info_list[index - 1];
	return next_bip_table->containsKey(s1);
}

bool NextBipTPreprocessor::evaluateWildAndConstant(int index) {
	StmtInfo s1 = stmt_info_list[index - 1];
	return next_bip_table->containsValue(s1);
}

bool NextBipTPreprocessor::evaluateConstantAndConstant(int index1, int index2) {
	StmtInfo s1 = stmt_info_list[index1 - 1];
	StmtInfo s2 = stmt_info_list[index2 - 1];
	fullyPopulate();
	return cache.containsPair(s1, s2);
}

std::vector<std::pair<StmtInfo, StmtInfo>> NextBipTPreprocessor::evaluateSynonymAndSynonym() {
	fullyPopulate();
	return cache.getPairs();
}

std::vector<StmtInfo> NextBipTPreprocessor::evaluateWildAndSynonym() {
	fullyPopulate();
	return cache.getValues();
}

std::vector<StmtInfo> NextBipTPreprocessor::evaluateSynonymAndWild() {
	fullyPopulate();
	return cache.getKeys();
}

std::vector<StmtInfo> NextBipTPreprocessor::evaluateConstantAndSynonym(int index) {
	StmtInfo s1 = stmt_info_list[index - 1];
	fullyPopulate();
	return cache.getValues(s1);
}

std::vector<StmtInfo> NextBipTPreprocessor::evaluateSynonymAndConstant(int index) {
	StmtInfo s1 = stmt_info_list[index - 1];
	fullyPopulate();
	return cache.getKeys(s1);
}

void NextBipTPreprocessor::fullyPopulate() {
	if (!is_fully_populated) {
		cache = RelationTableUtility<StmtInfo>::findTransitiveClosure(*next_bip_table);
		is_fully_populated = true;
	}
}

NextBipTPreprocessor::NextBipTPreprocessor(const MonotypeRelationTable<StmtInfo>& table, const std::vector<StmtInfo> v) :
	next_bip_table(&table) {
	stmt_info_list = v;
}