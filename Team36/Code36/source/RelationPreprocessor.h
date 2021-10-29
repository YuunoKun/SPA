#pragma once

#include "PKB.h"

enum BooleanStatus {
	STATUS_UNKNOWN = 2,
	STATUS_TRUE = 1,
	STATUS_FALSE = 0
};

class RelationPreprocessor {
public:
	virtual bool evaluateWildAndWild() = 0;
	virtual bool evaluateConstantAndWild(int) = 0;
	virtual bool evaluateWildAndConstant(int) = 0;
	virtual bool evaluateConstantAndConstant(int, int) = 0;
	virtual std::vector<std::pair<StmtInfo, StmtInfo>> evaluateSynonymAndSynonym() = 0;
	virtual std::vector<StmtInfo> evaluateWildAndSynonym() = 0;
	virtual std::vector<StmtInfo> evaluateSynonymAndWild() = 0;
	virtual std::vector<StmtInfo> evaluateConstantAndSynonym(int) = 0;
	virtual std::vector<StmtInfo> evaluateSynonymAndConstant(int) = 0;

	virtual void reset();
	virtual void fullyPopulate() = 0;

	void setDFSForwardTrue(int);
	void setDFSBackwardTrue(int);
	bool isFullyPopulated();
	const MonotypeRelationTable<StmtInfo>& getCache();
	bool isCacheEmpty();
	std::vector<std::vector<bool>> getCalculatedMatrix();
	bool isDFSForwardComputed(int);
	bool isDFSBackwardComputed(int);

protected:
	bool is_fully_populated = false;
	MonotypeRelationTable<StmtInfo> cache;
	std::vector<std::vector<bool>> calculated_matrix;
	std::vector<bool> calculated_dfs_forward;
	std::vector<bool> calculated_dfs_backward;
};