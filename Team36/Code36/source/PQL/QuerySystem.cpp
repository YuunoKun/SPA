#include "QuerySystem.h"

#include <iostream>
#include <exception>

#include "Common/SyntacticErrorException.h"
#include "Common/SemanticErrorException.h"
#include "PQL/QueryObject/Query.h"

std::list<std::string> QuerySystem::processQuery(std::string input) {
	QueryPreprocessor preprocessor;
	QueryOptimizer optimizer;
	QueryEvaluator evaluator;
	try {
		Query query = preprocessor.parse(input);

		if (!optimizer.checkAllConstantExist(query.getClauses())) {
			return returnEmptyResult(query.getSelected().front());
		}

		query.setClauses(optimizer.optimizeClausesOrder(query.getClauses()));

		return evaluator.evaluateQuery(query);;
	} catch (SyntacticErrorException&) {
		preprocessor.resetQuery();
		return {};
	} catch (SemanticErrorException& err) {
		preprocessor.resetQuery();

		if (err.getQuery().getSelected().size() == 0) {
			return {};
		} else {
			return returnEmptyResult(err.getQuery().getSelected().front());
		}
	} catch (std::exception& e) {
		std::cout << "QuerySystem processQuery(): " << e.what() << std::endl;
		preprocessor.resetQuery();
		return {};
	}
}

std::list<std::string> QuerySystem::returnEmptyResult(Entity& selected) {
	if (selected.getType() == BOOLEAN) {
		return { BOOLEAN_FALSE };
	} else {
		return { };
	}
}