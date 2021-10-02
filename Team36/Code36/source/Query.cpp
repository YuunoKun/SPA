#include <iostream>
#include "Query.h"
#include <vector>

Query::Query() {
}

std::unordered_map<std::string, Entity> Query::getEntities() {
	return entities;
}

std::vector<Clause> Query::getClauses() {
	return clauses;
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
	clauses.push_back({ relation });
}

void Query::addPattern(Pattern& pattern) {
	clauses.push_back({ pattern });
}

bool Query::operator==(const Query& query) const {
	return entities == query.entities &&
		clauses == query.clauses &&
		selected == query.selected;
}