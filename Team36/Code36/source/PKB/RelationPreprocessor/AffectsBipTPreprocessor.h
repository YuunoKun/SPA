#pragma once
#include <iostream>

#include "RelationPreprocessor.h"
#include "PKB/RelationTables/RelationTable.cpp"

class AffectsBipTPreprocessor : public RelationPreprocessor
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

	AffectsBipTPreprocessor(const MonotypeRelationTable<StmtInfo>&, const MonotypeRelationTable<LabelledProgLine>&, const std::vector<StmtInfo>);
	AffectsBipTPreprocessor() = default;

private:
	const MonotypeRelationTable<StmtInfo>* affects_bip_table = nullptr;
	const MonotypeRelationTable<LabelledProgLine>* labelled_affects_bip_table = nullptr;
	MonotypeRelationTable<LabelledProgLine> labelled_cache;
};
