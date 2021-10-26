#pragma once
#include <iostream>
#include <set>

#include "PKB.h"
#include "Common.h"
#include "Stmt.h"
#include "RelationTable.h"
#include "RelationTable.cpp"

struct ModifiesTuple {
	stmt_index stmt_index;
	var_name var_name;

	bool operator==(const ModifiesTuple& modifies_tuple_2) const {
		return stmt_index == modifies_tuple_2.stmt_index && var_name == modifies_tuple_2.var_name;
	}

	bool operator < (const ModifiesTuple& modifies_tuple_2) const {
		return (stmt_index < modifies_tuple_2.stmt_index || stmt_index == modifies_tuple_2.stmt_index && var_name < modifies_tuple_2.var_name);
	}
};

namespace std {
	template <>
	struct hash<ModifiesTuple> {
		size_t operator()(const ModifiesTuple& k) const {
			// Compute individual hash values for two data members and combine them using XOR and bit shifting
			return ((hash<int>()(k.stmt_index) ^ (hash<std::string>()(k.var_name) << 1)) >> 1);
		}
	};
}

class IterativeDataflowSolver
{
public:
	std::set<stmt_index> getVisited();
	std::vector<std::pair<StmtInfo, StmtInfo>> solve(std::vector<stmt_index>);

	IterativeDataflowSolver(
		const RelationTable<StmtInfo, StmtInfo>& next_table_,
		const RelationTable<StmtInfo, var_name>& useS_table_,
		const RelationTable<StmtInfo, var_name>& modifiesS_table,
		const RelationTable<proc_name, stmt_index>& procS_table_,
		const std::vector<StmtInfo> v);

private:
	void populateDataflowSets();

	bool is_dataflow_sets_populated = false;
	std::set<stmt_index> visited{};

	std::vector<std::set<ModifiesTuple>> kill_list;
	std::vector<std::set<ModifiesTuple>> gen_list;
	std::vector<std::set<ModifiesTuple>> in_list;
	std::vector<std::set<ModifiesTuple>> out_list;
	std::vector<std::set<stmt_index>> pred_list;
	std::vector<std::set<stmt_index>> succ_list;

	const RelationTable<StmtInfo, StmtInfo>& next_table;
	const RelationTable<StmtInfo, var_name>& useS_table;
	const RelationTable<StmtInfo, var_name>& modifiesS_table;
	const RelationTable<proc_name, stmt_index>& procS_table;
	const std::vector<StmtInfo> stmt_info_list{};
};
