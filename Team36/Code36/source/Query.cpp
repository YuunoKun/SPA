#include <iostream>
#include "Query.h"
#include <vector>

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

std::vector<Entity> Query::getSelected() {
	return selected;
}

void Query::addSelected(Entity& addSelect) {
	selected.push_back(addSelect);
}

void Query::addEntity(Entity& entity) {
	entities.insert({ entity.getSynonym(), entity });
}

void Query::addRelation(RelRef& relation) {
	relations.push_back(relation);
}

void Query::addPattern(Pattern& pattern) {
	patterns.push_back(pattern);
}

bool Query::operator==(const Query& query) const {
	return entities == query.entities &&
		relations == query.relations &&
		patterns == query.patterns &&
		selected == query.selected;
}