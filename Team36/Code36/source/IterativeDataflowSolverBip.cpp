#include "IterativeDataflowSolverBip.h"

IterativeDataflowSolverBip::IterativeDataflowSolverBip(
	const MonotypeRelationTable<LabelledProgLine>& next_table,
	const RelationTable<StmtInfo, var_name>& useS_table,
	const RelationTable<StmtInfo, var_name>& modifiesS_table,
	std::vector<StmtInfo> v,
	std::vector<LabelledProgLine>& first_proglines) :
	next_table(&next_table),
	modifiesS_table(&modifiesS_table),
	useS_table(&useS_table),
	stmt_info_list(v),
	first_proglines(first_proglines) {
	std::vector<LabelledProgLine> keys = next_table.getKeys();
	std::vector<LabelledProgLine> values = next_table.getValues();
	std::sort(keys.begin(), keys.end());
	std::sort(values.begin(), values.end());
	std::set_union(keys.begin(), keys.end(),
		values.begin(), values.end(),
		std::inserter(labelled_progline_list, labelled_progline_list.begin()));
	int size = stmt_info_list.size();
	kill_list.resize(size, {});
}

std::vector<std::pair<LabelledProgLine, LabelledProgLine>> IterativeDataflowSolverBip::solve() {
	populateDataflowSets();
	std::queue<LabelledProgLine> worklist;
	std::set<LabelledProgLine> visited{};

	for (auto& LabelledProgLine : first_proglines) {
		worklist.push(LabelledProgLine);
	}

	while (!worklist.empty()) {
		LabelledProgLine curr = worklist.front();
		visited.emplace(curr);
		worklist.pop();
		int old_out_size = out_list[curr].size();

		processInSet(curr);
		processOutSet(curr);

		if (out_list[curr].size() != old_out_size) {
			for (auto& succ : succ_list[curr]) {
				worklist.push(succ);
			}
		}
	}
	resetOutList();
	return findResults();
}

std::vector<std::pair<LabelledProgLine, LabelledProgLine>> IterativeDataflowSolverBip::findResults() {
	std::vector<std::pair<LabelledProgLine, LabelledProgLine>> res;
	for (auto& progline : labelled_progline_list) {
		StmtInfo statement_affected = stmt_info_list[progline.program_line - 1];
		if (statement_affected.stmt_type != STMT_ASSIGN) {
			continue;
		}
		for (auto& tuple : in_list[progline]) {
			LabelledProgLine affecting_progline = tuple.labelled_prog_line;
			StmtInfo statement_affecting = stmt_info_list[affecting_progline.program_line - 1];
			for (auto& var_used : useS_table->getValues(statement_affected)) {
				bool var_matched = tuple.var_name == var_used;
				if (var_matched) {
					res.push_back({ affecting_progline, progline });
					break;
				}
			}
		}
	}
	return res;
}

std::vector<LabelledProgLine> IterativeDataflowSolverBip::getProgLines(stmt_index index) {
	std::vector<LabelledProgLine> res;
	for (auto& labelled_progline : labelled_progline_list) {
		if (labelled_progline.program_line == index) {
			res.push_back(labelled_progline);
		}
	}
	return res;
}

void IterativeDataflowSolverBip::updateKillGenList(stmt_index index, var_name var, bool is_assign) {
	std::vector<StmtInfo> other_stmts_modify_var = modifiesS_table->getKeys(var);
	std::vector<LabelledProgLine> curr_prog_lines = getProgLines(index);
	for (auto& other_stmt : other_stmts_modify_var) {
		std::vector<LabelledProgLine> other_prog_lines = getProgLines(other_stmt.stmt_index);
		for (auto& other_prog_line : other_prog_lines) {
			if (stmt_info_list[other_prog_line.program_line - 1].stmt_type == STMT_ASSIGN)
				kill_list[index - 1].emplace(LabelledModifiesTuple{ other_prog_line, var });
		}
	}
	if (is_assign) {
		for (auto& curr_prog_line : curr_prog_lines) {
			gen_list[curr_prog_line].emplace(LabelledModifiesTuple{ curr_prog_line, var });
		}
	}
}

void IterativeDataflowSolverBip::populateDataflowSets() {
	if (is_dataflow_sets_populated == true) {
		return;
	}

	for (auto& s : stmt_info_list) {
		if (s.stmt_type == STMT_ASSIGN || s.stmt_type == STMT_READ) {
			std::vector<var_name> var_modified_by_s = modifiesS_table->getValues(s);
			for (auto& var : var_modified_by_s) {
				bool is_assign = s.stmt_type == STMT_ASSIGN;
				updateKillGenList(s.stmt_index, var, is_assign);
			}
		}
	}
	for (auto& progline : labelled_progline_list) {
		std::vector<LabelledProgLine> pred_proglines = next_table->getKeys(progline);
		for (auto& pred : pred_proglines) {
			pred_list[progline].emplace(pred);
		}
		std::vector<LabelledProgLine> succ_proglines = next_table->getValues(progline);
		for (auto& succ : succ_proglines) {
			succ_list[progline].emplace(succ);
		}
	}
	is_dataflow_sets_populated = true;
}

void IterativeDataflowSolverBip::processInSet(LabelledProgLine index) {
	std::set<LabelledModifiesTuple> new_in_list{};

	for (LabelledProgLine pred : pred_list[index]) {
		std::set_union(out_list[pred].begin(), out_list[pred].end(),
			in_list[index].begin(), in_list[index].end(),
			std::inserter(new_in_list, new_in_list.begin()));
	}

	in_list[index] = new_in_list;
}

void IterativeDataflowSolverBip::processOutSet(LabelledProgLine index) {
	std::set<LabelledModifiesTuple> new_out_list{};

	std::set_difference(in_list[index].begin(), in_list[index].end(),
		kill_list[index.program_line - 1].begin(), kill_list[index.program_line - 1].end(),
		std::inserter(out_list[index], out_list[index].begin()));

	//StmtInfo s = stmt_info_list[index.program_line];
	//if (s.stmt_type == STMT_ASSIGN) {
	//	LabelledModifiesTuple tuple = { index, modifiesS_table->getValues(s)[0] };
	//	out_list[index].insert(tuple);
	//}

	std::set_union(new_out_list.begin(), new_out_list.end(),
		gen_list[index].begin(), gen_list[index].end(),
		std::inserter(out_list[index], out_list[index].begin()));
}

void IterativeDataflowSolverBip::resetOutList() {
	out_list.clear();
}

void IterativeDataflowSolverBip::resetInList() {
	in_list.clear();
}

void IterativeDataflowSolverBip::reset() {
	resetOutList();
	resetInList();
}