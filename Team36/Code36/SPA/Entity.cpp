#include "Entity.h"


Entity::Entity(EntityType entity_type, string name) {
	this->entity_type = entity_type;
	this->name = name;
}

EntityType Entity::getType() {
	return entity_type;
}

string Entity::getName() {
	return name;
}

unordered_set<string> Entity::getValues() {
	return values;
}

void Entity::addValue(string value) {
	values.insert(value);
}

void Entity::removeValue(string value) {
	values.erase(value);
}
