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

	bool isNextBipEmpty();
	bool isNextBip(prog_line index1, prog_line index2);
	bool isPreviousBip(prog_line index);
	bool isNextBip(prog_line index);
	std::vector<std::pair<StmtInfo, StmtInfo>> getAllNextBipRelation();
	std::vector<StmtInfo> getPreviousBip();
	std::vector<StmtInfo> getNextBip();
	std::vector<StmtInfo> getPreviousBip(prog_line index);
	std::vector<StmtInfo> getNextBip(prog_line index);

	bool isNextBipTEmpty();
	bool isNextBipT(prog_line index1, prog_line index2);
	bool isPreviousBipT(prog_line index);
	bool isNextBipT(prog_line index);
	std::vector<std::pair<StmtInfo, StmtInfo>> getAllNextBipTRelation();
	std::vector<StmtInfo> getPreviousBipT();
	std::vector<StmtInfo> getNextBipT();
	std::vector<StmtInfo> getPreviousBipT(prog_line index);
	std::vector<StmtInfo> getNextBipT(prog_line index);

	bool isAffectsBipEmpty();
	bool isAffectsBip(stmt_index index1, stmt_index index2);
	bool isAffectingBip(stmt_index index);
	bool isAffectedBip(stmt_index index);
	std::vector<std::pair<StmtInfo, StmtInfo>> getAllAffectsBipRelation();
	std::vector<StmtInfo> getAffectedBip();
	std::vector<StmtInfo> getAffectingBip();
	std::vector<StmtInfo> getAffectedBip(stmt_index index);
	std::vector<StmtInfo> getAffectingBip(stmt_index index);

	bool isAffectsBipTEmpty();
	bool isAffectsBipT(stmt_index index1, stmt_index index2);
	bool isAffectingBipT(stmt_index index);
	bool isAffectedBipT(stmt_index index);
	std::vector<std::pair<StmtInfo, StmtInfo>> getAllAffectsBipTRelation();
	std::vector<StmtInfo> getAffectedBipT();
	std::vector<StmtInfo> getAffectingBipT();
	std::vector<StmtInfo> getAffectedBipT(stmt_index index);
	std::vector<StmtInfo> getAffectingBipT(stmt_index index);

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
