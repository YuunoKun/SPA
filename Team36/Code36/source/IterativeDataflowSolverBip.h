#pragma once
#include <iostream>
#include <set>

#include "PKB.h"
#include "Common.h"
#include "RelationTable.h"
#include "RelationTable.cpp"
#include "MonotypeRelationTable.h"
#include "MonotypeRelationTable.cpp"

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
			// Compute individual hash values for two data members and combine them using XOR and bit shifting
			return ((hash<LabelledProgLine>()(k.labelled_prog_line) ^ (hash<std::string>()(k.var_name) << 1)) >> 1);
		}
	};
}

class IterativeDataflowSolverBip
{
public:
	std::vector<std::pair<LabelledProgLine, LabelledProgLine>> solve();
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
	void updateKillGenList(stmt_index index, var_name var, bool is_assign);

	std::vector<LabelledProgLine> getProgLines(stmt_index index);
	std::vector<std::pair<LabelledProgLine, LabelledProgLine>> findResults();

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

	const MonotypeRelationTable<LabelledProgLine>* next_table;
	const RelationTable<StmtInfo, var_name>* useS_table;
	const RelationTable<StmtInfo, var_name>* modifiesS_table;
};
