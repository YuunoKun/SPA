#pragma once
#include <iostream>
#include <set>

#include "PKB.h"
#include "Common.h"
#include "RelationTable.h"
#include "RelationTable.cpp"
#include "RelationPreprocessor.h"
#include "IterativeDataflowSolverBip.h"

class AffectsBipPreprocessor : public RelationPreprocessor
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
	const MonotypeRelationTable<LabelledProgLine>& getLabelledProgLineCache();

	void reset();
	void fullyPopulate();

	AffectsBipPreprocessor(
		const MonotypeRelationTable<LabelledProgLine>& next_table,
		const RelationTable<StmtInfo, var_name>& useS_table,
		const RelationTable<StmtInfo, var_name>& modifiesS_table,
		const RelationTable<proc_name, stmt_index>& procS_table,
		std::vector<LabelledProgLine>& first_proglines,
		const std::vector<StmtInfo> v);
	AffectsBipPreprocessor() = default;
private:
	MonotypeRelationTable<LabelledProgLine> labelled_progline_cache;
	std::vector<LabelledProgLine> first_labelled_proglines;

	void updateCache(std::vector<std::pair<LabelledProgLine, LabelledProgLine>> results);
	std::vector<stmt_index> getAssignments();

	const MonotypeRelationTable<LabelledProgLine>* next_table = nullptr;
	const RelationTable<StmtInfo, var_name>* useS_table = nullptr;
	const RelationTable<StmtInfo, var_name>* modifiesS_table = nullptr;
	const RelationTable<proc_name, stmt_index>* procS_table = nullptr;
	IterativeDataflowSolverBip solver;
};
