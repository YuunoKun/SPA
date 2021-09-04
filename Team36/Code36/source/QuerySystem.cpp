#include "QuerySystem.h"

std::list<std::string> QuerySystem::processQuery(std::string input) {
	QueryPreprocessor preprocessor;
	QueryEvaluator evaluator;

	Query query = preprocessor.parse(input);
	return evaluator.evaluateQuery(query);
}