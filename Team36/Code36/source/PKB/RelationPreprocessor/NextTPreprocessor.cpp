#include "NextTPreprocessor.h"
#include "PKB/RelationTables/RelationTableUtility.cpp"

bool NextTPreprocessor::evaluateWildAndWild() {
	return !next_table->isEmpty();
}

bool NextTPreprocessor::evaluateConstantAndWild(int index) {
	StmtInfo s1 = stmt_info_list[index - 1];
	return next_table->containsKey(s1);
}

bool NextTPreprocessor::evaluateWildAndConstant(int index) {
	StmtInfo s1 = stmt_info_list[index - 1];
	return next_table->containsValue(s1);
}

bool NextTPreprocessor::evaluateConstantAndConstant(int index1, int index2) {
	StmtInfo s1 = stmt_info_list[index1 - 1];
	StmtInfo s2 = stmt_info_list[index2 - 1];
	if (is_fully_populated || calculated_matrix[index1 - 1][index2 - 1]) {
		return cache.containsPair(s1, s2);
	}

	auto dfs = RelationTableUtility<StmtInfo>::forwardDFS(*next_table, s1);
	calculated_matrix[index1 - 1][index2 - 1] = true;
	for (StmtInfo indirect_value : dfs) {
		calculated_matrix[index1 - 1][indirect_value.stmt_index - 1] = true;
		cache.insert(s1, indirect_value);
		if (indirect_value == s2) {
			return true;
		}
	}
	return false;
}

std::vector<std::pair<StmtInfo, StmtInfo>> NextTPreprocessor::evaluateSynonymAndSynonym() {
	fullyPopulate();
	return cache.getPairs();
}

std::vector<StmtInfo> NextTPreprocessor::evaluateWildAndSynonym() {
	return next_table->getValues();
}

std::vector<StmtInfo> NextTPreprocessor::evaluateSynonymAndWild() {
	return next_table->getKeys();
}

std::vector<StmtInfo> NextTPreprocessor::evaluateConstantAndSynonym(int index) {
	StmtInfo s1 = stmt_info_list[index - 1];
	if (is_fully_populated || calculated_dfs_forward[index - 1]) {
		return cache.getValues(s1);
	} else {
		std::vector<StmtInfo> res = RelationTableUtility<StmtInfo>::forwardDFS(*next_table, s1);
		for (auto& s2 : res) {
			cache.insert(s1, s2);
		}
		setDFSForwardTrue(index);
		return res;
	}
}

std::vector<StmtInfo> NextTPreprocessor::evaluateSynonymAndConstant(int index) {
	StmtInfo s1 = stmt_info_list[index - 1];
	if (is_fully_populated || calculated_dfs_backward[index - 1]) {
		return cache.getKeys(s1);
	} else {
		std::vector<StmtInfo> res = RelationTableUtility<StmtInfo>::backwardDFS(*next_table, s1);
		for (auto& s2 : res) {
			cache.insert(s2, s1);
		}
		setDFSBackwardTrue(index);
		return res;
	}
}

void NextTPreprocessor::fullyPopulate() {
	if (!is_fully_populated) {
		cache = RelationTableUtility<StmtInfo>::findTransitiveClosure(*next_table);
		is_fully_populated = true;
	}
}

NextTPreprocessor::NextTPreprocessor(const MonotypeRelationTable<StmtInfo>& table, const std::vector<StmtInfo> v) :
	next_table(&table) {
	stmt_info_list = v;
	int size = stmt_info_list.size();
	calculated_matrix.resize(size, std::vector<bool>(size, false));
	calculated_dfs_forward.resize(size, false);
	calculated_dfs_backward.resize(size, false);
}