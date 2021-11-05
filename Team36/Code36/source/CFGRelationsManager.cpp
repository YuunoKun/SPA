#include "CFGRelationsManager.h"

void CFGRelationsManager::reset() {
	next_t_preprocessor = NextTPreprocessor();
	affects_preprocessor = AffectsPreprocessor();
	affectsT_preprocessor = AffectsTPreprocessor();
	next_bip_preprocessor = NextBipPreprocessor();
	next_bipT_preprocessor = NextBipTPreprocessor();
	affects_bip_preprocessor = AffectsBipPreprocessor();
	affects_bipT_preprocessor = AffectsBipTPreprocessor();

	is_nextT_preprocessor_init = false;
	is_affects_preprocessor_init = false;
	is_affectsT_preprocessor_init = false;
	is_next_bip_preprocessor_init = false;
	is_next_bipT_preprocessor_init = false;
	is_affects_bip_preprocessor_init = false;
	is_affects_bipT_preprocessor_init = false;

	next_bip_table.clear();
	labelled_next_table.clear();
}

bool CFGRelationsManager::isNextTEmpty() {
	initNextTPreprocessor();
	return !next_t_preprocessor.evaluateWildAndWild();
}

bool CFGRelationsManager::isNextT(prog_line index1, prog_line index2) {
	if (inSameProc(index1, index2) == false) {
		return false;
	} else {
		initNextTPreprocessor();
		return next_t_preprocessor.evaluateConstantAndConstant(index1, index2);
	}
}

bool CFGRelationsManager::isPreviousT(prog_line index) {
	initNextTPreprocessor();
	return next_t_preprocessor.evaluateConstantAndWild(index);
}

bool CFGRelationsManager::isNextT(prog_line index) {
	initNextTPreprocessor();
	return next_t_preprocessor.evaluateWildAndConstant(index);
}

std::vector<std::pair<StmtInfo, StmtInfo>> CFGRelationsManager::getAllNextTRelation() {
	initNextTPreprocessor();
	return next_t_preprocessor.evaluateSynonymAndSynonym();
}

std::vector<StmtInfo> CFGRelationsManager::getPreviousT() {
	initNextTPreprocessor();
	return next_t_preprocessor.evaluateSynonymAndWild();
}

std::vector<StmtInfo> CFGRelationsManager::getNextT() {
	initNextTPreprocessor();
	return next_t_preprocessor.evaluateWildAndSynonym();
}

std::vector<StmtInfo> CFGRelationsManager::getPreviousT(prog_line index) {
	initNextTPreprocessor();
	return next_t_preprocessor.evaluateSynonymAndConstant(index);
}

std::vector<StmtInfo> CFGRelationsManager::getNextT(prog_line index) {
	initNextTPreprocessor();
	return next_t_preprocessor.evaluateConstantAndSynonym(index);
}

bool CFGRelationsManager::isAffectsEmpty() {
	initAffectsPreprocessor();
	return !affects_preprocessor.evaluateWildAndWild();
}

bool CFGRelationsManager::isAffects(stmt_index index1, stmt_index index2) {
	bool is_nextT_calculated = is_nextT_preprocessor_init &&
		(next_t_preprocessor.isFullyPopulated() ||
			next_t_preprocessor.isCalculated(index1 - 1, index2 - 1));
	if (is_nextT_calculated && !isNextT(index1, index2)) {
		return false;
	}
	if (inSameProc(index1, index2) == false) {
		return false;
	} else {
		initAffectsPreprocessor();
		return affects_preprocessor.evaluateConstantAndConstant(index1, index2);
	}
}

bool CFGRelationsManager::isAffecting(stmt_index index) {
	initAffectsPreprocessor();
	return affects_preprocessor.evaluateConstantAndWild(index);
}

bool CFGRelationsManager::isAffected(stmt_index index) {
	initAffectsPreprocessor();
	return affects_preprocessor.evaluateWildAndConstant(index);
}

std::vector<std::pair<StmtInfo, StmtInfo>> CFGRelationsManager::getAllAffectsRelation() {
	initAffectsPreprocessor();
	return affects_preprocessor.evaluateSynonymAndSynonym();
}

std::vector<StmtInfo> CFGRelationsManager::getAffected() {
	initAffectsPreprocessor();
	return affects_preprocessor.evaluateWildAndSynonym();
}

std::vector<StmtInfo> CFGRelationsManager::getAffecting() {
	initAffectsPreprocessor();
	return affects_preprocessor.evaluateSynonymAndWild();
}

