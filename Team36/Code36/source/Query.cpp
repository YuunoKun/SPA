#include "Query.h"

Query::Query() {
}

unordered_map<string, Entity> Query::getEntities() {
	return entities;
}

vector<RelRef> Query::getRelations() {
	return relations;
}

vector<Pattern> Query::getPatterns() {
	return patterns;
}

void Query::addEntity(Entity entity) {
	entities.insert({ entity.getName(), entity });
}

void Query::addRelation(RelRef relation) {
	relations.push_back(relation);
}

void Query::addPattern(Pattern pattern) {
	patterns.push_back(pattern);
}
