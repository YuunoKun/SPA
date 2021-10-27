#include "CFGRelationsManager.h"

#include "NextTPreprocessor.h"

void CFGRelationsManager::reset() {
	next_t_processor.reset();
	affects_processor.reset();
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
	bool isNextTCalculated = next_t_processor.isFullyPopulated() || next_t_processor.getCalculatedMatrix()[index1 - 1][index2 - 1];
	if (isNextTCalculated && !isNextT(index1, index2)) {
		return false;
	}
	else {
		return affects_processor.evaluateConstantAndConstant(index1, index2);
	}
}

bool CFGRelationsManager::isAffector(stmt_index index) {
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

std::vector<StmtInfo> CFGRelationsManager::getAffector() {
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

std::vector<StmtInfo> CFGRelationsManager::getAffector(stmt_index index) {
	auto& stmts = PKB::getInstance().getStmts();
	if (stmts[index - 1].stmt_type != STMT_ASSIGN) {
		return std::vector<StmtInfo>{};
	}
	else {
		return affects_processor.evaluateSynonymAndConstant(index);
	}
}

bool CFGRelationsManager::isAffectsTEmpty() {
	return false;
}

bool CFGRelationsManager::isAffectsT(stmt_index index1, stmt_index index2) {
	return false;
}

bool CFGRelationsManager::isAffectorT(stmt_index index) {
	return false;
}

bool CFGRelationsManager::isAffectedT(stmt_index index) {
	return false;
}

std::vector<std::pair<StmtInfo, StmtInfo>> CFGRelationsManager::getAllAffectsTRelation() {
	return std::vector<std::pair<StmtInfo, StmtInfo>>();
}

std::vector<StmtInfo> CFGRelationsManager::getAffectedT() {
	return std::vector<StmtInfo>();
}

std::vector<StmtInfo> CFGRelationsManager::getAffectorT() {
	return std::vector<StmtInfo>();
}

std::vector<StmtInfo> CFGRelationsManager::getAffectedT(stmt_index index) {
	return std::vector<StmtInfo>();
}

std::vector<StmtInfo> CFGRelationsManager::getAffectorT(stmt_index index) {
	return std::vector<StmtInfo>();
}

NextTPreprocessor CFGRelationsManager::getNextTProcessor() {
	return next_t_processor;
}

AffectsPreprocessor CFGRelationsManager::getAffectsProcessor() {
	return affects_processor;
}