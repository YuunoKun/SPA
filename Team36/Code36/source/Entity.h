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
	Entity();
	Entity(EntityType, std::string);
	Entity(EntityType, std::string, std::string);

	EntityType getType();
	std::string getSynonym();
	std::string getValue();
	bool isSynonym();

	bool operator==(const Entity&) const;

private:

	//Member Variable
	EntityType entity_type;
	std::string synonym;
	std::string value;
};
