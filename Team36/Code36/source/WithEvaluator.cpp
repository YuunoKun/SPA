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

WithEvaluator::WithEvaluator() {
	string_dummy = { VARIABLE, Synonym() };
	num_dummy = { STMT, Synonym() };
}

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
	ResultTable rt1 = getEntity(left);
	ResultTable rt2 = getEntity(right);
	rt1.joinTable(rt2);
	if (rt1.isEmpty()) {
		return ResultTable();
	}

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
	return getEntityWithAttribute(header, constant);
}

ResultTable WithEvaluator::evaluateSynonymAndConstant(Entity header, Entity constant) {
	return getEntityWithAttribute(header, constant);
}

ResultTable WithEvaluator::getEntity(Entity& synonym) {
	if (Utility::isSecondaryAttribute(synonym)) {
		return getEntityAndSecondaryAttribute(synonym);
	}

	std::pair<Entity, Entity> string_header = { synonym,  string_dummy };
	std::pair<Entity, Entity> num_header = { synonym,  num_dummy };

	switch (synonym.getType()) {
	case EntityType::PROG_LINE:
	case EntityType::STMT:
	case EntityType::READ:
	case EntityType::PRINT:
	case EntityType::CALL:
	case EntityType::WHILE:
	case EntityType::IF:
	case EntityType::ASSIGN: return ResultTable(num_header, Utility::duplicateColumn(Utility::filterResult(synonym.getType(), pkb.getStmts()))); break;
	case EntityType::VARIABLE: return ResultTable(string_header, Utility::duplicateColumn(pkb.getVariables())); break;
	case EntityType::CONSTANT: return ResultTable(num_header, Utility::duplicateColumn(pkb.getConstants())); break;
	case EntityType::PROCEDURE: return ResultTable(string_header, Utility::duplicateColumn(pkb.getProcedures())); break;
	default: throw std::domain_error("getEntityWithAttribute(Entity, Entity): Entity type does not have Attribute!");
		break;
	}
}

ResultTable WithEvaluator::getEntityAndSecondaryAttribute(Entity& synonym) {
	std::pair<Entity, Entity> header = { synonym,  string_dummy };
	switch (synonym.getType()) {
	case EntityType::READ:
		if (synonym.getAttribute() == AttrRef::VAR_NAME) {
			return ResultTable(header, pkb.getAllReadVars());
		}
		break;
	case EntityType::PRINT:
		if (synonym.getAttribute() == AttrRef::VAR_NAME) {
			return ResultTable(header, pkb.getAllPrintVars());
		}
		break;
	case EntityType::CALL:
		if (synonym.getAttribute() == AttrRef::PROC_NAME) {
			return ResultTable(header, pkb.getAllCallS());
		}
		break;
	}

	throw std::domain_error("getEntityAndSecondaryAttribute(Entity): Entity type does not have Secondary Attribute!");
}

ResultTable WithEvaluator::getEntityWithAttribute(Entity& e, Entity& constant) {
	if (Utility::isSecondaryAttribute(e)) {
		return getEntityWithSecondaryAttribute(e, constant);
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
	if (is_true && Utility::isStringEntityType(e.getType())) {
		std::vector<std::string> list = { constant.getValue() };
		return ResultTable(e, list);
	} else if(is_true) {
		std::vector<value> list = { std::stoul(constant.getValue()) };
		return ResultTable(e, list);
	} else {
		return ResultTable(e, std::vector<value>());
	}
}

ResultTable WithEvaluator::getEntityWithSecondaryAttribute(Entity& e, Entity& constant) {
	switch (constant.getType()) {
	case EntityType::READ:
		if (constant.getAttribute() == AttrRef::VAR_NAME) {
			return ResultTable(e, pkb.getRead(constant.getValue()));
		}
		break;
	case EntityType::PRINT:
		if (constant.getAttribute() == AttrRef::VAR_NAME) {
			return ResultTable(e, pkb.getPrint(constant.getValue()));
		}
		break;
	case EntityType::CALL:
		if (constant.getAttribute() == AttrRef::PROC_NAME) {
			return ResultTable(e, pkb.getCalleeS(constant.getValue()));
		}
		break;
	}

	throw std::domain_error("getEntityWithSecondaryAttribute(Entity): Entity type does not have Secondary Attribute!");
}