#include "IterativeDataflowSolver.h"

IterativeDataflowSolver::IterativeDataflowSolver(
	const MonotypeRelationTable<StmtInfo>& next_table,
	const RelationTable<StmtInfo, var_name>& useS_table,
	const RelationTable<StmtInfo, var_name>& modifiesS_table,
	const RelationTable<proc_name, stmt_index>& procS_table,
	std::vector<StmtInfo> v) :
	next_table(&next_table),
	modifiesS_table(&modifiesS_table),
	useS_table(&useS_table),
	procS_table(&procS_table),
	stmt_info_list(v) {
	int size = stmt_info_list.size();
	kill_list.resize(size, {});
	gen_list.resize(size, {});
	in_list.resize(size, {});
	out_list.resize(size, {});
	pred_list.resize(size, {});
	succ_list.resize(size, {});
}

std::pair<std::set<stmt_index>, std::vector<std::pair<StmtInfo, StmtInfo>>> IterativeDataflowSolver::solve(std::vector<stmt_index> starting_worklist) {
	populateDataflowSets();
	std::queue<stmt_index> worklist;
	std::set<stmt_index> visited{};

	for (auto& stmt_index : starting_worklist) {
		worklist.push(stmt_index);
	}

	while (!worklist.empty()) {
		stmt_index curr = worklist.front();
		visited.emplace(curr);
		worklist.pop();
		int index = curr - 1;
		int old_out_size = out_list[curr - 1].size();

		processInSet(index);
		processOutSet(index);

		if (out_list[index].size() != old_out_size) {
			for (auto& succ : succ_list[index]) {
				worklist.push(succ);
			}
		}
	}
	resetOutList();
	return { visited, findResults(visited) };
}

bool IterativeDataflowSolver::solveIfAffectingAndAffected(stmt_index affecting, stmt_index affected) {
	populateDataflowSets();
	std::queue<stmt_index> worklist;
	std::set<stmt_index> visited{};

	worklist.push(affecting);

	while (!worklist.empty()) {
		stmt_index curr = worklist.front();
		visited.emplace(curr);
		worklist.pop();
		int index = curr - 1;
		int old_out_size = out_list[curr - 1].size();

		processInSet(index);

		// Early termination if Affects(affecting, affected) is found
		StmtInfo curr_stmt_info = stmt_info_list[index];
		if (curr_stmt_info.stmt_type != STMT_ASSIGN) {
		}
		else {
			if (curr == affected) {
				for (ModifiesTuple tuple : in_list[index]) {
					if (tuple.stmt_index == affecting) {
						StmtInfo stmt_info = stmt_info_list[index];
						if (useS_table->containsPair(stmt_info, tuple.var_name)) {
							resetOutList();
							return true;
						}
					}
				}
			}
		}

		processOutSet(index);

		if (out_list[index].size() != old_out_size) {
			for (auto& succ : succ_list[index]) {
				worklist.push(succ);
			}
		}
	}
	resetOutList();
	return false;
}

bool IterativeDataflowSolver::solveIfAffecting(stmt_index affecting) {
	populateDataflowSets();
	std::queue<stmt_index> worklist;
	std::set<stmt_index> visited{};

	worklist.push(affecting);

	while (!worklist.empty()) {
		stmt_index curr = worklist.front();
		visited.emplace(curr);
		worklist.pop();
		int index = curr - 1;
		int old_out_size = out_list[curr - 1].size();

		processInSet(index);

		// Early termination if Affects(affecting, _) is found
		StmtInfo curr_stmt_info = stmt_info_list[index];
		if (curr_stmt_info.stmt_type == STMT_ASSIGN) {
			for (ModifiesTuple tuple : in_list[index]) {
				if (tuple.stmt_index == affecting) {
					if (useS_table->containsPair(curr_stmt_info, tuple.var_name)) {
						resetOutList();
						return true;
					}
				}
			}
		}

		processOutSet(index);

		if (out_list[index].size() != old_out_size) {
			for (auto& succ : succ_list[index]) {
				worklist.push(succ);
			}
		}
	}
	resetOutList();
	return false;
}

bool IterativeDataflowSolver::solveIfAffected(stmt_index affected) {
	populateDataflowSets();
	std::queue<stmt_index> worklist;
	std::set<stmt_index> visited{};

	proc_name proc = procS_table->getKeys(affected)[0];
	stmt_index first_statement = procS_table->getValues(proc)[0];

	worklist.push(first_statement);

	while (!worklist.empty()) {
		stmt_index curr = worklist.front();
		visited.emplace(curr);
		worklist.pop();
		int index = curr - 1;
		int old_out_size = out_list[curr - 1].size();

		processInSet(index);

		// Early termination if a tuple in affected's in set is used by affected
		if (curr == affected) {
			StmtInfo curr_stmt_info = stmt_info_list[index];
			if (curr_stmt_info.stmt_type == STMT_ASSIGN) {
				for (ModifiesTuple tuple : in_list[index]) {
					StmtInfo stmt_info = stmt_info_list[index];
					if (useS_table->containsPair(stmt_info, tuple.var_name)) {
						resetOutList();
						return true;
					}
				}
			}
		}

		processOutSet(index);

		if (out_list[index].size() != old_out_size) {
			for (auto& succ : succ_list[index]) {
				worklist.push(succ);
			}
		}
	}
	resetOutList();
	return false;
}

