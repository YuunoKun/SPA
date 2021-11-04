#pragma once
#include <iostream>

#include "PKB.h"
#include "Common.h"
#include "RelationTable.cpp"
#include "RelationPreprocessor.h"

class NextTPreprocessor : public RelationPreprocessor
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
	void fullyPopulate() override;

	NextTPreprocessor(const MonotypeRelationTable<StmtInfo>&, const std::vector<StmtInfo>);
	NextTPreprocessor() = default;

private:
	const MonotypeRelationTable<StmtInfo>* next_table = nullptr;
};
