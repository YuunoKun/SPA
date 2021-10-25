#include "AffectsPreprocessor.h"
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
		for (StmtInfo indirect_value : cache.forwardDFS(s1)) {
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
		iterativeDataFlowAnalysis(stmt_info_list);
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
		std::vector<StmtInfo> worklist_stmts{};
		for (auto& stmt_index : procS_table.getValues(proc)) {
			worklist_stmts.push_back(stmt_info_list[stmt_index - 1]);
		}
		iterativeDataFlowAnalysis(worklist_stmts);
	}
	return cache.getValues(s1);
}

std::vector<StmtInfo> AffectsPreprocessor::evaluateSynonymAndConstant(int index) {
	StmtInfo s1 = stmt_info_list[index - 1];
	if (!is_fully_populated && !calculated_dfs_backward[index - 1]) {
		proc_name proc = procS_table.getKeys(index)[0];
		std::vector<StmtInfo> worklist_stmts{};
		for (auto& stmt_index : procS_table.getValues(proc)) {
			worklist_stmts.push_back(stmt_info_list[stmt_index - 1]);
		}
		iterativeDataFlowAnalysis(worklist_stmts);
	}
	return cache.getKeys(s1);
}

void AffectsPreprocessor::populateDataflowSets() {
	if (is_dataflow_sets_populated == true) {
		return;
	}
	for (int i = 0; i < stmt_info_list.size(); i++) {
		StmtInfo s = stmt_info_list[i];
		if (s.stmt_type == STMT_ASSIGN || s.stmt_type == STMT_READ ||
			s.stmt_type == STMT_CALL) {
			std::vector<var_name> var_modified_by_s = modifiesS_table.getValues(s);
			for (auto& var : var_modified_by_s) {
				std::vector<StmtInfo> other_stmts_modify_var = modifiesS_table.getKeys(var);
				for (auto& other_stmt : other_stmts_modify_var) {
					kill_list[i].emplace(ModifiesTuple{ other_stmt.stmt_index, var });
				}
				if (s.stmt_type == STMT_ASSIGN) {
					gen_list[i].emplace(ModifiesTuple{ s.stmt_index, var });
					out_list[i].emplace(ModifiesTuple{ s.stmt_index, var });
				}
			}
		}
		std::vector<StmtInfo> predecessor_stmts = next_table.getKeys(s);
		for (auto& pred : predecessor_stmts) {
			pred_list[s.stmt_index - 1].emplace(pred.stmt_index);
		}
		std::vector<StmtInfo> successor_stmts = next_table.getValues(s);
		for (auto& succ : successor_stmts) {
			succ_list[s.stmt_index - 1].emplace(succ.stmt_index);
		}
	}
	is_dataflow_sets_populated = true;
}

void AffectsPreprocessor::updateCache(std::vector<StmtInfo> stmts) {
	for (auto& statement_affected : stmts) {
		if (statement_affected.stmt_type != STMT_ASSIGN) {
			continue;
		}
		for (auto& tuple : in_list[statement_affected.stmt_index - 1]) {
			StmtInfo statement_affecting = stmt_info_list[tuple.stmt_index - 1];
			for (auto& var_used : useS_table.getValues(statement_affected)) {
				bool var_matched = tuple.var_name == var_used;
				if (var_matched) {
					cache.insert(statement_affecting, statement_affected);
					break;
				}
			}
		}
	}
	for (auto& stmt : stmts) {
		setDFSForwardTrue(stmt.stmt_index);
		setDFSBackwardTrue(stmt.stmt_index);
	}
}

void AffectsPreprocessor::iterativeDataFlowAnalysis(std::vector<StmtInfo> stmts_worklist) {
	populateDataflowSets();
	std::stack<stmt_index> worklist;
	int counter = 0;
	for (int i = stmts_worklist.size(); i >= 1; i--) {
		worklist.push(i);
	}

	while (!worklist.empty()) {
		counter++;
		stmt_index curr = worklist.top();
		worklist.pop();
		int index = curr - 1;
		int old_out_size = out_list[curr - 1].size();
		int old_in_size = in_list[curr - 1].size();

		std::vector<StmtInfo> predecessors = next_table.getKeys(stmt_info_list[index]);
		std::set<ModifiesTuple> new_in_list{};

		for (stmt_index pred : pred_list[index]) {
			std::set_union(out_list[pred - 1].begin(), out_list[pred - 1].end(),
				in_list[index].begin(), in_list[index].end(),
				std::inserter(new_in_list, new_in_list.begin()));
		}

		if (old_in_size != new_in_list.size()) {
			in_list[index] = new_in_list;

			std::set<ModifiesTuple> filtered{ in_list[index] };
			std::set<ModifiesTuple> new_out_list{};
			std::set_difference(filtered.begin(), filtered.end(),
				kill_list[index].begin(), kill_list[index].end(),
				std::inserter(new_out_list, new_out_list.begin()));

			std::set_union(new_out_list.begin(), new_out_list.end(),
				gen_list[index].begin(), gen_list[index].end(),
				std::inserter(out_list[index], out_list[index].begin()));

			if (out_list[index].size() != old_out_size) {
				for (auto& succ : succ_list[index]) {
					worklist.push(succ);
				}
			}
		}
	}
	updateCache(stmts_worklist);
}

AffectsPreprocessor::AffectsPreprocessor(
	const RelationTable<StmtInfo, StmtInfo>& next_table,
	const RelationTable<StmtInfo, var_name>& useS_table,
	const RelationTable<StmtInfo, var_name>& modifiesS_table,
	const RelationTable<proc_name, stmt_index>& procS_table,
	const std::vector<StmtInfo> v) :
	next_table(next_table),
	modifiesS_table(modifiesS_table),
	useS_table(useS_table),
	procS_table(procS_table) {
	stmt_info_list = v;
	int size = stmt_info_list.size();
	is_cache_initialized = true;
	calculated_matrix.resize(size, std::vector<bool>(size, false));
	calculated_dfs_forward.resize(size, false);
	calculated_dfs_backward.resize(size, false);
	kill_list.resize(size, {});
	gen_list.resize(size, {});
	in_list.resize(size, {});
	out_list.resize(size, {});
	pred_list.resize(size, {});
	succ_list.resize(size, {});
}