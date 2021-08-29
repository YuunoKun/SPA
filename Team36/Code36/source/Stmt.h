#pragma once

#include <string>
#include <unordered_map>
#include "Common.h"

enum STMT_TYPE {
	readStmt,
	printStmt,
	callStmt,
	whileStmt,
	ifStmt,
	assignStmt
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
