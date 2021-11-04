#include "CFGRelationsManager.h"

CFGRelationsManager::CFGRelationsManager() {
	next_t_preprocessor = NextTPreprocessor(PKB::getInstance().getNext(),
		PKB::getInstance().getStmts());
	affects_preprocessor = AffectsPreprocessor(
		PKB::getInstance().getNext(),
		PKB::getInstance().getUsesS(),
		PKB::getInstance().getModifiesS(),
		PKB::getInstance().getProcContains(),
		PKB::getInstance().getStmts());
	affectsT_preprocessor = AffectsTPreprocessor(
		affects_preprocessor.getCache(),
		PKB::getInstance().getStmts());
	next_bip_preprocessor = NextBipPreprocessor(
		getNextBipTable(),
		PKB::getInstance().getStmts()
	);
	next_bipT_preprocessor = NextBipTPreprocessor(
		next_bip_preprocessor.getCache(),
		PKB::getInstance().getStmts()
	);
	affects_bip_preprocessor = AffectsBipPreprocessor(
		getLabelledNextTable(),
		PKB::getInstance().getUsesS(),
		PKB::getInstance().getModifiesS(),
		PKB::getInstance().getProcContains(),
		getFirstProgs(),
		PKB::getInstance().getStmts());
	affects_bipT_preprocessor = AffectsBipTPreprocessor(
		affects_bip_preprocessor.getCache(),
		affects_bip_preprocessor.getLabelledProgLineCache(),
		PKB::getInstance().getStmts());
}

void CFGRelationsManager::update() {
	next_t_preprocessor = NextTPreprocessor(PKB::getInstance().getNext(),
		PKB::getInstance().getStmts());
	affects_preprocessor = AffectsPreprocessor(
		PKB::getInstance().getNext(),
		PKB::getInstance().getUsesS(),
		PKB::getInstance().getModifiesS(),
		PKB::getInstance().getProcContains(),
		PKB::getInstance().getStmts());
	affectsT_preprocessor = AffectsTPreprocessor(
		affects_preprocessor.getCache(),
		PKB::getInstance().getStmts());
	next_bip_preprocessor = NextBipPreprocessor(
		getNextBipTable(),
		PKB::getInstance().getStmts()
	);
	next_bipT_preprocessor = NextBipTPreprocessor(
		next_bip_preprocessor.getCache(),
		PKB::getInstance().getStmts()
	);
	affects_bip_preprocessor = AffectsBipPreprocessor(
		getLabelledNextTable(),
		PKB::getInstance().getUsesS(),
		PKB::getInstance().getModifiesS(),
		PKB::getInstance().getProcContains(),
		getFirstProgs(),
		PKB::getInstance().getStmts());
	affects_bipT_preprocessor = AffectsBipTPreprocessor(
		affects_bip_preprocessor.getCache(),
		affects_bip_preprocessor.getLabelledProgLineCache(),
		PKB::getInstance().getStmts());
}

void CFGRelationsManager::reset() {
	next_t_preprocessor.reset();
	affects_preprocessor.reset();
	affectsT_preprocessor.reset();
	next_bip_preprocessor.reset();
	next_bipT_preprocessor.reset();
	affects_bip_preprocessor.reset();
	affects_bipT_preprocessor.reset();
}

bool CFGRelationsManager::isNextTEmpty() {
	return !next_t_preprocessor.evaluateWildAndWild();
}

bool CFGRelationsManager::isNextT(prog_line index1, prog_line index2) {
	if (inSameProc(index1, index2) == false) {
		return false;
	} else {
		return next_t_preprocessor.evaluateConstantAndConstant(index1, index2);
	}
}

bool CFGRelationsManager::isPreviousT(prog_line index) {
	return next_t_preprocessor.evaluateConstantAndWild(index);
}

bool CFGRelationsManager::isNextT(prog_line index) {
	return next_t_preprocessor.evaluateWildAndConstant(index);
}

std::vector<std::pair<StmtInfo, StmtInfo>> CFGRelationsManager::getAllNextTRelation() {
	return next_t_preprocessor.evaluateSynonymAndSynonym();
}

std::vector<StmtInfo> CFGRelationsManager::getPreviousT() {
	return next_t_preprocessor.evaluateSynonymAndWild();
}

std::vector<StmtInfo> CFGRelationsManager::getNextT() {
	return next_t_preprocessor.evaluateWildAndSynonym();
}

std::vector<StmtInfo> CFGRelationsManager::getPreviousT(prog_line index) {
	return next_t_preprocessor.evaluateSynonymAndConstant(index);
}

