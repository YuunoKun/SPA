#include "QuerySystem.h"

std::list<std::string> QuerySystem::processQuery(std::string input) {
	QueryPreprocessor preprocessor;
	QueryEvaluator evaluator;
	try {
		Query query = preprocessor.parse(input);
		std::list<std::string> result = evaluator.evaluateQuery(query);
		return result;
	}
	catch (...) {
		//Some error occur, return empty list
		return {};
	}
}