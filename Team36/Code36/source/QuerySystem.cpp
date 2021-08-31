#include "QuerySystem.h"

std::list<std::string> QuerySystem::processQuery(std::string input) {
	QueryPreprocessor preprocessor;
	QueryEvaluator evaluator;

	Query query = preprocessor.process(input);
	return evaluator.evaluateQuery(query);
}