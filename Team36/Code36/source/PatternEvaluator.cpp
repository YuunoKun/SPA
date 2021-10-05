#include "PatternEvaluator.h"
#include <stdexcept>

void PatternEvaluator::evaluatePattern(QueryResult& queryResult, Pattern& pattern) {
	if (pattern.getPatternType().getType() != ASSIGN) {
		throw std::invalid_argument("evaluatePattern(): only ASSIGN pattern type is allowed for iteration 1");
	}

	std::vector<assign_info> assignInfos;
	if (pattern.getExpression() == nullptr && pattern.isWild()) {
		assignInfos = pkb.getAssignInfo();
	}
	else {
		assignInfos = pkb.getAssignInfo(pattern.getExpression(), pattern.isWild());
	}

	Entity patternType = pattern.getPatternType();
	Entity lhsEntity = pattern.getLeftExpression();
	expr* expression = pattern.getExpression();

	//If left side is wild and right side is WILD: e.g a(v, _"x"_)
	//if lhs side is at least is declaration, return 2 column table
	if (lhsEntity.isSynonym()) {
		std::pair<Entity, Entity> header = { patternType, lhsEntity };
		ResultTable resultTable(header, assignInfos);
		queryResult.addResult(resultTable);
	}
	else {
		//if lhs side is not declaration
		if (lhsEntity.getType() == WILD) {
			//If left side is wild and right side is WILD: e.g a(_, _"x"_)
			std::vector<StmtInfo> table;
			for (auto it : assignInfos) {
				table.push_back(it.first);
			}
			ResultTable resultTable(patternType, table);
			queryResult.addResult(resultTable);
		}
		else {
			//If left side is constant and right side is normal: e.g a("x", "x")
			//return all assign statment that match expression and lhs
			std::vector<StmtInfo> table;
			for (auto it : assignInfos) {
				if (it.second == lhsEntity.getValue()) {
					table.push_back(it.first);
				}
			}
			ResultTable resultTable(patternType, table);
			queryResult.addResult(resultTable);
		}
	}
}