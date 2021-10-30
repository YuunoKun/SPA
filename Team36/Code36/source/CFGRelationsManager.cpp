#include "CFGRelationsManager.h"

CFGRelationsManager::CFGRelationsManager() {
	next_t_processor = NextTPreprocessor(PKB::getInstance().getNext(), PKB::getInstance().getStmts());
	affects_processor = AffectsPreprocessor(
		PKB::getInstance().getNext(), PKB::getInstance().getUsesS(), PKB::getInstance().getModifiesS(),
		PKB::getInstance().getProcContains(), PKB::getInstance().getStmts());
	affectsT_processor = AffectsTPreprocessor(
		affects_processor.getCache(), PKB::getInstance().getStmts());
}

void CFGRelationsManager::update() {
	next_t_processor = NextTPreprocessor(PKB::getInstance().getNext(), PKB::getInstance().getStmts());
	affects_processor = AffectsPreprocessor(
		PKB::getInstance().getNext(), PKB::getInstance().getUsesS(), PKB::getInstance().getModifiesS(),
		PKB::getInstance().getProcContains(), PKB::getInstance().getStmts());
	affectsT_processor = AffectsTPreprocessor(
		affects_processor.getCache(), PKB::getInstance().getStmts());
}

void CFGRelationsManager::reset() {
	next_t_processor.reset();
	affects_processor.reset();
	affectsT_processor.reset();
}

bool CFGRelationsManager::isNextTEmpty() {
	return !next_t_processor.evaluateWildAndWild();
}

bool CFGRelationsManager::isNextT(prog_line index1, prog_line index2) {
	auto& procS_table = PKB::getInstance().getProcContains();
	proc_name procedure_of_index1 = procS_table.getKeys(index1)[0];
	bool index1_index2_same_proc = procS_table.containsPair(procedure_of_index1, index2);
	if (index1_index2_same_proc == false) {
		return false;
	}
	else {
		return next_t_processor.evaluateConstantAndConstant(index1, index2);
	}
}

bool CFGRelationsManager::isPreviousT(prog_line index) {
	return next_t_processor.evaluateConstantAndWild(index);
}

bool CFGRelationsManager::isNextT(prog_line index) {
	return next_t_processor.evaluateWildAndConstant(index);
}

std::vector<std::pair<StmtInfo, StmtInfo>> CFGRelationsManager::getAllNextTRelation() {
	return next_t_processor.evaluateSynonymAndSynonym();
}

std::vector<StmtInfo> CFGRelationsManager::getPreviousT() {
	return next_t_processor.evaluateSynonymAndWild();
}

std::vector<StmtInfo> CFGRelationsManager::getNextT() {
	return next_t_processor.evaluateWildAndSynonym();
}

std::vector<StmtInfo> CFGRelationsManager::getPreviousT(prog_line index) {
	return next_t_processor.evaluateSynonymAndConstant(index);
}

std::vector<StmtInfo> CFGRelationsManager::getNextT(prog_line index) {
	return next_t_processor.evaluateConstantAndSynonym(index);
}

bool CFGRelationsManager::isAffectsEmpty() {
	return !affects_processor.evaluateWildAndWild();
}

bool CFGRelationsManager::isAffects(stmt_index index1, stmt_index index2) {
	bool is_nextT_calculated = next_t_processor.isFullyPopulated() || next_t_processor.getCalculatedMatrix()[index1 - 1][index2 - 1];
	if (is_nextT_calculated && !isNextT(index1, index2)) {
		return false;
	}
	else {
		return affects_processor.evaluateConstantAndConstant(index1, index2);
	}
}

bool CFGRelationsManager::isAffecting(stmt_index index) {
	return affects_processor.evaluateConstantAndWild(index);
}

bool CFGRelationsManager::isAffected(stmt_index index) {
	return affects_processor.evaluateWildAndConstant(index);
}

std::vector<std::pair<StmtInfo, StmtInfo>> CFGRelationsManager::getAllAffectsRelation() {
	return affects_processor.evaluateSynonymAndSynonym();
}

std::vector<StmtInfo> CFGRelationsManager::getAffected() {
	return affects_processor.evaluateWildAndSynonym();
}

std::vector<StmtInfo> CFGRelationsManager::getAffecting() {
	return affects_processor.evaluateSynonymAndWild();
}

std::vector<StmtInfo> CFGRelationsManager::getAffected(stmt_index index) {
	auto& stmts = PKB::getInstance().getStmts();
	if (stmts[index - 1].stmt_type != STMT_ASSIGN) {
		return std::vector<StmtInfo>{};
	}
	else {
		return affects_processor.evaluateConstantAndSynonym(index);
	}
}

std::vector<StmtInfo> CFGRelationsManager::getAffecting(stmt_index index) {
	auto& stmts = PKB::getInstance().getStmts();
	if (stmts[index - 1].stmt_type != STMT_ASSIGN) {
		return std::vector<StmtInfo>{};
	}
	else {
		return affects_processor.evaluateSynonymAndConstant(index);
	}
}

bool CFGRelationsManager::isAffectsTEmpty() {
	return isAffectsEmpty();
}

bool CFGRelationsManager::isAffectsT(stmt_index index1, stmt_index index2) {
	bool is_nextT_calculated = next_t_processor.isFullyPopulated() || next_t_processor.getCalculatedMatrix()[index1 - 1][index2 - 1];
	if (is_nextT_calculated && !isNextT(index1, index2)) {
		return false;
	}
	else {
		affects_processor.fullyPopulate();
		return affectsT_processor.evaluateConstantAndConstant(index1, index2);
	}
}

bool CFGRelationsManager::isAffectingT(stmt_index index) {
	return isAffecting(index);
}

bool CFGRelationsManager::isAffectedT(stmt_index index) {
	return isAffected(index);
}

std::vector<std::pair<StmtInfo, StmtInfo>> CFGRelationsManager::getAllAffectsTRelation() {
	affects_processor.fullyPopulate();
	return affectsT_processor.evaluateSynonymAndSynonym();
}

std::vector<StmtInfo> CFGRelationsManager::getAffectedT() {
	return getAffected();
}

std::vector<StmtInfo> CFGRelationsManager::getAffectingT() {
	return getAffecting();
}

std::vector<StmtInfo> CFGRelationsManager::getAffectedT(stmt_index index) {
	auto& stmts = PKB::getInstance().getStmts();
	if (stmts[index - 1].stmt_type != STMT_ASSIGN) {
		return std::vector<StmtInfo>{};
	}
	else {
		affects_processor.fullyPopulate();
		return affectsT_processor.evaluateConstantAndSynonym(index);
	}
}

std::vector<StmtInfo> CFGRelationsManager::getAffectingT(stmt_index index) {
	auto& stmts = PKB::getInstance().getStmts();
	if (stmts[index - 1].stmt_type != STMT_ASSIGN) {
		return std::vector<StmtInfo>{};
	}
	else {
		affects_processor.fullyPopulate();
		return affectsT_processor.evaluateSynonymAndConstant(index);
	}
}

NextTPreprocessor CFGRelationsManager::getNextTProcessor() {
	return next_t_processor;
}

AffectsPreprocessor CFGRelationsManager::getAffectsProcessor() {
	return affects_processor;
}

AffectsTPreprocessor CFGRelationsManager::getAffectsTProcessor() {
	return affectsT_processor;
}