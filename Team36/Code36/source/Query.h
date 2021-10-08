#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "Entity.h"
#include "Clause.h"

class Query {
public:
	Query();

	std::unordered_map<std::string, Entity> getEntities();
	std::vector<Clause> getClauses();
	std::vector<Entity> getSelected();
	void setSelectedAttribute(AttrRef&);
	void addSelected(Entity&);
	void addEntity(Entity&);
	void addRelation(RelRef&);
	void addPattern(Pattern&);

	void setClauses(std::vector<Clause>);
	bool operator==(const Query&) const;

private:
	//Member Variable
	std::unordered_map<std::string, Entity> entities;
	std::vector<Clause> clauses;
	std::vector<Entity> selected;
};
