#include "QuerySystem.h"

std::string QuerySystem::processQuery(std::string input) {
	QueryPreprocessor preprocessor;
	QueryEvaluator evaluator;

	Query query = preprocessor.process(input);
	std::string output = evaluator.evaluateQuery(query);
	return output;
}