std::vector<StmtInfo> CFGRelationsManager::getAffected(stmt_index index) {
	auto& stmts = pkb_instance.getStmts();
	if (stmts[index - 1].stmt_type != STMT_ASSIGN) {
		return std::vector<StmtInfo>{};
	} else {
		initAffectsPreprocessor();
		return affects_preprocessor.evaluateConstantAndSynonym(index);
	}
}

std::vector<StmtInfo> CFGRelationsManager::getAffecting(stmt_index index) {
	auto& stmts = pkb_instance.getStmts();
	if (stmts[index - 1].stmt_type != STMT_ASSIGN) {
		return std::vector<StmtInfo>{};
	} else {
		initAffectsPreprocessor();
		return affects_preprocessor.evaluateSynonymAndConstant(index);
	}
}

bool CFGRelationsManager::isAffectsTEmpty() {
	return isAffectsEmpty();
}

bool CFGRelationsManager::isAffectsT(stmt_index index1, stmt_index index2) {
	bool is_nextT_calculated = is_nextT_preprocessor_init &&
		(next_t_preprocessor.isFullyPopulated() ||
			next_t_preprocessor.isCalculated(index1 - 1, index2 - 1));
	if (is_nextT_calculated && !isNextT(index1, index2)) {
		return false;
	} else if (inSameProc(index1, index2) == false) {
		return false;
	} else {
		initAffectsTPreprocessor();
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
	initAffectsTPreprocessor();
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
	auto& stmts = pkb_instance.getStmts();
	if (stmts[index - 1].stmt_type != STMT_ASSIGN) {
		return std::vector<StmtInfo>{};
	} else {
		initAffectsTPreprocessor();
		affects_preprocessor.fullyPopulate();
		return affectsT_preprocessor.evaluateConstantAndSynonym(index);
	}
}

std::vector<StmtInfo> CFGRelationsManager::getAffectingT(stmt_index index) {
	auto& stmts = pkb_instance.getStmts();
	if (stmts[index - 1].stmt_type != STMT_ASSIGN) {
		return std::vector<StmtInfo>{};
	} else {
		initAffectsTPreprocessor();
		affects_preprocessor.fullyPopulate();
		return affectsT_preprocessor.evaluateSynonymAndConstant(index);
	}
}

bool CFGRelationsManager::isNextBipEmpty() {
	initNextBipPreprocessor();
	return !next_bip_preprocessor.evaluateWildAndWild();
}

bool CFGRelationsManager::isNextBip(prog_line index1, prog_line index2) {
	initNextBipPreprocessor();
	return next_bip_preprocessor.evaluateConstantAndConstant(index1, index2);
}

bool CFGRelationsManager::isPreviousBip(prog_line index) {
	initNextBipPreprocessor();
	return next_bip_preprocessor.evaluateConstantAndWild(index);
}

bool CFGRelationsManager::isNextBip(prog_line index) {
	initNextBipPreprocessor();
	return next_bip_preprocessor.evaluateWildAndConstant(index);
}

std::vector<std::pair<StmtInfo, StmtInfo>> CFGRelationsManager::getAllNextBipRelation() {
	initNextBipPreprocessor();
	return next_bip_preprocessor.evaluateSynonymAndSynonym();
}

std::vector<StmtInfo> CFGRelationsManager::getPreviousBip() {
	initNextBipPreprocessor();
	return next_bip_preprocessor.evaluateSynonymAndWild();
}

std::vector<StmtInfo> CFGRelationsManager::getNextBip() {
	initNextBipPreprocessor();
	return next_bip_preprocessor.evaluateWildAndSynonym();
}

std::vector<StmtInfo> CFGRelationsManager::getPreviousBip(prog_line index) {
	initNextBipPreprocessor();
	return next_bip_preprocessor.evaluateSynonymAndConstant(index);
}

std::vector<StmtInfo> CFGRelationsManager::getNextBip(prog_line index) {
	initNextBipPreprocessor();
	return next_bip_preprocessor.evaluateConstantAndSynonym(index);
}

bool CFGRelationsManager::isNextBipTEmpty() {
	return isNextBipEmpty();
}

bool CFGRelationsManager::isNextBipT(prog_line index1, prog_line index2) {
	initNextBipTPreprocessor();
	return next_bipT_preprocessor.evaluateConstantAndConstant(index1, index2);
}

bool CFGRelationsManager::isPreviousBipT(prog_line index) {
	return isPreviousBip(index);
}

bool CFGRelationsManager::isNextBipT(prog_line index) {
	return isNextBip(index);
}

std::vector<std::pair<StmtInfo, StmtInfo>> CFGRelationsManager::getAllNextBipTRelation() {
	initNextBipTPreprocessor();
	return next_bipT_preprocessor.evaluateSynonymAndSynonym();
}

std::vector<StmtInfo> CFGRelationsManager::getPreviousBipT() {
	return getPreviousBip();
}

std::vector<StmtInfo> CFGRelationsManager::getNextBipT() {
	return getNextBip();
}

std::vector<StmtInfo> CFGRelationsManager::getPreviousBipT(prog_line index) {
	initNextBipTPreprocessor();
	return next_bipT_preprocessor.evaluateSynonymAndConstant(index);
}

std::vector<StmtInfo> CFGRelationsManager::getNextBipT(prog_line index) {
	initNextBipTPreprocessor();
	return next_bipT_preprocessor.evaluateConstantAndSynonym(index);
}

bool CFGRelationsManager::isAffectsBipEmpty() {
	initAffectsBipPreprocessor();
	return !affects_bip_preprocessor.evaluateWildAndWild();
}

bool CFGRelationsManager::isAffectsBip(stmt_index index1, stmt_index index2) {
	auto& stmts = pkb_instance.getStmts();
	if (stmts[index1 - 1].stmt_type != STMT_ASSIGN || stmts[index2 - 1].stmt_type != STMT_ASSIGN) {
		return false;
	}
	initAffectsBipPreprocessor();
	return affects_bip_preprocessor.evaluateConstantAndConstant(index1, index2);
}

bool CFGRelationsManager::isAffectingBip(stmt_index index) {
	auto& stmts = pkb_instance.getStmts();
	if (stmts[index - 1].stmt_type != STMT_ASSIGN) {
		return false;
	}
	initAffectsBipPreprocessor();
	return affects_bip_preprocessor.evaluateConstantAndWild(index);
}

bool CFGRelationsManager::isAffectedBip(stmt_index index) {
	auto& stmts = pkb_instance.getStmts();
	if (stmts[index - 1].stmt_type != STMT_ASSIGN) {
		return false;
	}
	initAffectsBipPreprocessor();
	return affects_bip_preprocessor.evaluateWildAndConstant(index);
}

std::vector<std::pair<StmtInfo, StmtInfo>> CFGRelationsManager::getAllAffectsBipRelation() {
	initAffectsBipPreprocessor();
	return affects_bip_preprocessor.evaluateSynonymAndSynonym();
}

std::vector<StmtInfo> CFGRelationsManager::getAffectedBip() {
	initAffectsBipPreprocessor();
	return affects_bip_preprocessor.evaluateWildAndSynonym();
}

std::vector<StmtInfo> CFGRelationsManager::getAffectingBip() {
	initAffectsBipPreprocessor();
	return affects_bip_preprocessor.evaluateSynonymAndWild();
}

std::vector<StmtInfo> CFGRelationsManager::getAffectedBip(stmt_index index) {
	auto& stmts = pkb_instance.getStmts();
	if (stmts[index - 1].stmt_type != STMT_ASSIGN) {
		return std::vector<StmtInfo>{};
	}
	initAffectsBipPreprocessor();
	return affects_bip_preprocessor.evaluateConstantAndSynonym(index);
}

std::vector<StmtInfo> CFGRelationsManager::getAffectingBip(stmt_index index) {
	auto& stmts = pkb_instance.getStmts();
	if (stmts[index - 1].stmt_type != STMT_ASSIGN) {
		return std::vector<StmtInfo>{};
	}
	initAffectsBipPreprocessor();
	return affects_bip_preprocessor.evaluateSynonymAndConstant(index);
}

bool CFGRelationsManager::isAffectsBipTEmpty() {
	return isAffectsBipEmpty();
}

bool CFGRelationsManager::isAffectsBipT(stmt_index index1, stmt_index index2) {
	auto& stmts = pkb_instance.getStmts();
	if (stmts[index1 - 1].stmt_type != STMT_ASSIGN || stmts[index2 - 1].stmt_type != STMT_ASSIGN) {
		return false;
	}
	initAffectsBipTPreprocessor();
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
	initAffectsBipTPreprocessor();
	affects_bip_preprocessor.fullyPopulate();
	return affects_bipT_preprocessor.evaluateSynonymAndSynonym();
}

std::vector<StmtInfo> CFGRelationsManager::getAffectedBipT() {
	return getAffectedBip();
}

std::vector<StmtInfo> CFGRelationsManager::getAffectingBipT() {
	return getAffectingBip();
}

std::vector<StmtInfo> CFGRelationsManager::getAffectedBipT(stmt_index index) {
	auto& stmts = pkb_instance.getStmts();
	if (stmts[index - 1].stmt_type != STMT_ASSIGN) {
		return std::vector<StmtInfo>{};
	}
	initAffectsBipTPreprocessor();
	affects_bip_preprocessor.fullyPopulate();
	return affects_bipT_preprocessor.evaluateConstantAndSynonym(index);
}

std::vector<StmtInfo> CFGRelationsManager::getAffectingBipT(stmt_index index) {
	auto& stmts = pkb_instance.getStmts();
	if (stmts[index - 1].stmt_type != STMT_ASSIGN) {
		return std::vector<StmtInfo>{};
	}
	initAffectsBipTPreprocessor();
	affects_bip_preprocessor.fullyPopulate();
	return affects_bipT_preprocessor.evaluateSynonymAndConstant(index);
}

bool CFGRelationsManager::inSameProc(stmt_index index1, stmt_index index2) {
	std::vector<proc_name> v = pkb_instance.getProcContains().getKeys(index1);
	return pkb_instance.getProcContains().containsPair(v[0], index2);
}

void CFGRelationsManager::initNextTPreprocessor() {
	if (!is_nextT_preprocessor_init) {
		next_t_preprocessor = NextTPreprocessor(pkb_instance.getNext(),
			pkb_instance.getStmts());
		is_nextT_preprocessor_init = true;
	}
}

void CFGRelationsManager::initAffectsPreprocessor() {
	if (!is_affects_preprocessor_init) {
		affects_preprocessor = AffectsPreprocessor(
			pkb_instance.getNext(),
			pkb_instance.getUsesS(),
			pkb_instance.getModifiesS(),
			pkb_instance.getProcContains(),
			pkb_instance.getStmts());
		is_affects_preprocessor_init = true;
	}
}

void CFGRelationsManager::initAffectsTPreprocessor() {
	if (!is_affectsT_preprocessor_init) {
		initAffectsPreprocessor();
		affectsT_preprocessor = AffectsTPreprocessor(
			affects_preprocessor.getCache(),
			pkb_instance.getStmts());
		is_affectsT_preprocessor_init = true;
	}
}

void CFGRelationsManager::initNextBipPreprocessor() {
	if (!is_next_bip_preprocessor_init) {
		next_bip_preprocessor = NextBipPreprocessor(
			getNextBipTable(),
			pkb_instance.getStmts()
		);
		is_next_bip_preprocessor_init = true;
	}
}

void CFGRelationsManager::initNextBipTPreprocessor() {
	if (!is_next_bipT_preprocessor_init) {
		initNextBipPreprocessor();
		next_bipT_preprocessor = NextBipTPreprocessor(
			next_bip_table,
			getLabelledNextBipTable(),
			pkb_instance.getStmts()
		);
		is_next_bipT_preprocessor_init = true;
	}
}

void CFGRelationsManager::initAffectsBipPreprocessor() {
	if (!is_affects_bip_preprocessor_init) {
		affects_bip_preprocessor = AffectsBipPreprocessor(
			getLabelledNextBipTable(),
			pkb_instance.getUsesS(),
			pkb_instance.getModifiesS(),
			pkb_instance.getProcContains(),
			getFirstProgs(),
			pkb_instance.getStmts());
		is_affects_bip_preprocessor_init = true;
	}
}

void CFGRelationsManager::initAffectsBipTPreprocessor() {
	if (!is_affects_bipT_preprocessor_init) {
		initAffectsBipPreprocessor();
		affects_bipT_preprocessor = AffectsBipTPreprocessor(
			affects_bip_preprocessor.getCache(),
			affects_bip_preprocessor.getLabelledProgLineCache(),
			pkb_instance.getStmts());
		is_affects_bipT_preprocessor_init = true;
	}
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
	auto cfgs = pkb_instance.getCFGBips();
	for (CFG* cfg : cfgs) {
		res.push_back(cfg->getHeadLabelledProgLine());
	}
	return res;
}

const MonotypeRelationTable<StmtInfo>& CFGRelationsManager::getNextBipTable() {
	auto cfgs = pkb_instance.getCFGBips();
	for (CFG* cfg : cfgs) {
		for (const auto& pair : cfg->getNextBip()) {
			next_bip_table.insert(pkb_instance.getStmt(pair.first), pkb_instance.getStmt(pair.second));
		}
	}
	return next_bip_table;
}

const MonotypeRelationTable<LabelledProgLine>& CFGRelationsManager::getLabelledNextBipTable() {
	auto cfgs = pkb_instance.getCFGBips();
	for (CFG* cfg : cfgs) {
		for (const auto& pair : cfg->getNextBipWithLabel()) {
			labelled_next_table.insert(pair.first, pair.second);
		}
	}
	return labelled_next_table;
}