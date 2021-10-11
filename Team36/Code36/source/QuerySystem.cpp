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
		std::list<std::list<std::string>> results = evaluator.evaluateQuery(query);
		std::list<std::string> result = ResultProjector::projectResult(results);

		return result;
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