#include "IterativeDataflowSolver.h"

IterativeDataflowSolver::IterativeDataflowSolver(
	const RelationTable<StmtInfo, StmtInfo>& next_table,
	const RelationTable<StmtInfo, var_name>& useS_table,
	const RelationTable<StmtInfo, var_name>& modifiesS_table,
	const RelationTable<proc_name, stmt_index>& procS_table,
	std::vector<StmtInfo> v) :
	next_table(next_table),
	modifiesS_table(modifiesS_table),
	useS_table(useS_table),
	procS_table(procS_table),
	stmt_info_list(v)
{
	int size = stmt_info_list.size();
	kill_list.resize(size, {});
	gen_list.resize(size, {});
	in_list.resize(size, {});
	out_list.resize(size, {});
	pred_list.resize(size, {});
	succ_list.resize(size, {});
}

std::vector<std::pair<StmtInfo, StmtInfo>> IterativeDataflowSolver::solve(std::vector<stmt_index> starting_worklist) {
	populateDataflowSets();
	std::stack<stmt_index> worklist;
	std::vector<std::pair<StmtInfo, StmtInfo>> res;
	for (auto& stmt_index : starting_worklist) {
		worklist.push(stmt_index);
	}

	while (!worklist.empty()) {
		stmt_index curr = worklist.top();
		visited.emplace(curr);
		worklist.pop();
		int index = curr - 1;
		int old_out_size = out_list[curr - 1].size();

		std::set<ModifiesTuple> new_in_list{};

		for (stmt_index pred : pred_list[index]) {
			std::set_union(out_list[pred - 1].begin(), out_list[pred - 1].end(),
				in_list[index].begin(), in_list[index].end(),
				std::inserter(new_in_list, new_in_list.begin()));
		}

		in_list[index] = new_in_list;

		std::set<ModifiesTuple> new_out_list{};
		std::set_difference(new_in_list.begin(), new_in_list.end(),
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

	for (auto& affected_index : visited) {
		StmtInfo statement_affected = stmt_info_list[affected_index - 1];
		if (statement_affected.stmt_type != STMT_ASSIGN) {
			continue;
		}
		for (auto& tuple : in_list[affected_index - 1]) {
			StmtInfo statement_affecting = stmt_info_list[tuple.stmt_index - 1];
			for (auto& var_used : useS_table.getValues(statement_affected)) {
				bool var_matched = tuple.var_name == var_used;
				if (var_matched) {
					res.push_back({ statement_affecting, statement_affected });
					break;
				}
			}
		}
	}

	return res;
}

void IterativeDataflowSolver::populateDataflowSets() {
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

std::set<stmt_index> IterativeDataflowSolver::getVisited() {
	return visited;
}