#pragma once
#include <iostream>

#include "PKB.h"
#include "Common.h"
#include "RelationTable.h"
#include "RelationTable.cpp"
#include "NextTPreprocessor.h"
#include "AffectsPreprocessor.h"
#include "AffectsTPreprocessor.h"

class CFGRelationsManager {
public:
	CFGRelationsManager();
	void update();
	void reset();

	bool isNextTEmpty();
	bool isNextT(prog_line index1, prog_line index2);
	bool isPreviousT(prog_line index);
	bool isNextT(prog_line index);
	std::vector<std::pair<StmtInfo, StmtInfo>> getAllNextTRelation();
	std::vector<StmtInfo> getPreviousT();
	std::vector<StmtInfo> getNextT();
	std::vector<StmtInfo> getPreviousT(prog_line index);
	std::vector<StmtInfo> getNextT(prog_line index);

	bool isAffectsEmpty();
	bool isAffects(stmt_index index1, stmt_index index2);
	bool isAffecting(stmt_index index);
	bool isAffected(stmt_index index);
	std::vector<std::pair<StmtInfo, StmtInfo>> getAllAffectsRelation();
	std::vector<StmtInfo> getAffected();
	std::vector<StmtInfo> getAffecting();
	std::vector<StmtInfo> getAffected(stmt_index index);
	std::vector<StmtInfo> getAffecting(stmt_index index);

	bool isAffectsTEmpty();
	bool isAffectsT(stmt_index index1, stmt_index index2);
	bool isAffectingT(stmt_index index);
	bool isAffectedT(stmt_index index);
	std::vector<std::pair<StmtInfo, StmtInfo>> getAllAffectsTRelation();
	std::vector<StmtInfo> getAffectedT();
	std::vector<StmtInfo> getAffectingT();
	std::vector<StmtInfo> getAffectedT(stmt_index index);
	std::vector<StmtInfo> getAffectingT(stmt_index index);

	NextTPreprocessor getNextTProcessor();
	AffectsPreprocessor getAffectsProcessor();
	AffectsTPreprocessor getAffectsTProcessor();

private:
	NextTPreprocessor next_t_processor = NextTPreprocessor(PKB::getInstance().getNext(), PKB::getInstance().getStmts());
	AffectsPreprocessor affects_processor = AffectsPreprocessor(
		PKB::getInstance().getNext(), PKB::getInstance().getUsesS(), PKB::getInstance().getModifiesS(),
		PKB::getInstance().getProcContains(), PKB::getInstance().getStmts());
	AffectsTPreprocessor affectsT_processor = AffectsTPreprocessor(
		affects_processor.getCache(), PKB::getInstance().getStmts());
};