std::vector<StmtInfo> CFGRelationsManager::getNextT(prog_line index) {
	return next_t_preprocessor.evaluateConstantAndSynonym(index);
}

bool CFGRelationsManager::isAffectsEmpty() {
	return !affects_preprocessor.evaluateWildAndWild();
}

bool CFGRelationsManager::isAffects(stmt_index index1, stmt_index index2) {
	bool is_nextT_calculated = next_t_preprocessor.isFullyPopulated() || next_t_preprocessor.isCalculated(index1 - 1, index2 - 1);
	if (is_nextT_calculated && !isNextT(index1, index2)) {
		return false;
	}
	if (inSameProc(index1, index2) == false) {
		return false;
	} else {
		return affects_preprocessor.evaluateConstantAndConstant(index1, index2);
	}
}

bool CFGRelationsManager::isAffecting(stmt_index index) {
	return affects_preprocessor.evaluateConstantAndWild(index);
}

bool CFGRelationsManager::isAffected(stmt_index index) {
	return affects_preprocessor.evaluateWildAndConstant(index);
}

std::vector<std::pair<StmtInfo, StmtInfo>> CFGRelationsManager::getAllAffectsRelation() {
	return affects_preprocessor.evaluateSynonymAndSynonym();
}

std::vector<StmtInfo> CFGRelationsManager::getAffected() {
	return affects_preprocessor.evaluateWildAndSynonym();
}

std::vector<StmtInfo> CFGRelationsManager::getAffecting() {
	return affects_preprocessor.evaluateSynonymAndWild();
}

std::vector<StmtInfo> CFGRelationsManager::getAffected(stmt_index index) {
	auto& stmts = PKB::getInstance().getStmts();
	if (stmts[index - 1].stmt_type != STMT_ASSIGN) {
		return std::vector<StmtInfo>{};
	} else {
		return affects_preprocessor.evaluateConstantAndSynonym(index);
	}
}

std::vector<StmtInfo> CFGRelationsManager::getAffecting(stmt_index index) {
	auto& stmts = PKB::getInstance().getStmts();
	if (stmts[index - 1].stmt_type != STMT_ASSIGN) {
		return std::vector<StmtInfo>{};
	} else {
		return affects_preprocessor.evaluateSynonymAndConstant(index);
	}
}

bool CFGRelationsManager::isAffectsTEmpty() {
	return isAffectsEmpty();
}

bool CFGRelationsManager::isAffectsT(stmt_index index1, stmt_index index2) {
	bool is_nextT_calculated = next_t_preprocessor.isFullyPopulated() || next_t_preprocessor.isCalculated(index1 - 1, index2 - 1);
	if (is_nextT_calculated && !isNextT(index1, index2)) {
		return false;
	} else if (inSameProc(index1, index2) == false) {
		return false;
	} else {
		affects_preprocessor.fullyPopulate();
		return affectsT_preprocessor.evaluateConstantAndConstant(index1, index2);
	}
}

bool CFGRelationsManager::isAffectingT(stmt_index index) {
	return isAffecting(index);
}

bool CFGRelationsManager::isAffectedT(stmt_index index) {
	return isAffected(index);
}

std::vector<std::pair<StmtInfo, StmtInfo>> CFGRelationsManager::getAllAffectsTRelation() {
	affects_preprocessor.fullyPopulate();
	return affectsT_preprocessor.evaluateSynonymAndSynonym();
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
	} else {
		affects_preprocessor.fullyPopulate();
		return affectsT_preprocessor.evaluateConstantAndSynonym(index);
	}
}

std::vector<StmtInfo> CFGRelationsManager::getAffectingT(stmt_index index) {
	auto& stmts = PKB::getInstance().getStmts();
	if (stmts[index - 1].stmt_type != STMT_ASSIGN) {
		return std::vector<StmtInfo>{};
	} else {
		affects_preprocessor.fullyPopulate();
		return affectsT_preprocessor.evaluateSynonymAndConstant(index);
	}
}

//Temp implementation
bool CFGRelationsManager::isNextBipEmpty() {
	return !next_bip_preprocessor.evaluateWildAndWild();
}

bool CFGRelationsManager::isNextBip(prog_line index1, prog_line index2) {
	return next_bip_preprocessor.evaluateConstantAndConstant(index1, index2);
}

bool CFGRelationsManager::isPreviousBip(prog_line index) {
	return next_bip_preprocessor.evaluateConstantAndWild(index);
}

bool CFGRelationsManager::isNextBip(prog_line index) {
	return next_bip_preprocessor.evaluateWildAndConstant(index);
}

std::vector<std::pair<StmtInfo, StmtInfo>> CFGRelationsManager::getAllNextBipRelation() {
	return next_bip_preprocessor.getCache().getPairs();
}

