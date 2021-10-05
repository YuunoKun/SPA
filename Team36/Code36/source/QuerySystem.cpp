#include "QuerySystem.h"
#include <iostream>
#include <exception>
#include "SyntacticErrorException.h"
#include "SemanticErrorException.h"

std::list<std::string> QuerySystem::processQuery(std::string input) {
	QueryPreprocessor preprocessor;
	QueryEvaluator evaluator;
	try {
		Query query = preprocessor.parse(input);
		query.setClauses(QueryOptimizer::optimizeClauses(query.getClauses()));

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