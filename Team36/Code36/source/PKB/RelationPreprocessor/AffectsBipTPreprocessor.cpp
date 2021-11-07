#include "AffectsBipTPreprocessor.h"
#include "PKB/RelationTables/RelationTableUtility.cpp"

bool AffectsBipTPreprocessor::evaluateWildAndWild() {
	return !affects_bip_table->isEmpty();
}

bool AffectsBipTPreprocessor::evaluateConstantAndWild(int index) {
	StmtInfo s1 = stmt_info_list[index - 1];
	return affects_bip_table->containsKey(s1);
}

bool AffectsBipTPreprocessor::evaluateWildAndConstant(int index) {
	StmtInfo s1 = stmt_info_list[index - 1];
	return affects_bip_table->containsValue(s1);
}

bool AffectsBipTPreprocessor::evaluateConstantAndConstant(int index1, int index2) {
	StmtInfo s1 = stmt_info_list[index1 - 1];
	StmtInfo s2 = stmt_info_list[index2 - 1];
	fullyPopulate();
	return cache.containsPair(s1, s2);
}

std::vector<std::pair<StmtInfo, StmtInfo>> AffectsBipTPreprocessor::evaluateSynonymAndSynonym() {
	fullyPopulate();
	return cache.getPairs();
}

std::vector<StmtInfo> AffectsBipTPreprocessor::evaluateWildAndSynonym() {
	fullyPopulate();
	return cache.getValues();
}

std::vector<StmtInfo> AffectsBipTPreprocessor::evaluateSynonymAndWild() {
	fullyPopulate();
	return cache.getKeys();
}

std::vector<StmtInfo> AffectsBipTPreprocessor::evaluateConstantAndSynonym(int index) {
	StmtInfo s1 = stmt_info_list[index - 1];
	fullyPopulate();
	return cache.getValues(s1);
}

std::vector<StmtInfo> AffectsBipTPreprocessor::evaluateSynonymAndConstant(int index) {
	StmtInfo s1 = stmt_info_list[index - 1];
	fullyPopulate();
	return cache.getKeys(s1);
}

void AffectsBipTPreprocessor::fullyPopulate() {
	if (!is_fully_populated) {
		labelled_cache = RelationTableUtility<LabelledProgLine>::findTransitiveClosure(*labelled_affects_bip_table);
		is_fully_populated = true;

		for (auto& affects_pair : labelled_cache.getPairs()) {
			stmt_index labelled_progline_affecting = affects_pair.first.program_line;
			stmt_index labelled_progline_affected = affects_pair.second.program_line;
			StmtInfo statement_affecting = stmt_info_list[labelled_progline_affecting - 1];
			StmtInfo statement_affected = stmt_info_list[labelled_progline_affected - 1];
			cache.insert(statement_affecting, statement_affected);
		}
	}
}

AffectsBipTPreprocessor::AffectsBipTPreprocessor(const MonotypeRelationTable<StmtInfo>& table,
	const MonotypeRelationTable<LabelledProgLine>& labelled_table,
	const std::vector<StmtInfo> v) :
	affects_bip_table(&table),
	labelled_affects_bip_table(&labelled_table) {
	stmt_info_list = v;
	int size = stmt_info_list.size();
	calculated_matrix.resize(size, std::vector<bool>(size, false));
	calculated_dfs_forward.resize(size, false);
	calculated_dfs_backward.resize(size, false);
}