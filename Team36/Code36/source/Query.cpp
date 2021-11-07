#include <iostream>

#include "Query.h"

Query::Query() {
	this->isSemanticError = "";
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

void Query::setSelectedAttribute(AttrRef& attrRef) {
	int last_index = selected.size() - 1;
	selected[last_index].setAttribute(attrRef);
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

void Query::setClauses(std::vector<Clause> clauses) {
	this->clauses = clauses;
}

void Query::setIsSemanticError(std::string error) {
	this->isSemanticError = error;
}

std::string Query::getIsSemanticError() {
	return this->isSemanticError;
}

bool Query::operator==(const Query& query) const {
	return entities == query.entities &&
		clauses == query.clauses &&
		selected == query.selected;
}