bool IterativeDataflowSolver::solveIfNonEmpty(std::vector<stmt_index> first_statements) {
	populateDataflowSets();
	std::queue<stmt_index> worklist;
	std::set<stmt_index> visited{};

	proc_name proc = procS_table->getKeys()[0];
	for (auto& stmt_index : first_statements) {
		worklist.push(stmt_index);
	}

	while (!worklist.empty()) {
		stmt_index curr = worklist.front();
		visited.emplace(curr);
		worklist.pop();
		int index = curr - 1;
		int old_out_size = out_list[curr - 1].size();

		processInSet(index);

		// Early termination if a tuple in the current statement's in set is used by it
		for (ModifiesTuple tuple : in_list[index]) {
			StmtInfo stmt_info = stmt_info_list[index];
			if (useS_table->containsPair(stmt_info, tuple.var_name)) {
				resetOutList();
				return true;
			}
		}

		processOutSet(index);

		if (out_list[index].size() != old_out_size) {
			for (auto& succ : succ_list[index]) {
				worklist.push(succ);
			}
		}
	}
	resetOutList();
	return false;
}

std::vector<std::pair<StmtInfo, StmtInfo>> IterativeDataflowSolver::findResults(std::set<stmt_index> visited) {
	std::vector<std::pair<StmtInfo, StmtInfo>> res;
	for (auto& affected_index : visited) {
		StmtInfo statement_affected = stmt_info_list[affected_index - 1];
		if (statement_affected.stmt_type != STMT_ASSIGN) {
			continue;
		}
		for (auto& tuple : in_list[affected_index - 1]) {
			StmtInfo statement_affecting = stmt_info_list[tuple.stmt_index - 1];
			for (auto& var_used : useS_table->getValues(statement_affected)) {
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
			std::vector<var_name> var_modified_by_s = modifiesS_table->getValues(s);
			for (auto& var : var_modified_by_s) {
				std::vector<StmtInfo> other_stmts_modify_var = modifiesS_table->getKeys(var);
				for (auto& other_stmt : other_stmts_modify_var) {
					kill_list[i].emplace(ModifiesTuple{ other_stmt.stmt_index, var });
				}
				if (s.stmt_type == STMT_ASSIGN) {
					gen_list[i].emplace(ModifiesTuple{ s.stmt_index, var });
				}
			}
		}
		std::vector<StmtInfo> predecessor_stmts = next_table->getKeys(s);
		for (auto& pred : predecessor_stmts) {
			pred_list[s.stmt_index - 1].emplace(pred.stmt_index);
		}
		std::vector<StmtInfo> successor_stmts = next_table->getValues(s);
		for (auto& succ : successor_stmts) {
			succ_list[s.stmt_index - 1].emplace(succ.stmt_index);
		}
	}
	is_dataflow_sets_populated = true;
}

void IterativeDataflowSolver::processInSet(int index) {
	std::set<ModifiesTuple> new_in_list{};

	for (stmt_index pred : pred_list[index]) {
		std::set_union(out_list[pred - 1].begin(), out_list[pred - 1].end(),
			in_list[index].begin(), in_list[index].end(),
			std::inserter(new_in_list, new_in_list.begin()));
	}

	in_list[index] = new_in_list;
}

void IterativeDataflowSolver::processOutSet(int index) {
	std::set<ModifiesTuple> new_out_list{};

	std::set_difference(in_list[index].begin(), in_list[index].end(),
		kill_list[index].begin(), kill_list[index].end(),
		std::inserter(new_out_list, new_out_list.begin()));

	std::set_union(new_out_list.begin(), new_out_list.end(),
		gen_list[index].begin(), gen_list[index].end(),
		std::inserter(out_list[index], out_list[index].begin()));
}

void IterativeDataflowSolver::resetOutList() {
	auto size = out_list.size();
	out_list.resize(0);
	out_list.resize(size, {});
}

void IterativeDataflowSolver::resetInList() {
	auto size = in_list.size();
	in_list.resize(0);
	in_list.resize(size, {});
}

void IterativeDataflowSolver::reset() {
	resetOutList();
	resetInList();
}