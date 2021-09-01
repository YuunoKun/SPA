#pragma once

#include <string>
#include <unordered_map>

#include "Entity.h"
#include "RelRef.h"
#include "Pattern.h"

class Query {
public:
	Query();

	std::unordered_map<std::string, Entity*> getEntities();
	std::vector<RelRef> getRelations();
	std::vector<Pattern> getPatterns();
	Entity& getSelected();
	void setSelected(Entity&);
	void addEntity(Entity&);
	void addRelation(RelRef);
	void addPattern(Pattern);

private:
	//Member Variable
	std::unordered_map<std::string, Entity*> entities;
	std::vector<RelRef> relations;
	std::vector<Pattern> patterns;
	Entity* selected;
};
