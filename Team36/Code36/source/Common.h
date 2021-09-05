#include <string>

typedef std::string procedure_name;
typedef std::string variable_name;
typedef unsigned int constant;
typedef unsigned int stmt_index;
typedef unsigned int procedure_index;

typedef enum STMT_TYPE {
	STMT_READ,
	STMT_PRINT,
	STMT_CALL,
	STMT_WHILE,
	STMT_IF,
	STMT_ASSIGN
} StmtType;