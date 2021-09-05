#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "Entity.h"
#include "RelRef.h"
#include "Pattern.h"

class Query {
public:
	Query();

	std::unordered_map<std::string, Entity> getEntities();
	std::vector<RelRef> getRelations();
	std::vector<Pattern> getPatterns();
	std::vector<Entity> getSelected();
	void addSelected(Entity&);
	void addEntity(Entity&);
	void addRelation(RelRef&);
	void addPattern(Pattern&);

private:
	//Member Variable
	std::unordered_map<std::string, Entity> entities;
	std::vector<RelRef> relations;
	std::vector<Pattern> patterns;
	std::vector<Entity> selected;
};
