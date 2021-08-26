#pragma once

#include <string>
#include <unordered_set>

using namespace std;

enum EntityType {
	stmt,
	read,
	print,
	whileStmt,
	ifStmt,
	assign,
	variable,
	constant,
	procedure,
	wild
};

class Entity {
public:
	Entity(EntityType, string);

	EntityType getType();
	string getName();
	unordered_set<string> getValues();
	void addValue(string);
	void removeValue(string);

private:

	//Member Variable
	EntityType entity_type;
	string name;
	unordered_set<string> values;
};


