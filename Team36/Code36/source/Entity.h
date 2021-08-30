#pragma once

#include <string>
#include <unordered_set>

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
	Entity(EntityType, std::string);
	Entity();

	EntityType getType();
	std::string getName();
	std::unordered_set<std::string> getValues();
	void addValue(std::string);
	void removeValue(std::string);

	bool operator==(const Entity& entity) const;

private:

	//Member Variable
	EntityType entity_type;
	std::string name;
	std::unordered_set<std::string> values;
};
