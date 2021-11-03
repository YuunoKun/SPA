#include "AffectsBipPreprocessor.h"

bool AffectsBipPreprocessor::evaluateWildAndWild() {
	fullyPopulate();
	return !cache.isEmpty();
}

bool AffectsBipPreprocessor::evaluateConstantAndWild(int index) {
	StmtInfo s = stmt_info_list[index - 1];
	fullyPopulate();
	return cache.containsKey(s);
}

bool AffectsBipPreprocessor::evaluateWildAndConstant(int index) {
	StmtInfo s = stmt_info_list[index - 1];
	fullyPopulate();
	return cache.containsValue(s);
}

bool AffectsBipPreprocessor::evaluateConstantAndConstant(int index1, int index2) {
	StmtInfo s1 = stmt_info_list[index1 - 1];
	StmtInfo s2 = stmt_info_list[index2 - 1];
	fullyPopulate();
	return cache.containsPair(s1, s2);
}

std::vector<std::pair<StmtInfo, StmtInfo>> AffectsBipPreprocessor::evaluateSynonymAndSynonym() {
	fullyPopulate();
	return cache.getPairs();
}

std::vector<StmtInfo> AffectsBipPreprocessor::evaluateWildAndSynonym() {
	fullyPopulate();
	return cache.getValues();
}

std::vector<StmtInfo> AffectsBipPreprocessor::evaluateSynonymAndWild() {
	fullyPopulate();
	return cache.getKeys();
}

std::vector<StmtInfo> AffectsBipPreprocessor::evaluateConstantAndSynonym(int index) {
	StmtInfo s1 = stmt_info_list[index - 1];
	fullyPopulate();
	return cache.getValues(s1);
}

std::vector<StmtInfo> AffectsBipPreprocessor::evaluateSynonymAndConstant(int index) {
	StmtInfo s1 = stmt_info_list[index - 1];
	fullyPopulate();
	return cache.getKeys(s1);
}

const MonotypeRelationTable<LabelledProgLine>& AffectsBipPreprocessor::getLabelledProgLineCache() {
	return labelled_progline_cache;
}

void AffectsBipPreprocessor::updateCache(std::vector<std::pair<LabelledProgLine, LabelledProgLine>> results) {
	for (auto& affects_pair : results) {
		labelled_progline_cache.insert(affects_pair.first, affects_pair.second);

		stmt_index labelled_progline_affecting = affects_pair.first.program_line;
		stmt_index labelled_progline_affected = affects_pair.second.program_line;
		StmtInfo statement_affecting = stmt_info_list[labelled_progline_affecting - 1];
		StmtInfo statement_affected = stmt_info_list[labelled_progline_affected - 1];
		cache.insert(statement_affecting, statement_affected);
	}
}

void AffectsBipPreprocessor::reset() {
	is_fully_populated = false;
	solver.reset();
	cache = MonotypeRelationTable<StmtInfo>();
}

void AffectsBipPreprocessor::fullyPopulate() {
	if (!is_fully_populated) {
		std::vector<std::pair<LabelledProgLine, LabelledProgLine>> res = solver.solve(getAssignments());
		updateCache(res);
		// add to secondary cache
		is_fully_populated = true;
	}
}

std::vector<stmt_index> AffectsBipPreprocessor::getAssignments() {
	std::vector<stmt_index> assignment_stmts;
	for (StmtInfo s : stmt_info_list) {
		if (s.stmt_type == STMT_ASSIGN) assignment_stmts.push_back(s.stmt_index);
	}
	return assignment_stmts;
}

AffectsBipPreprocessor::AffectsBipPreprocessor(
	const MonotypeRelationTable<LabelledProgLine>& next_table,
	const RelationTable<StmtInfo, var_name>& useS_table,
	const RelationTable<StmtInfo, var_name>& modifiesS_table,
	const RelationTable<proc_name, stmt_index>& procS_table,
	std::vector<LabelledProgLine>& first_stmts,
	const std::vector<StmtInfo> v) :
	next_table(&next_table),
	modifiesS_table(&modifiesS_table),
	useS_table(&useS_table),
	procS_table(&procS_table) {
	stmt_info_list = v;
	first_labelled_proglines = first_stmts;
	int size = stmt_info_list.size();
	calculated_matrix.resize(size, std::vector<bool>(size, false));
	calculated_dfs_forward.resize(size, false);
	calculated_dfs_backward.resize(size, false);
	solver = IterativeDataflowSolverBip(next_table, useS_table, modifiesS_table, stmt_info_list, first_stmts);
}