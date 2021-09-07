#pragma once
#include <string>

typedef std::string proc_name;
typedef std::string var_name;
typedef unsigned int constant;
typedef unsigned int stmt_index;
typedef unsigned int var_index;
typedef unsigned int proc_index;

enum StmtType {
	STMT_READ,
	STMT_PRINT,
	STMT_CALL,
	STMT_WHILE,
	STMT_IF,
	STMT_ASSIGN
};

struct StmtInfo {
	stmt_index stmt_index;
	StmtType stmt_type;

	bool operator==(const StmtInfo& another_stmt_info) const
	{
		return stmt_index == another_stmt_info.stmt_index && stmt_type == another_stmt_info.stmt_type;
	}

	bool operator < (const StmtInfo& st) const {
		return (stmt_index < st.stmt_index);
	}
};

struct Synonym {
	std::string name;

	bool operator==(const Synonym& synonyms) const {
		return name == synonyms.name;
	}
};

namespace std
{
	template <>
	struct hash<StmtInfo>
	{
		size_t operator()(const StmtInfo& k) const
		{
			// Compute individual hash values for two data members and combine them using XOR and bit shifting
			return ((hash<int>()(k.stmt_index) ^ (hash<int>()(k.stmt_type) << 1)) >> 1);
		}
	};
}