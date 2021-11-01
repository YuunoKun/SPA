#include "CFGRelationsManager.h"

CFGRelationsManager::CFGRelationsManager() {
	next_t_processor = NextTPreprocessor(PKB::getInstance().getNext(),
		PKB::getInstance().getStmts());
	affects_processor = AffectsPreprocessor(
		PKB::getInstance().getNext(),
		PKB::getInstance().getUsesS(),
		PKB::getInstance().getModifiesS(),
		PKB::getInstance().getProcContains(),
		PKB::getInstance().getStmts());
	affects_bip_processor = AffectsBipPreprocessor(
		getLabelledNextTable(),
		PKB::getInstance().getUsesS(),
		PKB::getInstance().getModifiesS(),
		PKB::getInstance().getProcContains(),
		getFirstProgs(),
		PKB::getInstance().getStmts());
	affectsT_processor = AffectsTPreprocessor(
		affects_processor.getCache(),
		PKB::getInstance().getStmts());
	affects_bipT_processor = AffectsBipTPreprocessor(
		affects_bip_processor.getCache(),
		affects_bip_processor.getLabelledProgLineCache(),
		PKB::getInstance().getStmts());
}

void CFGRelationsManager::update() {
	next_t_processor = NextTPreprocessor(PKB::getInstance().getNext(),
		PKB::getInstance().getStmts());
	affects_processor = AffectsPreprocessor(
		PKB::getInstance().getNext(),
		PKB::getInstance().getUsesS(),
		PKB::getInstance().getModifiesS(),
		PKB::getInstance().getProcContains(),
		PKB::getInstance().getStmts());
	affects_bip_processor = AffectsBipPreprocessor(
		getLabelledNextTable(),
		PKB::getInstance().getUsesS(),
		PKB::getInstance().getModifiesS(),
		PKB::getInstance().getProcContains(),
		getFirstProgs(),
		PKB::getInstance().getStmts());
	affectsT_processor = AffectsTPreprocessor(
		affects_processor.getCache(),
		PKB::getInstance().getStmts());
	affects_bipT_processor = AffectsBipTPreprocessor(
		affects_bip_processor.getCache(),
		affects_bip_processor.getLabelledProgLineCache(),
		PKB::getInstance().getStmts());
}

void CFGRelationsManager::reset() {
	next_t_processor.reset();
	affects_processor.reset();
	affectsT_processor.reset();
	affects_bip_processor.reset();
	affects_bipT_processor.reset();
}

bool CFGRelationsManager::isNextTEmpty() {
	return !next_t_processor.evaluateWildAndWild();
}

