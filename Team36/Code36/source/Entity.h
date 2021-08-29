#pragma once

#include <string>
#include <unordered_set>

using namespace std;

enum EntityType {
	STMT,
	READ,
	PRINT,
	CALL,
	WHILE,
	IF,
	ASSIGN,
	VARIABLE,
	CONSTANT,
	PROCEDURE,
	WILD
};

class Entity {
public:
	Entity(EntityType, string);
	Entity();

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