std::vector<StmtInfo> CFGRelationsManager::getPreviousBip() {
	return next_bip_preprocessor.evaluateSynonymAndWild();
}

std::vector<StmtInfo> CFGRelationsManager::getNextBip() {
	return next_bip_preprocessor.evaluateWildAndSynonym();
}

std::vector<StmtInfo> CFGRelationsManager::getPreviousBip(prog_line index) {
	return next_bip_preprocessor.evaluateConstantAndSynonym(index);
}

std::vector<StmtInfo> CFGRelationsManager::getNextBip(prog_line index) {
	return next_bip_preprocessor.evaluateSynonymAndConstant(index);
}

bool CFGRelationsManager::isNextBipTEmpty() {
	return isNextBipEmpty();
}

bool CFGRelationsManager::isNextBipT(prog_line index1, prog_line index2) {
	return next_bipT_preprocessor.evaluateConstantAndConstant(index1, index2);
}

bool CFGRelationsManager::isPreviousBipT(prog_line index) {
	return next_bipT_preprocessor.evaluateConstantAndWild(index);
}

bool CFGRelationsManager::isNextBipT(prog_line index) {
	return next_bipT_preprocessor.evaluateWildAndConstant(index);
}

std::vector<std::pair<StmtInfo, StmtInfo>> CFGRelationsManager::getAllNextBipTRelation() {
	return next_bipT_preprocessor.evaluateSynonymAndSynonym();
}

std::vector<StmtInfo> CFGRelationsManager::getPreviousBipT() {
	return next_bipT_preprocessor.evaluateSynonymAndWild();
}

std::vector<StmtInfo> CFGRelationsManager::getNextBipT() {
	return next_bipT_preprocessor.evaluateWildAndSynonym();
}

std::vector<StmtInfo> CFGRelationsManager::getPreviousBipT(prog_line index) {
	return next_bipT_preprocessor.evaluateConstantAndSynonym(index);
}

std::vector<StmtInfo> CFGRelationsManager::getNextBipT(prog_line index) {
	return next_bipT_preprocessor.evaluateSynonymAndConstant(index);
}

bool CFGRelationsManager::isAffectsBipEmpty() {
	return !affects_bip_preprocessor.evaluateWildAndWild();
}

bool CFGRelationsManager::isAffectsBip(stmt_index index1, stmt_index index2) {
	auto& stmts = PKB::getInstance().getStmts();
	if (stmts[index1 - 1].stmt_type != STMT_ASSIGN || stmts[index2 - 1].stmt_type != STMT_ASSIGN) {
		return false;
	}
	return affects_bip_preprocessor.evaluateConstantAndConstant(index1, index2);
}

bool CFGRelationsManager::isAffectingBip(stmt_index index) {
	auto& stmts = PKB::getInstance().getStmts();
	if (stmts[index - 1].stmt_type != STMT_ASSIGN) {
		return false;
	}
	return affects_bip_preprocessor.evaluateConstantAndWild(index);
}

bool CFGRelationsManager::isAffectedBip(stmt_index index) {
	auto& stmts = PKB::getInstance().getStmts();
	if (stmts[index - 1].stmt_type != STMT_ASSIGN) {
		return false;
	}
	return affects_bip_preprocessor.evaluateWildAndConstant(index);
}

std::vector<std::pair<StmtInfo, StmtInfo>> CFGRelationsManager::getAllAffectsBipRelation() {
	return affects_bip_preprocessor.evaluateSynonymAndSynonym();
}

std::vector<StmtInfo> CFGRelationsManager::getAffectedBip() {
	return affects_bip_preprocessor.evaluateWildAndSynonym();
}

std::vector<StmtInfo> CFGRelationsManager::getAffectingBip() {
	return affects_bip_preprocessor.evaluateSynonymAndWild();
}

std::vector<StmtInfo> CFGRelationsManager::getAffectedBip(stmt_index index) {
	auto& stmts = PKB::getInstance().getStmts();
	if (stmts[index - 1].stmt_type != STMT_ASSIGN) {
		return std::vector<StmtInfo>{};
	}
	return affects_bip_preprocessor.evaluateConstantAndSynonym(index);
}

std::vector<StmtInfo> CFGRelationsManager::getAffectingBip(stmt_index index) {
	auto& stmts = PKB::getInstance().getStmts();
	if (stmts[index - 1].stmt_type != STMT_ASSIGN) {
		return std::vector<StmtInfo>{};
	}
	return affects_bip_preprocessor.evaluateSynonymAndConstant(index);
}

bool CFGRelationsManager::isAffectsBipTEmpty() {
	return isAffectsBipEmpty();
}

