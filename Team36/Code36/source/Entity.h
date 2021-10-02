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
	WILD,
	BOOLEAN
};

enum AttrRef {
	STMT_INDEX,
	PROC_NAME,
	VAR_NAME,
	VALUE,
	NONE
};

class Entity {
public:
	Entity();

	Entity(EntityType);
	Entity(EntityType, Synonym);
	Entity(EntityType, Synonym, AttrRef);
	Entity(EntityType, std::string);

	EntityType getType();
	std::string getSynonym();
	std::string getValue();
	AttrRef getAttribute();
	bool isSynonym();

	bool operator==(const Entity&) const;

private:

	//Member Variable
	EntityType entity_type;
	Synonym synonym;
	std::string value;
	AttrRef attribute;
	bool is_synonym;

	void defaultAttribute(EntityType);
};
