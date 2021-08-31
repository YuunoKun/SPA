#include "QueryResult.h"

bool QueryResult::haveResult() {
	return false;
}

bool QueryResult::isInTable(Entity) {
	return false;
}

void QueryResult::join(QueryResult) {
}
