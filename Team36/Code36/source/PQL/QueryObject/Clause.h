#pragma once

#include "RelRef.h"
#include "Pattern.h"

enum ClauseType {
	RELATION,
	PATTERN
};

class Clause {
public:
	Clause(RelRef);
	Clause(Pattern);

	ClauseType getType();
	RelRef getRelation();
	Pattern getPattern();

	bool operator==(const Clause&) const;

private:
	ClauseType type;
	RelRef rel;
	Pattern pattern;

};