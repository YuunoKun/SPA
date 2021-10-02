#include "QuerySystem.h"

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
	catch (std::exception& e)
	{
		std::cout << "Standard exception: " << e.what() << std::endl;
		return {};
	}
}