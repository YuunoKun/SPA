#pragma once

#include "Query.h"
#include "QueryResult.h"
#include "PKBAdapter.h"
#include "WithEvaluator.h"

#include "Query.h"
#include "QueryResult.h"
#include "PKBAdapter.h"
#include "Utility.h"
#include "WithEvaluator.h"

bool WithEvaluator::evaluateWildAndWild() {
	throw std::invalid_argument("evaluateWildAndWild(): Wild is not allowed for WITH");
}

bool WithEvaluator::evaluateConstantAndConstant(Entity e1, Entity e2) {
	return e1.getValue() == e2.getValue();
}

bool WithEvaluator::evaluateConstantAndWild(Entity e) {
	throw std::invalid_argument("evaluateConstantAndWild(): Wild is not allowed for WITH");
}

bool WithEvaluator::evaluateWildAndConstant(Entity e) {
	throw std::invalid_argument("evaluateWildAndConstant(): Wild is not allowed for WITH");
}

ResultTable WithEvaluator::evaluateSynonymAndSynonym(Entity left, Entity right) {
	Entity dummy = { STMT, Synonym() };
	std::vector<Entity> header1 = { left, dummy };
	std::vector<Entity> header2 = { right, dummy };
	ResultTable rt1(header1, getEntity(left));
	ResultTable rt2(header2, getEntity(right));

	rt1.merge(rt2);
	std::vector<Entity> header = { left, right };
	return rt1.getResultTable(header);
}

ResultTable WithEvaluator::evaluateWildAndSynonym(Entity header) {
	throw std::invalid_argument("evaluateWildAndSynonym(): Wild is not allowed for WITH");
}

ResultTable WithEvaluator::evaluateSynonymAndWild(Entity header) {
	throw std::invalid_argument("evaluateSynonymAndWild(): Wild is not allowed for WITH");
}

ResultTable WithEvaluator::evaluateConstantAndSynonym(Entity constant, Entity header) {
	return ResultTable(header, getEntityWithAttribute(header.getType(), constant));
}

ResultTable WithEvaluator::evaluateSynonymAndConstant(Entity header, Entity constant) {
	return ResultTable(header, getEntityWithAttribute(header.getType(), constant));
}

std::list<std::vector<std::string>> WithEvaluator::getEntity(Entity e) {

	if (Utility::isSecondaryAttribute(e)) {
		return getEntityAndSecondaryAttribute(e);
	}
	std::list<std::string> result;
	switch (e.getType()) {
	case EntityType::PROG_LINE:
	case EntityType::STMT: result = Utility::stmtInfoToStringList(pkb.getStmts()); break;
	case EntityType::READ: result = Utility::stmtInfoToStringList(pkb.getReads());  break;
	case EntityType::PRINT: result = Utility::stmtInfoToStringList(pkb.getPrints()); break;
	case EntityType::CALL: result = Utility::stmtInfoToStringList(pkb.getCalls()); break;
	case EntityType::WHILE: result = Utility::stmtInfoToStringList(pkb.getWhiles()); break;
	case EntityType::IF: result = Utility::stmtInfoToStringList(pkb.getIfs()); break;
	case EntityType::ASSIGN: result = Utility::stmtInfoToStringList(pkb.getAssigns()); break;
	case EntityType::VARIABLE: result = Utility::variablesToStringList(pkb.getVariables());  break;
	case EntityType::CONSTANT: result = Utility::constantsToStringList(pkb.getConstants());  break;
	case EntityType::PROCEDURE: result = Utility::proceduresToStringList(pkb.getProcedures());  break;
	default: throw std::domain_error("getEntityWithAttribute(Entity, Entity): Entity type does not have Attribute!");
		break;
	}
	return Utility::pairToStringTable(Utility::stringListToStringPair(result));
}



std::list<std::vector<std::string>> WithEvaluator::getEntityAndSecondaryAttribute(Entity synonym) {
	switch (synonym.getType()) {
	case EntityType::READ:
		if (synonym.getAttribute() == AttrRef::VAR_NAME) {
			return Utility::pairToStringTable(pkb.getAllReadVars());
		}
		break;
	case EntityType::PRINT:
		if (synonym.getAttribute() == AttrRef::VAR_NAME) {
			return Utility::pairToStringTable(pkb.getAllPrintVars());
		}
		break;
	case EntityType::CALL:
		if (synonym.getAttribute() == AttrRef::PROC_NAME) {
			return Utility::pairToStringTable(pkb.getAllCallS());
		}
		break;
	}

	throw std::domain_error("getEntityAndSecondaryAttribute(Entity): Entity type does not have Secondary Attribute!");
}

std::list<std::string> WithEvaluator::getEntityWithAttribute(Entity e, Entity constant) {
	if (Utility::isSecondaryAttribute(e)) {
		return getEntityWithSecondaryAttribute(constant);
	}
	bool is_true = false;
	switch (e.getType()) {
	case EntityType::STMT:
	case EntityType::PROG_LINE: is_true = pkb.isStmt(std::stoi(constant.getValue())); break;
	case EntityType::READ: is_true = pkb.isRead(std::stoi(constant.getValue())); break;
	case EntityType::PRINT: is_true = pkb.isPrint(std::stoi(constant.getValue())); break;
	case EntityType::CALL: is_true = pkb.isCall(std::stoi(constant.getValue())); break;
	case EntityType::WHILE: is_true = pkb.isWhile(std::stoi(constant.getValue())); break;
	case EntityType::IF: is_true = pkb.isIf(std::stoi(constant.getValue())); break;
	case EntityType::ASSIGN: is_true = pkb.isAssign(std::stoi(constant.getValue())); break;
	case EntityType::VARIABLE: is_true = pkb.isVariable(constant.getValue()); break;
	case EntityType::CONSTANT: is_true = pkb.isConstant(std::stoi(constant.getValue())); break;
	case EntityType::PROCEDURE: is_true = pkb.isProcedure(constant.getValue()); break;
	default: throw std::domain_error("getEntityWithAttribute(Entity, Entity): Entity type does not have Attribute!");
		break;
	}
	if (is_true) {
		return { constant.getValue() };
	}
	else {
		return { };
	}
}

std::list<std::string> WithEvaluator::getEntityWithSecondaryAttribute(Entity constant) {
	switch (constant.getType()) {
	case EntityType::READ:
		if (constant.getAttribute() == AttrRef::VAR_NAME) {
		return Utility::stmtIndexToStringList(pkb.getPrint(constant.getValue()));
		}
		break;
	case EntityType::PRINT:
		if (constant.getAttribute() == AttrRef::VAR_NAME) {
			return Utility::stmtIndexToStringList(pkb.getRead(constant.getValue()));
		}
		break;
	case EntityType::CALL: 
		if (constant.getAttribute() == AttrRef::PROC_NAME) {
			return Utility::stmtIndexToStringList(pkb.getCalleeS(constant.getValue()));
		}
		break;
	}

	throw std::domain_error("getEntityWithSecondaryAttribute(Entity): Entity type does not have Secondary Attribute!");
}

