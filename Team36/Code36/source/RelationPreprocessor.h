#pragma once

#include "PKB.h"

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

	void setDFSForwardTrue(int);
	void setDFSBackwardTrue(int);
	bool isFullyPopulated();
	bool isCacheInitialized();
	const RelationTable<StmtInfo, StmtInfo>& getCache();
	std::vector<std::vector<bool>> getCalculatedMatrix();
	std::vector<bool> getDFSForwardComputedList();
	std::vector<bool> getDFSBackwardComputedList();

protected:
	bool is_cache_initialized = false;
	bool is_fully_populated = false;
	RelationTable<StmtInfo, StmtInfo> cache;
	std::vector<std::vector<bool>> calculated_matrix;
	std::vector<bool> calculated_dfs_forward;
	std::vector<bool> calculated_dfs_backward;
};