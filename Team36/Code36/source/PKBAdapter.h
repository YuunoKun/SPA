#pragma once

#include <iostream>
#include <vector>

#include "Stmt.h"
#include "Common.h"

class PKBAdapter {
public:
	PKBAdapter();

	std::vector<var_name> getVariables();
	std::vector<Stmt> getStmts();
	std::vector<constant> getConstants();
	std::vector<proc_name> getProcedures();
};