#pragma once

#include <string>
#include <unordered_map>
#include "Common.h"

class Stmt {
public:
	Stmt(StmtType, stmt_index);

	StmtType getType();
	stmt_index getNum();

private:
	//Member Variable
	StmtType stmt_type;
	stmt_index stmt_num;
};