#include "QueryEvaluator.h"
#include "ResultTable.h"
#include "RelationsEvaluator.h"
#include "PatternsEvaluator.h"
#include "PKB/RelationPreprocessorManager.h"
#include "PQL/QueryObject/Clause.h"
#include "PQL/Helper/Utility.h"

QueryEvaluator::QueryEvaluator() {
}

std::list<std::string> QueryEvaluator::evaluateQuery(Query& query) {
	QueryResult result;
	try {
		evaluateClauses(query, result);
		PKBAdapter::getRelationManager().reset();

		result.updateHaveResultAfterTableJoin();

		std::list<std::string> out;
		getResult(query, result, out);
		return out;
	}
	catch (std::exception& e) {
		std::cout << "QueryEvaluator evaluateQuery(): " << e.what() << std::endl;
		PKBAdapter::getRelationManager().reset();
		return {};
	}
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

ResultTable QueryEvaluator::getRawResult(Entity& selected) {
	switch (selected.getType()) {
	case EntityType::PROG_LINE:
	case EntityType::STMT:
	case EntityType::READ:
	case EntityType::PRINT:
	case EntityType::CALL:
	case EntityType::WHILE:
	case EntityType::IF:
	case EntityType::ASSIGN: return ResultTable(selected, pkb.getStmts()); break;
	case EntityType::VARIABLE: return ResultTable(selected, pkb.getVariables()); break;
	case EntityType::CONSTANT: return ResultTable(selected, pkb.getConstants()); break;
	case EntityType::PROCEDURE: return ResultTable(selected, pkb.getProcedures()); break;
	}

	throw std::domain_error("Selected Entity is not supported for select");
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
				row_result += SPACE + getEntitySecondaryAttribute(row.front(), selected[index]);
			} else {
				row_result += SPACE + row.front();
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
		result.joinTable(getRawResult(selected));
	}

	mergeTable(result, query.getSelected(), out);
}

void QueryEvaluator::getResult(Query& query, QueryResult& result, std::list<std::string>& out) {
	if (query.getSelected()[0].getType() == EntityType::BOOLEAN) {
		if (result.haveResult()) {
			out.push_back(BOOLEAN_TRUE);
		} else {
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

	Entity selected = query.getSelected()[0];
	if (result.isInTables(selected)) {
		result.getResult(query.getSelected()[0], out);
	} else {
		getRawResult(selected).getEntityResult(selected, out);
	}

	if (Utility::isSecondaryAttribute(query.getSelected()[0])) {
		convertToSecondaryAttribute(out, query.getSelected()[0]);
	}
}