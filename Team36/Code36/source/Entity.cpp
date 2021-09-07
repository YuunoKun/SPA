#include "Entity.h"

Entity::Entity() {
}

Entity::Entity(EntityType entity_type) {
	this->entity_type = entity_type;
	this->is_synonym = false;
}

Entity::Entity(EntityType entity_type, Synonym synonym) {
	this->entity_type = entity_type;
	this->synonym = synonym;
	this->is_synonym = true;
}

Entity::Entity(EntityType entity_type, std::string value) {
	this->entity_type = entity_type;
	this->value = value;
	this->is_synonym = false;
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

bool Entity::isSynonym() {
	return is_synonym;
}

bool Entity::operator==(const Entity& entity) const {
	return entity_type == entity.entity_type
		&& synonym == entity.synonym
		&& value == entity.value;
}