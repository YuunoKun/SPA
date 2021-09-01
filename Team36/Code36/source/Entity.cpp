#include "Entity.h"


Entity::Entity() {
}

Entity::Entity(EntityType entity_type, std::string name) {
	this->entity_type = entity_type;
	this->name = name;
}

Entity::Entity(EntityType entity_type, std::string name, std::string value) {
	this->entity_type = entity_type;
	this->name = name;
	this->value = value;
}

EntityType Entity::getType() {
	return entity_type;
}

std::string Entity::getName() {
	return name;
}

std::string Entity::getValue() {
	return value;
}

bool Entity::isDeclaration() {
	return name != "";
}


bool Entity::operator==(const Entity& entity) const {
	return entity_type == entity.entity_type
		&& name == entity.name
		&& value == entity.value;
}