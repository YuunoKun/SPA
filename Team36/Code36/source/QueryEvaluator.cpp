#include "QueryEvaluator.h"

QueryEvaluator::QueryEvaluator() {
}

QueryEvaluator::QueryEvaluator(PKBAdapter pkb) {
	this->pkb = pkb;
}

std::list<std::string> QueryEvaluator::evaluateQuery(Query query) {
	initEntity(query);
	return getResult(query);;
}

void QueryEvaluator::initEntity(Query& query) {
	std::unordered_map<std::string, Entity*>::iterator it;
	for (auto it : query.getEntities()) {
		printf("init entity: %s\n", it.second->getName().c_str());
		printf("variable size: %d\n", pkb.getVariables().size());
		printf("constant size: %d\n", pkb.getConstants().size());
		printf("procedure size: %d\n", pkb.getProcedures().size());
		switch (it.second->getType()) {
		case STMT: initEntityStmts(it.second, pkb.getStmts()); break;
		case READ: break;
		case PRINT: break;
		case CALL: break;
		case WHILE: break;
		case IF: break;
		case ASSIGN: break;
		case VARIABLE: initEntityVariables(it.second, pkb.getVariables());  break;
		case CONSTANT: initEntityConstants(it.second, pkb.getConstants());  break;
		case PROCEDURE: initEntityProcedures(it.second, pkb.getProcedures());  break;
		}
	}
	for (auto it : query.getEntities()) {
		printf("after entities size: %d\n", it.second->getValues().size());
	}
}

void QueryEvaluator::initEntityConstants(Entity* entity, std::vector<constant> v) {
	for each (constant c in v) {
		entity->addValue(std::to_string(c));
		printf("added const: %d\n", c);
	}
}

void QueryEvaluator::initEntityProcedures(Entity* entity, std::vector<procedure_name> v) {
	for each (procedure_name name in v) {
		entity->addValue(name);
	}
}

void QueryEvaluator::initEntityStmts(Entity* entity, std::vector<Stmt> v) {
	for each (Stmt stmt in v) {
		entity->addValue(std::to_string(stmt.getNum()));

		printf("added num: %d\n", stmt.getNum());
	}

	printf("added stmts\n");
}

void QueryEvaluator::initEntityVariables(Entity* entity, std::vector<variable_name> v) {
	for each (variable_name name in v) {
		entity->addValue(name.c_str());
		printf("added var: %s\n", name.c_str());
	}

	printf("added variable count:%d\n", entity->getValues().size());
}

std::list<std::string> QueryEvaluator::getResult(Query& query) {
	std::unordered_set<std::string> values = query.getSelected().getValues();
	printf("result size %d\n", values.size());
	printf("entity size:%d\n", query.getEntities().at(query.getSelected().getName())->getValues().size());
	std::list<std::string> results(values.begin(), values.end());

	return results;
}