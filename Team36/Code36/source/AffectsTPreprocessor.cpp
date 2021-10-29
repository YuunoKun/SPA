#include "AffectsTPreprocessor.h"
#include "MonotypeRelationTable.cpp"

bool AffectsTPreprocessor::evaluateWildAndWild() {
	return !affects_table->isEmpty();
}

bool AffectsTPreprocessor::evaluateConstantAndWild(int index) {
	StmtInfo s1 = stmt_info_list[index - 1];
	return affects_table->containsKey(s1);
}

bool AffectsTPreprocessor::evaluateWildAndConstant(int index) {
	StmtInfo s1 = stmt_info_list[index - 1];
	return affects_table->containsValue(s1);
}

bool AffectsTPreprocessor::evaluateConstantAndConstant(int index1, int index2) {
	checkCache();
	StmtInfo s1 = stmt_info_list[index1 - 1];
	StmtInfo s2 = stmt_info_list[index2 - 1];
	if (is_fully_populated || calculated_matrix[index1 - 1][index2 - 1]) {
		return cache.containsPair(s1, s2);
	}
	else {
		auto dfs = cache.forwardDFS(s1);
		calculated_matrix[index1 - 1][index2 - 1] = true;
		for (StmtInfo indirect_value : dfs) {
			cache.insert(s1, indirect_value);
			if (indirect_value == s2) {
				return true;
			}
		}
		return cache.containsPair(s1, s2);
	}
}

std::vector<std::pair<StmtInfo, StmtInfo>> AffectsTPreprocessor::evaluateSynonymAndSynonym() {
	fullyPopulate();
	is_fully_populated = true;
	return cache.getPairs();
}

std::vector<StmtInfo> AffectsTPreprocessor::evaluateWildAndSynonym() {
	return affects_table->getValues();
}

std::vector<StmtInfo> AffectsTPreprocessor::evaluateSynonymAndWild() {
	return affects_table->getKeys();
}

std::vector<StmtInfo> AffectsTPreprocessor::evaluateConstantAndSynonym(int index) {
	checkCache();
	StmtInfo s1 = stmt_info_list[index - 1];
	if (is_fully_populated || calculated_dfs_forward[index - 1]) {
		return cache.getValues(s1);
	}
	else {
		std::vector<StmtInfo> res = cache.forwardDFS(s1);
		for (auto& s2 : res) {
			cache.insert(s1, s2);
		}
		setDFSForwardTrue(index);
		return res;
	}
}

std::vector<StmtInfo> AffectsTPreprocessor::evaluateSynonymAndConstant(int index) {
	checkCache();
	StmtInfo s1 = stmt_info_list[index - 1];
	if (is_fully_populated || calculated_dfs_backward[index - 1]) {
		return cache.getKeys(s1);
	}
	else {
		std::vector<StmtInfo> res = cache.backwardDFS(s1);
		for (auto& s2 : res) {
			cache.insert(s2, s1);
		}
		setDFSBackwardTrue(index);
		return res;
	}
}

void AffectsTPreprocessor::checkCache() {
	if (isCacheEmpty()) {
		cache = affects_table->copy();
	}
}

void AffectsTPreprocessor::fullyPopulate() {
	if (!is_fully_populated) {
		cache = affects_table->findTransitiveClosure();
		is_fully_populated = true;
	}
}

AffectsTPreprocessor::AffectsTPreprocessor(const MonotypeRelationTable<StmtInfo>& table, const std::vector<StmtInfo> v) :
	affects_table(&table) {
	stmt_info_list = v;
	int size = stmt_info_list.size();
	calculated_matrix.resize(size, std::vector<bool>(size, false));
	calculated_dfs_forward.resize(size, false);
	calculated_dfs_backward.resize(size, false);
}