bool CFGRelationsManager::isNextT(prog_line index1, prog_line index2) {
	if (inSameProc(index1, index2) == false) {
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
	if (inSameProc(index1, index2) == false) {
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
	else if (inSameProc(index1, index2) == false) {
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

//Temp implementation
bool CFGRelationsManager::isNextBipEmpty() {
	return isNextTEmpty();
}

bool CFGRelationsManager::isNextBip(prog_line index1, prog_line index2) {
	auto a = PKB::getInstance().PKB::getNext();
	return a.containsPair(PKB::getInstance().PKB::getStmt(index1), PKB::getInstance().PKB::getStmt(index2));
}

bool CFGRelationsManager::isPreviousBip(prog_line index) {
	return isPreviousT(index);
}

bool CFGRelationsManager::isNextBip(prog_line index) {
	return isNextT(index);
}

std::vector<std::pair<StmtInfo, StmtInfo>> CFGRelationsManager::getAllNextBipRelation() {
	auto a = PKB::getInstance().PKB::getNext();
	return a.getPairs();
}

std::vector<StmtInfo> CFGRelationsManager::getPreviousBip() {
	return getPreviousT();
}

std::vector<StmtInfo> CFGRelationsManager::getNextBip() {
	return getNextT();
}

std::vector<StmtInfo> CFGRelationsManager::getPreviousBip(prog_line index) {
	auto a = PKB::getInstance().PKB::getNext();
	return a.getKeys(PKB::getInstance().PKB::getStmt(index));
}

std::vector<StmtInfo> CFGRelationsManager::getNextBip(prog_line index) {
	auto a = PKB::getInstance().PKB::getNext();
	return a.getValues(PKB::getInstance().PKB::getStmt(index));
}

bool CFGRelationsManager::isNextBipTEmpty() {
	return isNextTEmpty();
}

bool CFGRelationsManager::isNextBipT(prog_line index1, prog_line index2) {
	return isNextT(index1, index2);
}

bool CFGRelationsManager::isPreviousBipT(prog_line index) {
	return isPreviousT(index);
}

bool CFGRelationsManager::isNextBipT(prog_line index) {
	return isNextT(index);
}

std::vector<std::pair<StmtInfo, StmtInfo>> CFGRelationsManager::getAllNextBipTRelation() {
	return getAllNextTRelation();
}

std::vector<StmtInfo> CFGRelationsManager::getPreviousBipT() {
	return getPreviousT();
}

std::vector<StmtInfo> CFGRelationsManager::getNextBipT() {
	return getNextT();
}

std::vector<StmtInfo> CFGRelationsManager::getPreviousBipT(prog_line index) {
	return getPreviousT(index);
}

std::vector<StmtInfo> CFGRelationsManager::getNextBipT(prog_line index) {
	return getNextT(index);
}

bool CFGRelationsManager::isAffectsBipEmpty() {
	return !affects_bip_processor.evaluateWildAndWild();
}

bool CFGRelationsManager::isAffectsBip(stmt_index index1, stmt_index index2) {
	auto& stmts = PKB::getInstance().getStmts();
	if (stmts[index1 - 1].stmt_type != STMT_ASSIGN || stmts[index2 - 1].stmt_type != STMT_ASSIGN) {
		return false;
	}
	return affects_bip_processor.evaluateConstantAndConstant(index1, index2);
}

bool CFGRelationsManager::isAffectingBip(stmt_index index) {
	auto& stmts = PKB::getInstance().getStmts();
	if (stmts[index - 1].stmt_type != STMT_ASSIGN) {
		return false;
	}
	return affects_bip_processor.evaluateConstantAndWild(index);
}

bool CFGRelationsManager::isAffectedBip(stmt_index index) {
	auto& stmts = PKB::getInstance().getStmts();
	if (stmts[index - 1].stmt_type != STMT_ASSIGN) {
		return false;
	}
	return affects_bip_processor.evaluateWildAndConstant(index);
}

std::vector<std::pair<StmtInfo, StmtInfo>> CFGRelationsManager::getAllAffectsBipRelation() {
	return affects_bip_processor.evaluateSynonymAndSynonym();
}

std::vector<StmtInfo> CFGRelationsManager::getAffectedBip() {
	return affects_bip_processor.evaluateWildAndSynonym();
}

std::vector<StmtInfo> CFGRelationsManager::getAffectingBip() {
	return affects_bip_processor.evaluateSynonymAndWild();
}

std::vector<StmtInfo> CFGRelationsManager::getAffectedBip(stmt_index index) {
	auto& stmts = PKB::getInstance().getStmts();
	if (stmts[index - 1].stmt_type != STMT_ASSIGN) {
		return std::vector<StmtInfo>{};
	}
	return affects_bip_processor.evaluateConstantAndSynonym(index);
}

std::vector<StmtInfo> CFGRelationsManager::getAffectingBip(stmt_index index) {
	auto& stmts = PKB::getInstance().getStmts();
	if (stmts[index - 1].stmt_type != STMT_ASSIGN) {
		return std::vector<StmtInfo>{};
	}
	return affects_bip_processor.evaluateSynonymAndConstant(index);
}

bool CFGRelationsManager::isAffectsBipTEmpty() {
	return isAffectsBipEmpty();
}

bool CFGRelationsManager::isAffectsBipT(stmt_index index1, stmt_index index2) {
	auto& stmts = PKB::getInstance().getStmts();
	if (stmts[index1 - 1].stmt_type != STMT_ASSIGN || stmts[index2 - 1].stmt_type != STMT_ASSIGN) {
		return false;
	}
	affects_bipT_processor.fullyPopulate();
	return affects_bipT_processor.evaluateConstantAndConstant(index1, index2);
}

bool CFGRelationsManager::isAffectingBipT(stmt_index index) {
	return isAffectingBip(index);
}

bool CFGRelationsManager::isAffectedBipT(stmt_index index) {
	return isAffectedBip(index);
}

std::vector<std::pair<StmtInfo, StmtInfo>> CFGRelationsManager::getAllAffectsBipTRelation() {
	affects_bip_processor.fullyPopulate();
	return affects_bipT_processor.evaluateSynonymAndSynonym();
}

std::vector<StmtInfo> CFGRelationsManager::getAffectedBipT() {
	affects_bip_processor.fullyPopulate();
	return affects_bipT_processor.evaluateWildAndSynonym();
}

std::vector<StmtInfo> CFGRelationsManager::getAffectingBipT() {
	affects_bip_processor.fullyPopulate();
	return affects_bipT_processor.evaluateSynonymAndWild();
}

std::vector<StmtInfo> CFGRelationsManager::getAffectedBipT(stmt_index index) {
	auto& stmts = PKB::getInstance().getStmts();
	if (stmts[index - 1].stmt_type != STMT_ASSIGN) {
		return std::vector<StmtInfo>{};
	}
	affects_bip_processor.fullyPopulate();
	return affects_bipT_processor.evaluateConstantAndSynonym(index);
}

std::vector<StmtInfo> CFGRelationsManager::getAffectingBipT(stmt_index index) {
	auto& stmts = PKB::getInstance().getStmts();
	if (stmts[index - 1].stmt_type != STMT_ASSIGN) {
		return std::vector<StmtInfo>{};
	}
	affects_bip_processor.fullyPopulate();
	return affects_bipT_processor.evaluateSynonymAndConstant(index);
}

bool CFGRelationsManager::inSameProc(stmt_index index1, stmt_index index2) {
	std::vector<proc_name> v = PKB::getInstance().getProcContains().getKeys(index1);
	return PKB::getInstance().getProcContains().containsPair(v[0], index2);
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

AffectsBipPreprocessor CFGRelationsManager::getAffectsBipProcessor() {
	return affects_bip_processor;
}

AffectsBipTPreprocessor CFGRelationsManager::getAffectsBipTProcessor() {
	return affects_bipT_processor;
}

std::vector<LabelledProgLine> CFGRelationsManager::getFirstProgs() {
	std::vector<LabelledProgLine> res;
	auto cfgs = PKB::getInstance().getCFGBips();
	for (CFG* cfg : cfgs) {
		res.push_back(cfg->getHeadLabelledProgLine());
	}
	return res;
}

const MonotypeRelationTable<LabelledProgLine>& CFGRelationsManager::getLabelledNextTable() {
	auto cfgs = PKB::getInstance().getCFGBips();
	for (CFG* cfg : cfgs) {
		for (auto pair : cfg->getNextBipWithLabel()) {
			labelled_next_table.insert(pair.first, pair.second);
		}
	}
	return labelled_next_table;
}