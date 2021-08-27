#pragma once

#include <string>
#include <unordered_map>
#include "Common.h"

using namespace std;

enum StmtType {
	read,
	print,
	call,
	whileStmt,
	ifStmt,
	assign,
};

class Stmt {
public:
	Stmt();
	Stmt(StmtType, stmt_index);

	StmtType getType();
	stmt_index getNum();

private:
	//Member Variable
	StmtType stmt_type;
	stmt_index stmt_num;
};
