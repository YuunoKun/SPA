#include "Query.h"

Query::Query() {
}

std::unordered_map<std::string, Entity> Query::getEntities() {
	return entities;
}

std::vector<RelRef> Query::getRelations() {
	return relations;
}

std::vector<Pattern> Query::getPatterns() {
	return patterns;
}

Entity Query::getSelected() {
	return selected;
}

void Query::setSelected(Entity selected) {
	this->selected = selected;
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
