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

void Entity::setAttribute(AttrRef attrRef) {
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
	case EntityType::STMT:
	case EntityType::PROG_LINE:
	case EntityType::READ:
	case EntityType::PRINT:
	case EntityType::CALL:
	case EntityType::WHILE:
	case EntityType::IF:
	case EntityType::ASSIGN: attribute = AttrRef::STMT_INDEX;
		break;
	case EntityType::VARIABLE: attribute = AttrRef::VAR_NAME;
		break;
	case EntityType::CONSTANT: attribute = AttrRef::VALUE;
		break;
	case EntityType::PROCEDURE: attribute = AttrRef::PROC_NAME;
		break;
	case EntityType::BOOLEAN:
	case EntityType::WILD: attribute = AttrRef::NONE;
		break;
	default: throw std::domain_error("Some default attribute value is not being handle!!!!");
		break;
	}
}