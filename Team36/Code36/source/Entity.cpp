#include "Entity.h"

Entity::Entity() {
}

Entity::Entity(EntityType entity_type, std::string synonym) {
	this->entity_type = entity_type;
	this->synonym = synonym;
}

Entity::Entity(EntityType entity_type, std::string synonym, std::string value) {
	this->entity_type = entity_type;
	this->synonym = synonym;
	this->value = value;
}

EntityType Entity::getType() {
	return entity_type;
}

std::string Entity::getSynonym() {
	return synonym;
}

std::string Entity::getValue() {
	return value;
}

bool Entity::isSynonym() {
	return synonym != "";
}

bool Entity::operator==(const Entity& entity) const {
	return entity_type == entity.entity_type
		&& synonym == entity.synonym
		&& value == entity.value;
}