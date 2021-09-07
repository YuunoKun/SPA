#pragma once

#include <string>
#include <unordered_set>
#include "Common.h"

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

	Entity(EntityType);
	Entity(EntityType, Synonym);
	Entity(EntityType, std::string);


	EntityType getType();
	std::string getSynonym();
	std::string getValue();
	bool isSynonym();

	bool operator==(const Entity&) const;

private:

	//Member Variable
	EntityType entity_type;
	Synonym synonym;
	std::string value;
	bool is_synonyms;
};
