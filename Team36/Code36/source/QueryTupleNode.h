#pragma once

#include <string>

#include "Entity.h"

class QueryTupleNode {
public:

	//bool operator==(const Pattern&) const;

	QueryTupleNode(Entity, bool);
	QueryTupleNode(Entity);
	QueryTupleNode();

private:
	Entity entity;
	bool isSelected;

	void setIsSelected(bool);
	bool getIsSelected();
};