bool CFGRelationsManager::isAffectsBipT(stmt_index index1, stmt_index index2) {
	auto& stmts = PKB::getInstance().getStmts();
	if (stmts[index1 - 1].stmt_type != STMT_ASSIGN || stmts[index2 - 1].stmt_type != STMT_ASSIGN) {
		return false;
	}
	affects_bip_preprocessor.fullyPopulate();
	return affects_bipT_preprocessor.evaluateConstantAndConstant(index1, index2);
}

bool CFGRelationsManager::isAffectingBipT(stmt_index index) {
	return isAffectingBip(index);
}

bool CFGRelationsManager::isAffectedBipT(stmt_index index) {
	return isAffectedBip(index);
}

std::vector<std::pair<StmtInfo, StmtInfo>> CFGRelationsManager::getAllAffectsBipTRelation() {
	affects_bip_preprocessor.fullyPopulate();
	return affects_bipT_preprocessor.evaluateSynonymAndSynonym();
}

std::vector<StmtInfo> CFGRelationsManager::getAffectedBipT() {
	affects_bip_preprocessor.fullyPopulate();
	return affects_bipT_preprocessor.evaluateWildAndSynonym();
}

std::vector<StmtInfo> CFGRelationsManager::getAffectingBipT() {
	affects_bip_preprocessor.fullyPopulate();
	return affects_bipT_preprocessor.evaluateSynonymAndWild();
}

std::vector<StmtInfo> CFGRelationsManager::getAffectedBipT(stmt_index index) {
	auto& stmts = PKB::getInstance().getStmts();
	if (stmts[index - 1].stmt_type != STMT_ASSIGN) {
		return std::vector<StmtInfo>{};
	}
	affects_bip_preprocessor.fullyPopulate();
	return affects_bipT_preprocessor.evaluateConstantAndSynonym(index);
}

std::vector<StmtInfo> CFGRelationsManager::getAffectingBipT(stmt_index index) {
	auto& stmts = PKB::getInstance().getStmts();
	if (stmts[index - 1].stmt_type != STMT_ASSIGN) {
		return std::vector<StmtInfo>{};
	}
	affects_bip_preprocessor.fullyPopulate();
	return affects_bipT_preprocessor.evaluateSynonymAndConstant(index);
}

bool CFGRelationsManager::inSameProc(stmt_index index1, stmt_index index2) {
	std::vector<proc_name> v = PKB::getInstance().getProcContains().getKeys(index1);
	return PKB::getInstance().getProcContains().containsPair(v[0], index2);
}

NextTPreprocessor CFGRelationsManager::getNextTPreprocessor() {
	return next_t_preprocessor;
}

AffectsPreprocessor CFGRelationsManager::getAffectsPreprocessor() {
	return affects_preprocessor;
}

AffectsTPreprocessor CFGRelationsManager::getAffectsTPreprocessor() {
	return affectsT_preprocessor;
}

NextBipPreprocessor CFGRelationsManager::getNextBipPreprocessor() {
	return next_bip_preprocessor;
}

NextBipTPreprocessor CFGRelationsManager::getNextBipTPreprocessor() {
	return next_bipT_preprocessor;
}

AffectsBipPreprocessor CFGRelationsManager::getAffectsBipPreprocessor() {
	return affects_bip_preprocessor;
}

AffectsBipTPreprocessor CFGRelationsManager::getAffectsBipTPreprocessor() {
	return affects_bipT_preprocessor;
}

std::vector<LabelledProgLine> CFGRelationsManager::getFirstProgs() {
	std::vector<LabelledProgLine> res;
	auto cfgs = PKB::getInstance().getCFGBips();
	for (CFG* cfg : cfgs) {
		res.push_back(cfg->getHeadLabelledProgLine());
	}
	return res;
}

const MonotypeRelationTable<StmtInfo>& CFGRelationsManager::getNextBipTable() {
	auto cfgs = PKB::getInstance().getCFGBips();
	for (CFG* cfg : cfgs) {
		for (const auto& pair : cfg->getNextBip()) {
			next_bip_table.insert(PKB::getInstance().getStmt(pair.first), PKB::getInstance().getStmt(pair.second));
		}
	}
	return next_bip_table;
}

const MonotypeRelationTable<LabelledProgLine>& CFGRelationsManager::getLabelledNextTable() {
	auto cfgs = PKB::getInstance().getCFGBips();
	for (CFG* cfg : cfgs) {
		for (const auto& pair : cfg->getNextBipWithLabel()) {
			labelled_next_table.insert(pair.first, pair.second);
		}
	}
	return labelled_next_table;
}