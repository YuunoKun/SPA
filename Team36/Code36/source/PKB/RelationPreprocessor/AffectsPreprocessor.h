#pragma once
#include <iostream>
#include <set>

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
	std::vector<BooleanStatus> getAffecting();
	std::vector<BooleanStatus> getAffected();
	BooleanStatus isNonEmpty();

	void reset();
	void fullyPopulate();

	AffectsPreprocessor(
		const MonotypeRelationTable<StmtInfo>& next_table,
		const RelationTable<StmtInfo, var_name>& useS_table,
		const RelationTable<StmtInfo, var_name>& modifiesS_table,
		const RelationTable<proc_name, stmt_index>& procS_table,
		const std::vector<StmtInfo> v);
	AffectsPreprocessor() = default;
private:
	void updateCache(std::set<stmt_index>, std::vector<std::pair<StmtInfo, StmtInfo>>);
	std::vector<stmt_index> getAssignments(proc_name proc);

	const MonotypeRelationTable<StmtInfo>* next_table = nullptr;
	const RelationTable<StmtInfo, var_name>* useS_table = nullptr;
	const RelationTable<StmtInfo, var_name>* modifiesS_table = nullptr;
	const RelationTable<proc_name, stmt_index>* procS_table = nullptr;
	IterativeDataflowSolver solver;

	BooleanStatus is_non_empty = STATUS_UNKNOWN;
	std::vector<BooleanStatus> is_affecting{};
	std::vector<BooleanStatus> is_affected{};
};
