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
	//Todo Evaluator
	return ResultTable({ left, right }, pkb.getFollows());
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

std::list<std::string> WithEvaluator::getEntityWithAttribute(Entity e) {
	switch (e.getAttribute()) {
	case STMT_INDEX:
	case VALUE:  break;
	case PROC_NAME:break;
	case VAR_NAME:break;
	default: throw std::domain_error("Attribute is not allow for With Evaluator");
	}
}



std::list<std::string> WithEvaluator::getEntityWithAttribute(Entity e, Entity constant) {
	if (Utility::isSecondaryAttribute(e)) {
		return getEntityWithSecondaryAttribute(constant);
	}
	bool is_true = false;
	switch (e.getType()) {
	case EntityType::STMT:
	case EntityType::PROG_LINE: is_true = pkb.isStmt(std::stoi(constant.getValue()));
		break;
	case EntityType::READ: is_true = pkb.isRead(std::stoi(constant.getValue()));
		break;
	case EntityType::PRINT: is_true = pkb.isPrint(std::stoi(constant.getValue()));
		break;
	case EntityType::CALL: is_true = pkb.isCall(std::stoi(constant.getValue()));
		break;
	case EntityType::WHILE: is_true = pkb.isWhile(std::stoi(constant.getValue()));
		break;
	case EntityType::IF: is_true = pkb.isIf(std::stoi(constant.getValue()));
		break;
	case EntityType::ASSIGN: is_true = pkb.isAssign(std::stoi(constant.getValue()));
		break;
	case EntityType::VARIABLE: is_true = pkb.isVariable(constant.getValue());
		break;
	case EntityType::CONSTANT: is_true = pkb.isConstant(std::stoi(constant.getValue()));
		break;
	case EntityType::PROCEDURE: is_true = pkb.isProcedure(constant.getValue());
		break;
	default: throw std::domain_error("Entity type does not have Attribute!");
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

	throw std::domain_error("Entity type does not have Secondary Attribute!");
}

