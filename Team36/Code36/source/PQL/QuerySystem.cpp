#include "QuerySystem.h"

string QuerySystem::processQuery(string input) {
	QueryPreprocessor preprocessor;
	QueryEvaluator evaluator;

	Query query = preprocessor.process(input);
	string output = evaluator.evaluateQuery(query);
	return output;
}
