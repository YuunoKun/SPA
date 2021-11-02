#include "QueryEvaluator.h"

#include "Utility.h"
#include "ResultTable.h"
#include "RelationsEvaluator.h"
#include "PatternsEvaluator.h"
#include "CFGRelationsManager.h"
#include "Clause.h"

QueryEvaluator::QueryEvaluator() {
}

std::list<std::string> QueryEvaluator::evaluateQuery(Query& query) {
	QueryResult result;

	evaluateClauses(query, result);

	PKBAdapter::getRelationManager().reset();

	std::list<std::string> out;
	getResult(query, result, out);
	return out;
}

void QueryEvaluator::evaluateClauses(Query& query, QueryResult& query_result) {
	RelationsEvaluator relation_evaluator;
	PatternsEvaluator pattern_evaluator;
	for (auto& it : query.getClauses()) {
		if (!query_result.haveResult()) {
			break;
		}
		if (it.getType() == ClauseType::RELATION) {
			relation_evaluator.evaluateRelation(query_result, it.getRelation());
		} else {
			pattern_evaluator.evaluatePattern(query_result, it.getPattern());
		}
	}
}


void QueryEvaluator::getRawResult(Entity selected, std::list<std::string>& out) {
	if (Utility::isSecondaryAttribute(selected)) {
		getRawResultWithSecondaryAttribute(selected, out);
		return;
	}

	switch (selected.getType()) {
	case EntityType::PROG_LINE :
	case EntityType::STMT: Utility::stmtInfoToStringList(pkb.getStmts(), out); break;
	case EntityType::READ: Utility::stmtInfoToStringList(pkb.getReads(), out);  break;
	case EntityType::PRINT: Utility::stmtInfoToStringList(pkb.getPrints(), out); break;
	case EntityType::CALL: Utility::stmtInfoToStringList(pkb.getCalls(), out); break;
	case EntityType::WHILE: Utility::stmtInfoToStringList(pkb.getWhiles(), out); break;
	case EntityType::IF: Utility::stmtInfoToStringList(pkb.getIfs(), out); break;
	case EntityType::ASSIGN: Utility::stmtInfoToStringList(pkb.getAssigns(), out); break;
	case EntityType::VARIABLE: Utility::variableToStringList(pkb.getVariables(), out);  break;
	case EntityType::CONSTANT: Utility::constantToStringList(pkb.getConstants(), out);  break;
	case EntityType::PROCEDURE: Utility::procedureToStringList(pkb.getProcedures(), out);  break;
	}

}

void QueryEvaluator::getRawResultWithSecondaryAttribute(Entity selected, std::list<std::string>& out) {
	switch (selected.getType()) {
	case EntityType::READ:
		if (selected.getAttribute() == AttrRef::VAR_NAME) {
			Utility::variableToStringList(pkb.getPrintVar(), out);
			return;
		}
		break;
	case EntityType::PRINT:
		if (selected.getAttribute() == AttrRef::VAR_NAME) {
			Utility::variableToStringList(pkb.getReadVar(), out);
			return;
		}
		break;
	case EntityType::CALL:
		if (selected.getAttribute() == AttrRef::PROC_NAME) {
			Utility::procedureToStringList(pkb.getCalledS(), out);
			return;
		}
		break;
	}

	throw std::domain_error("Selected Entity type does not have Secondary Attribute!");
}

std::string QueryEvaluator::getEntitySecondaryAttribute(std::string primary, Entity& type) {
	switch (type.getType()) {
	case EntityType::READ:
		if (type.getAttribute() == AttrRef::VAR_NAME) {
			return pkb.getReadVar(std::stoi(primary)).front();
		}
		break;
	case EntityType::PRINT:
		if (type.getAttribute() == AttrRef::VAR_NAME) {
			return pkb.getPrintVar(std::stoi(primary)).front();
		}
		break;
	case EntityType::CALL:
		if (type.getAttribute() == AttrRef::PROC_NAME) {
			return pkb.getCalledS(std::stoi(primary)).front();
		}
		break;
	}

	throw std::domain_error("Selected Entity type does not have Secondary Attribute!");
}

void QueryEvaluator::convertToSecondaryAttribute(std::list<std::string>& table, Entity& type) {
	auto& it = table.begin();
	while (it != table.end()) {
		*it = getEntitySecondaryAttribute(*it, type);
		it++;
	}

}

void QueryEvaluator::mergeTable(ResultTable& result_table, std::vector<Entity>& selected, std::list<std::string>& out) {
	std::list<std::list<std::string>> table;
	result_table.getEntityResults(selected, table);
	for (auto row : table) {
		int index = 0;
		std::string row_result = row.front();
		if (Utility::isSecondaryAttribute(selected[index])) {
			row_result = getEntitySecondaryAttribute(row.front(), selected[index]);
		}
		row.pop_front();
		while (!row.empty()) {
			index++;
			if (Utility::isSecondaryAttribute(selected[index])) {
				row_result += " " +getEntitySecondaryAttribute(row.front(), selected[index]);
			}
			else {
				row_result += " " + row.front();
			}
			row.pop_front();
		}
		out.emplace_back(row_result);
	}
}


void QueryEvaluator::getTupleResult(Query& query, QueryResult& query_result, std::list<std::string>& out) {
	std::vector<Entity> selected_entities = Utility::getEntitiesWithoutDuplicate(query.getSelected());
	ResultTable result;
	if (query_result.isInTables(selected_entities)) {
		query_result.getResults(selected_entities, result);
		selected_entities = Utility::getEntitiesExclude(selected_entities, result.getHeaders());
	}

	for (auto& selected : selected_entities) {
		std::list<std::string> raw_result;
		getRawResult(selected, raw_result);
		result.joinTable(ResultTable(selected, raw_result));
	}
	
	mergeTable(result, query.getSelected(), out);
}


void QueryEvaluator::getResult(Query& query, QueryResult& result, std::list<std::string>& out) {
	if (query.getSelected()[0].getType() == EntityType::BOOLEAN) {
		if (result.haveResult()) {
			out.push_back(BOOLEAN_TRUE);
		}
		else {
			out.push_back(BOOLEAN_FALSE);
		}
		return;
	} 

	if (!result.haveResult()) {
		return;
	}

	if (query.getSelected().size() > 1) {
		getTupleResult(query, result, out);
		return;
	}

	if (!result.isInTables(query.getSelected()[0])) {
		getRawResult(query.getSelected()[0], out);

		if (Utility::isSecondaryAttribute(query.getSelected()[0])) {
			}
		return;
	}

	result.getResult(query.getSelected()[0], out);

	if (Utility::isSecondaryAttribute(query.getSelected()[0])) {
		convertToSecondaryAttribute(out, query.getSelected()[0]);
		return;
	}

}