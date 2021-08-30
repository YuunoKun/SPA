#include "QueryEvaluator.h"

QueryEvaluator::QueryEvaluator() {
}

std::string QueryEvaluator::evaluateQuery(Query query) {

	initEntity(query);
	return getResult(query);;
}



void QueryEvaluator::initEntity(Query query) {
	std::unordered_map<std::string, Entity>::iterator it;

	for (it = query.getEntities().begin(); it != query.getEntities().end(); it++) {
		switch (it->second.getType()) {
		case STMT: initEntityStmts(it->second, pkb.getStmts()); break;
		case READ: break;
		case PRINT: break;
		case CALL: break;
		case WHILE: break;
		case IF: break;
		case ASSIGN: break;
		case VARIABLE: initEntityVariables(it->second, pkb.getVariables());  break;
		case CONSTANT: initEntityConstants(it->second, pkb.getConstants());  break;
		case PROCEDURE: initEntityProcedures(it->second, pkb.getProcedures());  break;
		}
	}

}

void QueryEvaluator::initEntityConstants(Entity entity, std::vector<constant> v) {
	for each (constant c in v) {
		entity.addValue(std::to_string(c));
	}

}

void QueryEvaluator::initEntityProcedures(Entity entity, std::vector<procedure_name> v) {
	for each (procedure_name name in v) {
		entity.addValue(name);
	}

}

void QueryEvaluator::initEntityStmts(Entity entity, std::vector<Stmt> v) {
	for each (Stmt stmt in v) {
		entity.addValue(std::to_string(stmt.getNum()));
	}
}



void QueryEvaluator::initEntityVariables(Entity entity, std::vector<variable_name> v) {
	for each (variable_name name in v) {
		entity.addValue(name);
	}
}


std::string QueryEvaluator::getResult(Query query) {
	std::vector<std::string> results(query.getSelected().getValues().begin(), query.getSelected().getValues().end()); ;

	std::string result;

	for (int i = 0; i < results.size(); i++) {
		result += results[i];
		if (i + 1 != results.size()) {
			result += ", ";
		}
	}

	return std::string();
}





