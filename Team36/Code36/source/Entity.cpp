#include "Entity.h"

Entity::Entity() {
}

Entity::Entity(EntityType entity_type) {
	this->entity_type = entity_type;
	this->is_synonym = false;
	defaultAttribute(entity_type);
}

Entity::Entity(EntityType entity_type, Synonym synonym) {
	this->entity_type = entity_type;
	this->synonym = synonym;
	this->is_synonym = true;
	defaultAttribute(entity_type);
}

Entity::Entity(EntityType entity_type, std::string value) {
	this->entity_type = entity_type;
	this->value = value;
	this->is_synonym = false;
	defaultAttribute(entity_type);
}

Entity::Entity(EntityType entity_type, Synonym synonym, AttrRef attribute) {
	this->entity_type = entity_type;
	this->synonym = synonym;
	this->is_synonym = true;
	this->attribute = attribute;
}

EntityType Entity::getType() {
	return entity_type;
}

std::string Entity::getSynonym() {
	return synonym.name;
}

std::string Entity::getValue() {
	return value;
}

AttrRef Entity::getAttribute() {
	return attribute;
}

void Entity::setAttribute(AttrRef& attrRef) {
	this->attribute = attrRef;
}

bool Entity::isSynonym() {
	return is_synonym;
}

bool Entity::operator==(const Entity& entity) const {
	return entity_type == entity.entity_type
		&& synonym == entity.synonym
		&& value == entity.value;
}

void Entity::defaultAttribute(EntityType type) {
	switch (type) {
	case STMT:
	case READ:
	case PRINT:
	case CALL:
	case WHILE:
	case IF:
	case ASSIGN: attribute = STMT_INDEX;
		break;
	case VARIABLE: attribute = VAR_NAME;
		break;
	case CONSTANT: attribute = VALUE;
		break;
	case PROCEDURE: attribute = PROC_NAME;
		break;
	case BOOLEAN:
	case WILD: attribute = NONE;
		break;
	default: throw std::domain_error("Some default attribute value is not being handle!!!!");
		break;
	}
}