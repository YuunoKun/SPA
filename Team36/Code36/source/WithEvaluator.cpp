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
	std::list<std::vector<std::string>> result1, result2;
	getEntity(left, result1);
	getEntity(right, result2);
	ResultTable rt1(header1, result1);
	ResultTable rt2(header2, result2);

	rt1.merge(rt2);
	std::vector<Entity> header = { left, right };
	if (left == right) {
		header = { left };
	}
	return rt1.getResultTable(header);
}

ResultTable WithEvaluator::evaluateWildAndSynonym(Entity header) {
	throw std::invalid_argument("evaluateWildAndSynonym(): Wild is not allowed for WITH");
}

ResultTable WithEvaluator::evaluateSynonymAndWild(Entity header) {
	throw std::invalid_argument("evaluateSynonymAndWild(): Wild is not allowed for WITH");
}

ResultTable WithEvaluator::evaluateConstantAndSynonym(Entity constant, Entity header) {
	return evaluateSynonymAndConstant(header, constant);
}

ResultTable WithEvaluator::evaluateSynonymAndConstant(Entity header, Entity constant) {
	std::list<std::vector<std::string>> result;
	getEntityWithAttribute(header, constant, result);
	return ResultTable(header, result);
}

void WithEvaluator::getEntity(Entity& e, std::list<std::vector<std::string>>& out) {
	if (Utility::isSecondaryAttribute(e)) {
		getEntityAndSecondaryAttribute(e, out);
		return;
	}
	std::list<std::string> result;
	switch (e.getType()) {
	case EntityType::PROG_LINE:
	case EntityType::STMT: Utility::stmtInfoToStringList(pkb.getStmts(), result); break;
	case EntityType::READ: Utility::stmtInfoToStringList(pkb.getReads(), result);  break;
	case EntityType::PRINT: Utility::stmtInfoToStringList(pkb.getPrints(), result); break;
	case EntityType::CALL: Utility::stmtInfoToStringList(pkb.getCalls(), result); break;
	case EntityType::WHILE: Utility::stmtInfoToStringList(pkb.getWhiles(), result); break;
	case EntityType::IF: Utility::stmtInfoToStringList(pkb.getIfs(), result); break;
	case EntityType::ASSIGN: Utility::stmtInfoToStringList(pkb.getAssigns(), result); break;
	case EntityType::VARIABLE: Utility::variableToStringList(pkb.getVariables(), result);  break;
	case EntityType::CONSTANT: Utility::constantToStringList(pkb.getConstants(), result);  break;
	case EntityType::PROCEDURE: Utility::procedureToStringList(pkb.getProcedures(), result);  break;
	default: throw std::domain_error("getEntityWithAttribute(Entity, Entity): Entity type does not have Attribute!");
		break;
	}
	Utility::stringListToStringTablePair(result, out);
}


void WithEvaluator::getEntityAndSecondaryAttribute(Entity& synonym, std::list<std::vector<std::string>>& out) {
	switch (synonym.getType()) {
	case EntityType::READ:
		if (synonym.getAttribute() == AttrRef::VAR_NAME) {
			Utility::pairToStringTable(pkb.getAllReadVars(), out);
			return;
		}
		break;
	case EntityType::PRINT:
		if (synonym.getAttribute() == AttrRef::VAR_NAME) {
			Utility::pairToStringTable(pkb.getAllPrintVars(), out);
			return;
		}
		break;
	case EntityType::CALL:
		if (synonym.getAttribute() == AttrRef::PROC_NAME) {
			Utility::pairToStringTable(pkb.getAllCallS(), out);
			return;
		}
		break;
	}

	throw std::domain_error("getEntityAndSecondaryAttribute(Entity): Entity type does not have Secondary Attribute!");
}

void WithEvaluator::getEntityWithAttribute(Entity& e, Entity& constant, std::list<std::vector<std::string>>& out) {
	if (Utility::isSecondaryAttribute(e)) {
		getEntityWithSecondaryAttribute(constant, out);
		return;
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
		out.push_back({ constant.getValue() });
	}
}

void WithEvaluator::getEntityWithSecondaryAttribute(Entity& constant, std::list<std::vector<std::string>>& out) {
	switch (constant.getType()) {
	case EntityType::READ:
		if (constant.getAttribute() == AttrRef::VAR_NAME) {
			Utility::stmtIndexToStringTable(pkb.getRead(constant.getValue()), out);
			return;
		}
		break;
	case EntityType::PRINT:
		if (constant.getAttribute() == AttrRef::VAR_NAME) {
			Utility::stmtIndexToStringTable(pkb.getPrint(constant.getValue()), out);
			return;
		}
		break;
	case EntityType::CALL: 
		if (constant.getAttribute() == AttrRef::PROC_NAME) {
			Utility::stmtIndexToStringTable(pkb.getCalleeS(constant.getValue()), out);
			return;
		}
		break;
	}

	throw std::domain_error("getEntityWithSecondaryAttribute(Entity): Entity type does not have Secondary Attribute!");
}

