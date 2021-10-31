#pragma once
#include <string>
#include <stdexcept>
#include "ExprNode.h"

typedef std::string proc_name;
typedef std::string var_name;
typedef ExprNode expr;
typedef unsigned int constant;
typedef unsigned int stmt_index;
typedef unsigned int prog_line;
typedef unsigned int var_index;
typedef unsigned int proc_index;

static const std::string BOOLEAN_TRUE = "TRUE";
static const std::string BOOLEAN_FALSE = "FALSE";

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

	bool operator==(const StmtInfo& another_stmt_info) const {
		return stmt_index == another_stmt_info.stmt_index && stmt_type == another_stmt_info.stmt_type;
	}

	bool operator < (const StmtInfo& st) const {
		return (stmt_index < st.stmt_index);
	}
};

typedef std::pair<stmt_index, var_name> pattern_info;

struct Synonym {
	std::string name;
	Synonym() {
		name = "_";
	};
	Synonym(std::string name) {
		if (name.size() == 0 || !isalpha(name[0])) {
			throw std::invalid_argument("Invalid synonym");
		}
		for (char c : name) {
			if (!isalnum(c)) {
				throw std::invalid_argument("Invalid synonym");
			}
		}
		this->name = name;
	}

	bool operator==(const Synonym& synonyms) const {
		return name == synonyms.name;
	}
};

namespace std {
	template <>
	struct hash<StmtInfo> {
		size_t operator()(const StmtInfo& k) const {
			// Compute individual hash values for two data members and combine them using XOR and bit shifting
			return ((hash<int>()(k.stmt_index) ^ (hash<int>()(k.stmt_type) << 1)) >> 1);
		}
	};
}

struct LabelledProgLine {
	prog_line program_line;
	prog_line label;

	bool operator==(const LabelledProgLine& labelled_progline) const {
		return program_line == labelled_progline.program_line && label == labelled_progline.label;
	}

	bool operator < (const LabelledProgLine& labelled_progline) const {
		return ((program_line < labelled_progline.program_line) || 
			(program_line == labelled_progline.program_line &&
			label < labelled_progline.label));
	}
};

namespace std {
	template <>
	struct hash<LabelledProgLine> {
		size_t operator()(const LabelledProgLine& k) const {
			// Compute individual hash values for two data members and combine them using XOR and bit shifting
			return ((hash<int>()(k.program_line) ^ (hash<int>()(k.label) << 1)) >> 1);
		}
	};
}