#pragma once
#include <iostream>
#include <set>

#include "PKB.h"
#include "Common.h"
#include "RelationTable.h"
#include "RelationTable.cpp"
#include "MonotypeRelationTable.h"

struct LabelledModifiesTuple {
	LabelledProgLine labelled_prog_line;
	var_name var_name;

	bool operator==(const LabelledModifiesTuple& modifies_tuple_2) const {
		return labelled_prog_line == modifies_tuple_2.labelled_prog_line && var_name == modifies_tuple_2.var_name;
	}

	bool operator < (const LabelledModifiesTuple& modifies_tuple_2) const {
		return (labelled_prog_line < modifies_tuple_2.labelled_prog_line || labelled_prog_line == modifies_tuple_2.labelled_prog_line && var_name < modifies_tuple_2.var_name);
	}
};

namespace std {
	template <>
	struct hash<LabelledModifiesTuple> {
		size_t operator()(const LabelledModifiesTuple& k) const {
			return ((hash<LabelledProgLine>()(k.labelled_prog_line) ^ (hash<std::string>()(k.var_name) << 1)) >> 1);
		}
	};
}

class IterativeDataflowSolverBip
{
public:
	std::vector<std::pair<LabelledProgLine, LabelledProgLine>> solve(std::vector<stmt_index> starting_worklist);
	void reset();

	IterativeDataflowSolverBip(
		const MonotypeRelationTable<LabelledProgLine>& next_table,
		const RelationTable<StmtInfo, var_name>& useS_table_,
		const RelationTable<StmtInfo, var_name>& modifiesS_table,
		std::vector<StmtInfo> v,
		std::vector<LabelledProgLine>& first_stmts);
	IterativeDataflowSolverBip() = default;

private:
	void populateDataflowSets();
	void processInSet(LabelledProgLine index);
	void processOutSet(LabelledProgLine index);
	void resetOutList();
	void resetInList();
	void updateKillList(stmt_index index, var_name var);
	void updateGenList(stmt_index index, var_name var);

	std::vector<LabelledProgLine> getAllProgLines();
	std::vector<LabelledProgLine> getProgLines(stmt_index index);
	StmtInfo getStmt(stmt_index index);
	void addSuccessorsToWorklist(LabelledProgLine index, std::deque<LabelledProgLine>& worklist, std::unordered_set<LabelledProgLine>& worklist_set);
	std::vector<std::pair<LabelledProgLine, LabelledProgLine>> findResults();
	bool checkIfTupleAffects(LabelledModifiesTuple, LabelledProgLine);

	std::vector<std::set<LabelledModifiesTuple>> kill_list;
	std::unordered_map<LabelledProgLine, std::set<LabelledModifiesTuple>> gen_list;
	std::unordered_map<LabelledProgLine, std::set<LabelledModifiesTuple>> in_list;
	std::unordered_map<LabelledProgLine, std::set<LabelledModifiesTuple>> out_list;
	std::unordered_map<LabelledProgLine, std::set<LabelledProgLine>> pred_list;
	std::unordered_map<LabelledProgLine, std::set<LabelledProgLine>> succ_list;

	std::vector<StmtInfo> stmt_info_list;
	std::vector<LabelledProgLine> labelled_progline_list;
	std::vector<LabelledProgLine> first_proglines;
	bool is_dataflow_sets_populated = false;

	const MonotypeRelationTable<LabelledProgLine>* next_table = nullptr;
	const RelationTable<StmtInfo, var_name>* useS_table = nullptr;
	const RelationTable<StmtInfo, var_name>* modifiesS_table = nullptr;
};
