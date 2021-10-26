#include "CFGRelationsManager.h"

#include "NextTPreprocessor.h"


CFGRelationsManager& CFGRelationsManager::getInstance() {
	static CFGRelationsManager relations_manager;
	return relations_manager;
}

void CFGRelationsManager::reset() {
	next_t_processor.reset();
}

bool CFGRelationsManager::isNextTEmpty() {
	return !next_t_processor.evaluateWildAndWild();
}

bool CFGRelationsManager::isNextT(prog_line index1, prog_line index2) {
	if (PKB::getInstance().inSameProc(index1, index2) == false) {
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
	return false;
}

bool CFGRelationsManager::isAffects(stmt_index index1, stmt_index index2) {
	return false;
}

bool CFGRelationsManager::isAffector(stmt_index index) {
	return false;
}

bool CFGRelationsManager::isAffected(stmt_index index) {
	return false;
}

std::vector<std::pair<StmtInfo, StmtInfo>> CFGRelationsManager::getAllAffectsRelation() {
	return std::vector<std::pair<StmtInfo, StmtInfo>>();
}

std::vector<StmtInfo> CFGRelationsManager::getAffected() {
	return std::vector<StmtInfo>();
}

std::vector<StmtInfo> CFGRelationsManager::getAffector() {
	return std::vector<StmtInfo>();
}

std::vector<StmtInfo> CFGRelationsManager::getAffected(stmt_index index) {
	return std::vector<StmtInfo>();
}

std::vector<StmtInfo> CFGRelationsManager::getAffector(stmt_index index) {
	return std::vector<StmtInfo>();
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