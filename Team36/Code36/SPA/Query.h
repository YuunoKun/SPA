#pragma once

#include <string>
#include <unordered_map>

#include "Entity.h"
#include "RelRef.h"
#include "Pattern.h"

using namespace std;


class Query {
public:
	Query();

	unordered_map<string, Entity> getEntities();
	vector<RelRef> getRelations();
	vector<Pattern> getPatterns();
	void addEntity(Entity);
	void addRelation(RelRef);
	void addPattern(Pattern);

private:

	//Member Variable
	unordered_map<string, Entity> entities;
	vector<RelRef> relations;
	vector<Pattern> patterns;
};


