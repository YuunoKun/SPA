#pragma once

#include <string>
#include <unordered_map>
#include "Common.h"

enum StmtType {
	STMT_READ,
	STMT_PRINT,
	STMT_CALL,
	STMT_WHILE,
	STMT_IF,
	STMT_ASSIGN
};

class Stmt {
public:
	Stmt(STMT_TYPE, stmt_index);

	STMT_TYPE getType();
	stmt_index getNum();

private:
	//Member Variable
	STMT_TYPE stmt_type;
	stmt_index stmt_num;
};
