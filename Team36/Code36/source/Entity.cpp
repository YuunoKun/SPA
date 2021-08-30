#include "Entity.h"

Entity::Entity(EntityType entity_type, std::string name) {
	this->entity_type = entity_type;
	this->name = name;
}

Entity::Entity() {
}

EntityType Entity::getType() {
	return entity_type;
}

std::string Entity::getName() {
	return name;
}

std::unordered_set<std::string> Entity::getValues() {
	return values;
}

void Entity::addValue(std::string value) {
	values.insert(value);
}

void Entity::removeValue(std::string value) {
	values.erase(value);
}

bool Entity::operator==(const Entity& entity) const {
	return entity_type == entity.entity_type
		&& name == entity.name
		&& values == entity.values;
}