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
	BOOLEAN,
	PROG_LINE,
	NONE_ENTITY
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
	Entity(EntityType, std::string, AttrRef);

	EntityType getType();
	std::string getSynonym();
	std::string getValue();
	AttrRef getAttribute();
	void setAttribute(AttrRef);
	bool isSynonym();

	bool operator==(const Entity&) const;

private:
	EntityType entity_type;
	Synonym synonym;
	std::string value;
	AttrRef attribute;
	bool is_synonym;

	void defaultAttribute(EntityType);
};
