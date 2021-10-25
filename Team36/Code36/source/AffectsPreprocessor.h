#pragma once
#include <iostream>
#include <set>

#include "PKB.h"
#include "Common.h"
#include "Stmt.h"
#include "RelationTable.h"
#include "RelationTable.cpp"
#include "RelationPreprocessor.h"

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

class AffectsPreprocessor : public RelationPreprocessor
{
public:
	bool evaluateWildAndWild() override;
	bool evaluateConstantAndWild(int) override;
	bool evaluateWildAndConstant(int) override;
	bool evaluateConstantAndConstant(int, int) override;
	std::vector<std::pair<StmtInfo, StmtInfo>> evaluateSynonymAndSynonym() override;
	std::vector<StmtInfo> evaluateWildAndSynonym() override;
	std::vector<StmtInfo> evaluateSynonymAndWild() override;
	std::vector<StmtInfo> evaluateConstantAndSynonym(int) override;
	std::vector<StmtInfo> evaluateSynonymAndConstant(int) override;

	std::vector<StmtInfo> stmt_info_list;
	std::vector<std::set<ModifiesTuple>> kill_list;
	std::vector<std::set<ModifiesTuple>> gen_list;
	std::vector<std::set<ModifiesTuple>> in_list;
	std::vector<std::set<ModifiesTuple>> out_list;
	std::vector<std::set<stmt_index>> pred_list;
	std::vector<std::set<stmt_index>> succ_list;

	AffectsPreprocessor(
		const RelationTable<StmtInfo, StmtInfo>& next_table,
		const RelationTable<StmtInfo, var_name>& useS_table,
		const RelationTable<StmtInfo, var_name>& modifiesS_table,
		const RelationTable<proc_name, stmt_index>& procS_table,
		const std::vector<StmtInfo> v);
	AffectsPreprocessor() = default;
private:
	void iterativeDataFlowAnalysis(std::vector<StmtInfo>);
	void populateDataflowSets();
	void updateCache(std::vector<StmtInfo>);

	bool is_dataflow_sets_populated = false;
	const RelationTable<StmtInfo, StmtInfo>& next_table;
	const RelationTable<StmtInfo, var_name>& useS_table;
	const RelationTable<StmtInfo, var_name>& modifiesS_table;
	const RelationTable<proc_name, stmt_index>& procS_table;
};
