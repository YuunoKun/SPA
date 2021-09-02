#include "QueryResult.h"

bool QueryResult::haveResult() {
	return true;
}

bool QueryResult::isInTable(Entity e) {
	return false;
}

void QueryResult::addResult(ResultTable table) {
}