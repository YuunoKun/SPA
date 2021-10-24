#include "QuerySystem.h"
#include <iostream>
#include <exception>
#include "SyntacticErrorException.h"
#include "SemanticErrorException.h"

std::list<std::string> QuerySystem::processQuery(std::string input) {
	QueryPreprocessor preprocessor;
	QueryOptimizer optimizer;
	QueryEvaluator evaluator;
	try {
		Query query = preprocessor.parse(input);

		if (!optimizer.checkAllConstantExist(query.getClauses())) {
			returnEmptyResult(query.getSelected().front());
		}

		query.setClauses(optimizer.optimizeClausesOrder(query.getClauses()));

		return evaluator.evaluateQuery(query);;
	}
	catch (SyntacticErrorException&)
	{
		preprocessor.resetQuery();
		return {};
	}
	catch (SemanticErrorException&)
	{
		// TODO: Return false here instead of empty query
		preprocessor.resetQuery();
		return {};
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
		preprocessor.resetQuery();
		return {};
	}
}


std::list<std::string> QuerySystem::returnEmptyResult(Entity& selected) {
	if (selected.getType() == BOOLEAN) {
		return { BOOLEAN_FALSE };
	}
	else {
		return { };
	}
}