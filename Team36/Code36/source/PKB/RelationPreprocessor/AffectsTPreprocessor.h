#pragma once
#include <iostream>

#include "RelationPreprocessor.h"
#include "PKB/RelationTables/RelationTable.cpp"

class AffectsTPreprocessor : public RelationPreprocessor
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

	AffectsTPreprocessor(const MonotypeRelationTable<StmtInfo>&, const std::vector<StmtInfo>);
	AffectsTPreprocessor() = default;

private:
	const MonotypeRelationTable<StmtInfo>* affects_table = nullptr;

	void checkCache();
};
