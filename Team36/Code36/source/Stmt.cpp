#include "Stmt.h"

Stmt::Stmt(StmtType stmt_type, stmt_index stmt_num) {
	this->stmt_type = stmt_type;
	this->stmt_num = stmt_num;
}

StmtType Stmt::getType() {
	return stmt_type;
}

stmt_index Stmt::getNum() {
	return stmt_num;
}