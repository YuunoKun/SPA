#pragma once
#include <iostream>
#include <set>

#include "PKB.h"
#include "Common.h"
#include "Stmt.h"
#include "RelationTable.h"
#include "RelationTable.cpp"
#include "RelationPreprocessor.h"
#include "IterativeDataflowSolver.h"

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

	AffectsPreprocessor(
		const RelationTable<StmtInfo, StmtInfo>& next_table,
		const RelationTable<StmtInfo, var_name>& useS_table,
		const RelationTable<StmtInfo, var_name>& modifiesS_table,
		const RelationTable<proc_name, stmt_index>& procS_table,
		const std::vector<StmtInfo>& v);
	AffectsPreprocessor() = default;
private:
	void updateCache(std::set<stmt_index>, std::vector<std::pair<StmtInfo, StmtInfo>>);

	const std::vector<StmtInfo>& stmt_info_list;
	const RelationTable<StmtInfo, StmtInfo>& next_table;
	const RelationTable<StmtInfo, var_name>& useS_table;
	const RelationTable<StmtInfo, var_name>& modifiesS_table;
	const RelationTable<proc_name, stmt_index>& procS_table;
	IterativeDataflowSolver solver{ next_table , useS_table, modifiesS_table, procS_table, stmt_info_list };
};
