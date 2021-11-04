#pragma once
#include <string>
#include <stdexcept>
#include <vector>
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

struct LabelledProgLine {
	prog_line program_line;
	std::vector<prog_line> labels;

	bool operator==(const LabelledProgLine& other) const {
		return program_line == other.program_line && labels == other.labels;
	}

	bool operator<(const LabelledProgLine& other) const {
		if (program_line == other.program_line) {
			return labels < other.labels;
		}
		else {
			return program_line < other.program_line;
		}
	}
};

namespace std {
	template <>
	struct hash<StmtInfo> {
		size_t operator()(const StmtInfo& k) const {
			return ((hash<int>()(k.stmt_index) ^ (hash<int>()(k.stmt_type) << 1)) >> 1);
		}
	};

	template <>
	struct hash<LabelledProgLine> {
		size_t operator()(const LabelledProgLine& l) const {
			int v_hash = l.labels.size();
			for (auto& i : l.labels) v_hash ^= i + 0x9e3779b9 + (v_hash << 6) + (v_hash >> 2);
			return ((hash<int>()(l.program_line) ^ (hash<int>()(v_hash) << 1)) >> 1);
		}
	};
}

constexpr char SPACE_C = ' ';
constexpr char* SPACE_S = "	";
constexpr char TAB_C = '	';
constexpr char* TAB_S = "	";
constexpr char PLUS_SIGN_C = '+';
constexpr char* PLUS_SIGN_S = "+";
constexpr char MINUS_SIGN_C = '-';
constexpr char* MINUS_SIGN_S = "-";
constexpr char ASTERISK_C = '*';
constexpr char* ASTERISK_S = "*";
constexpr char SLASH_C = '/';
constexpr char* SLASH_S = "/";
constexpr char PERCENT_SIGN_C = '%';
constexpr char* PERCENT_SIGN_S = "%";
constexpr char EQUAL_SIGN_C = '=';
constexpr char* EQUAL_SIGN_S = "=";
constexpr char LEFT_PARENTHESIS_C = '(';
constexpr char* LEFT_PARENTHESIS_S = "(";
constexpr char RIGHT_PARENTHESIS_C = ')';
constexpr char* RIGHT_PARENTHESIS_S = ")";
constexpr char SEMICOLON_C = ';';
constexpr char* SEMICOLON_S = ";";

constexpr char T_C = 't';
constexpr char L_C = 'l';
constexpr char TUPLE_OPEN_C = '<';
constexpr char TUPLE_CLOSE_C = '>';
constexpr char QUOTATION_C = '"';
constexpr char UNDERSCORE_C = '_';
constexpr char COMMA_C = ',';
constexpr char NEWLINE_C = '\n';
constexpr char TABS_C = '\t';
constexpr char DOT_C = '.';
constexpr char HASH_C = '#';
constexpr char ZERO_C = '0';
constexpr char* ZERO_S = "0";

constexpr char* EMPTY_S = "";

constexpr char* SUCH_STR = "such";
constexpr char* SUCH_SPACE_STR = "such ";
constexpr char* SUCH_SPACE_THAT_STR = "such that";
constexpr char* PROG_STR = "prog";
constexpr char* PROG_UNDERSCORE_STR = "prog_";
constexpr char* PROG_UNDERSCORE_LINE_STR = "prog_line";

constexpr char* DOT_PROCNAME_STR = ".procName";
constexpr char* DOT_VARNAME_STR = ".varName";
constexpr char* DOT_VALIUE_STR = ".value";
constexpr char* DOT_STMT_STR = ".stmt";
constexpr char* DOT_STMT_HASH_STR = ".stmt#";

constexpr char* USES_STR = "Uses";
constexpr char* MODIFIES_STR = "Modifies";
constexpr char* PARENT_STR = "Parent";
constexpr char* PARENTT_STR = "";
constexpr char* FOLLOWS_STR = "Follows";
constexpr char* FOLLOWST_STR = "";
constexpr char* CALLS_STR = "Calls";
constexpr char* CALLST_STR = "";
constexpr char* NEXT_STR = "Next";
constexpr char* NEXTT_STR = "";
constexpr char* AFFECTS_STR = "Affects";
constexpr char* AFFECTST_STR = "";
constexpr char* AFFECTS_BIP_STR = "AffectsBip";
constexpr char* AFFECTS_BIPT_STR = "";
constexpr char* NEXT_BIP_STR = "NextBip";
constexpr char* NEXT_BIPT_STR = "";

constexpr char* STMT_STR = "stmt";
constexpr char* PROCEDURE_STR = "procedure";
constexpr char* READ_STR = "read";
constexpr char* PRINT_STR = "print";
constexpr char* CALL_STR = "call";
constexpr char* IF_STR = "if";
constexpr char* WHILE_STR = "while";
constexpr char* ASSIGN_STR = "assign";
constexpr char* VARIABLE_STR = "variable";
constexpr char* CONSTANT_STR = "constant";
constexpr char* PROGLINE_STR = "prog_line";
constexpr char* SELECT_STR = "Select";

constexpr char* AND_STR = "and";
constexpr char* PATTERN_STR = "pattern";
constexpr char* WITH_STR = "with";