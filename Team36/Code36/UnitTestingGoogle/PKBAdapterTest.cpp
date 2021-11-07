#include "pch.h"

#include "PKBAdapter.h"
#include "PKB.h"
#include "RelationTable.cpp"
#include <ExprParser.h>

namespace UnitTesting {
	class MockPKB : public PKBQueryInterface {
	public:
		const std::vector<proc_name>& MockPKB::getProcedures() {
			return mock_proc_table;
		}

		const std::vector<var_name>& MockPKB::getVariables() {
			return mock_var_table;
		}

		const std::vector<StmtInfo>& MockPKB::getStmts() {
			return mock_stmt_table;
		}

		const StmtInfo MockPKB::getStmt(stmt_index stmt_index) {
			if (stmt_index <= 0) {
				throw std::invalid_argument("Stmt index must be greater than zero. ");
			} else if (stmt_index > mock_stmt_table.size()) {
				throw std::invalid_argument("Invalid stmt index. ");
			}
			return mock_stmt_table[stmt_index - 1];
		}

		const var_name MockPKB::getAssignment(stmt_index stmt_index) {
			if (stmt_index <= 0) {
				throw std::invalid_argument("Stmt index must be greater than zero: " + std::to_string(stmt_index));
			} else if (stmt_index > mock_stmt_table.size()) {
				throw std::invalid_argument("Invalid stmt index: " + std::to_string(stmt_index));
			} else if (mock_stmt_table[stmt_index - 1].stmt_type != STMT_ASSIGN) {
				throw std::invalid_argument("Stmt index does not belong to an assignment statement: " + std::to_string(stmt_index));
			} else if (!mock_assignment_table.containsKey(stmt_index)) {
				throw std::invalid_argument("Stmt-related assignment has not been initiated: " + std::to_string(stmt_index));
			}
			return mock_assignment_table.getValues(stmt_index)[0];
		}

		expr MockPKB::getExpression(stmt_index stmt_index) {
			if (stmt_index <= 0) {
				throw std::invalid_argument("Stmt index must be greater than zero: " + std::to_string(stmt_index));
			} else if (stmt_index > mock_stmt_table.size()) {
				throw std::invalid_argument("Invalid stmt index: " + std::to_string(stmt_index));
			} else if (mock_stmt_table[stmt_index - 1].stmt_type != STMT_ASSIGN) {
				throw std::invalid_argument("Stmt index does not belong to an assignment statement: " + std::to_string(stmt_index));
			} else if (mock_expr_table.find(stmt_index) == mock_expr_table.end()) {
				throw std::invalid_argument("Stmt-related expression has not been initiated: " + std::to_string(stmt_index));
			}
			return mock_expr_table.at(stmt_index);
		}

		const std::vector<constant> MockPKB::getConstants() {
			std::vector<constant> v(mock_const_table.begin(), mock_const_table.end());
			return v;
		}

		const RelationTable<stmt_index, var_name>& MockPKB::getAssigns() {
			return mock_assignment_table;
		}

		const std::unordered_map<stmt_index, expr>& MockPKB::getExpr() {
			return mock_expr_table;
		}

		const MonotypeRelationTable<StmtInfo>& MockPKB::getFollows() {
			return mock_follows_table;
		}

		const MonotypeRelationTable<StmtInfo>& MockPKB::getParent() {
			return mock_parent_table;
		}

		const MonotypeRelationTable<StmtInfo>& MockPKB::getFollowsT() {
			return mock_followsT_table;
		}

		const MonotypeRelationTable<StmtInfo>& MockPKB::getParentT() {
			return mock_parentT_table;
		}

		const RelationTable<StmtInfo, var_name>& MockPKB::getUsesS() {
			return mock_usesS_table;
		}

		const RelationTable<StmtInfo, var_name>& MockPKB::getModifiesS() {
			return mock_modifiesS_table;
		}

		const RelationTable<proc_name, var_name>& MockPKB::getUsesP() {
			return mock_usesP_table;
		}

		const RelationTable<proc_name, var_name>& MockPKB::getModifiesP() {
			return mock_modifiesP_table;
		}

		const MonotypeRelationTable<proc_name>& MockPKB::getCallsP() {
			return mock_callsP_table;
		}

		const MonotypeRelationTable<proc_name>& MockPKB::getCallsPT() {
			return mock_callsPT_table;
		}

		const RelationTable<stmt_index, var_name>& MockPKB::getRead() {
			return mock_read_table;
		}

		const RelationTable<stmt_index, var_name>& MockPKB::getPrint() {
			return mock_print_table;
		}

		const RelationTable<stmt_index, proc_name>& MockPKB::getCallsS() {
			return mock_callsS_table;
		}

		const RelationTable<stmt_index, var_name>& MockPKB::getIf() {
			return mock_if_table;
		}

		const RelationTable<stmt_index, var_name>& MockPKB::getWhile() {
			return mock_while_table;
		}

		const MonotypeRelationTable<StmtInfo>& MockPKB::getNext() {
			return mock_next_table;
		}

		const RelationTable<proc_name, stmt_index>& MockPKB::getProcContains() {
			return mock_proc_contains_table;
		}

		std::vector<CFG*> getCFGBips() {
			return mock_cfgs;
		};

		void resetCache() {
			mock_const_table.clear();
			mock_proc_table.clear();
			mock_var_table.clear();
			mock_stmt_table.clear();
			mock_assignment_table.clear();
			mock_follows_table.clear();
			mock_parent_table.clear();
			mock_followsT_table.clear();
			mock_parentT_table.clear();
			mock_usesS_table.clear();
			mock_modifiesS_table.clear();
			mock_usesP_table.clear();
			mock_modifiesP_table.clear();
			mock_callsP_table.clear();
			mock_callsS_table.clear();
			mock_callsPT_table.clear();
			mock_if_table.clear();
			mock_while_table.clear();
			mock_next_table.clear();
			mock_read_table.clear();
			mock_print_table.clear();
			mock_expr_table.clear();
			mock_proc_contains_table.clear();
			mock_cfgs = {};
		};

		void resetEntities() {
			mock_const_table.clear();
			mock_proc_table.clear();
			mock_var_table.clear();
			mock_stmt_table.clear();
		};

		void addConstant(constant constant) {
			mock_const_table.push_back(constant);
		}

		void addProcedure(proc_name proc_name) {
			mock_proc_table.push_back(proc_name);
		}

		void addVariable(var_name var_name) {
			mock_var_table.push_back(var_name);
		}

		void addStmt(StmtType stmt_type) {
			mock_stmt_table.push_back({ mock_curr_stmt_id, stmt_type });
			mock_curr_stmt_id++;
		}

		void addExprTree(stmt_index stmt_index, expr expr) {
			mock_expr_table.insert({ stmt_index, expr });
		}

		void addParent(stmt_index parent, stmt_index child) {
			mock_parent_table.insert(mock_stmt_table[parent - 1], mock_stmt_table[child - 1]);
		}

		void addParentT(stmt_index parent, stmt_index child) {
			mock_parentT_table.insert(mock_stmt_table[parent - 1], mock_stmt_table[child - 1]);
		}

		void addFollows(stmt_index first, stmt_index second) {
			mock_follows_table.insert(mock_stmt_table[first - 1], mock_stmt_table[second - 1]);
		}

		void addFollowsT(stmt_index first, stmt_index second) {
			mock_followsT_table.insert(mock_stmt_table[first - 1], mock_stmt_table[second - 1]);
		}

		void addUsesS(stmt_index user, var_name used) {
			StmtType user_stmt_type = mock_stmt_table[user - 1].stmt_type;
			if (user_stmt_type == STMT_PRINT) {
				mock_print_table.insert(user, used);
			}
			mock_usesS_table.insert(mock_stmt_table[user - 1], used);
		}

		void addModifiesS(stmt_index modifier, var_name modified) {
			StmtType modifier_stmt_type = mock_stmt_table[modifier - 1].stmt_type;
			if (modifier_stmt_type == STMT_ASSIGN) {
				mock_assignment_table.insert(modifier, modified);
			} else if (modifier_stmt_type == STMT_READ) {
				mock_read_table.insert(modifier, modified);
			}
			mock_modifiesS_table.insert(mock_stmt_table[modifier - 1], modified);
		}

		void addModifiesP(proc_name proc, var_name modified) {
			mock_modifiesP_table.insert(proc, modified);
		}

		void addUsesP(proc_name proc, var_name used) {
			mock_usesP_table.insert(proc, used);
		}

		void addNext(prog_line prog_line1, prog_line prog_line2) {
			mock_next_table.insert(mock_stmt_table[prog_line1 - 1], mock_stmt_table[prog_line2 - 1]);
		}

		void addCallsP(proc_name caller, proc_name callee) {
			mock_callsP_table.insert(caller, callee);
		}

		void addCallsPT(proc_name caller, proc_name callee) {
			mock_callsPT_table.insert(caller, callee);
		}

		void addCallsS(stmt_index caller_stmt, proc_name callee) {
			mock_callsS_table.insert(caller_stmt, callee);
		}

		void addIf(stmt_index if_stmt_index, var_name control_var) {
			mock_if_table.insert(if_stmt_index, control_var);
		}

		void addWhile(stmt_index while_stmt_index, var_name control_var) {
			mock_while_table.insert(while_stmt_index, control_var);
		}

		void addProcContains(proc_name proc, stmt_index index) {
			mock_proc_contains_table.insert(proc, index);
		}

		void addCFGsToDestroy(std::vector<CFG*> cfgs) {
			mock_cfgs_to_destroy = cfgs;
		}

		void addCFGBip(CFG* cfg) {
			mock_cfgs.push_back(cfg);
		}

		std::vector<proc_name> mock_proc_table;
		std::vector<StmtInfo> mock_stmt_table;
		std::vector<var_name> mock_var_table;
		std::vector<constant> mock_const_table;

		std::unordered_map<stmt_index, expr> mock_expr_table;
		RelationTable<stmt_index, var_name> mock_assignment_table;

		MonotypeRelationTable<StmtInfo> mock_parent_table;
		MonotypeRelationTable<StmtInfo> mock_parentT_table;
		MonotypeRelationTable<StmtInfo> mock_follows_table;
		MonotypeRelationTable<StmtInfo> mock_followsT_table;
		RelationTable<StmtInfo, var_name> mock_usesS_table;
		RelationTable<proc_name, var_name> mock_usesP_table;
		RelationTable<StmtInfo, var_name> mock_modifiesS_table;
		RelationTable<proc_name, var_name> mock_modifiesP_table;
		RelationTable<stmt_index, proc_name> mock_callsS_table;
		MonotypeRelationTable<proc_name> mock_callsP_table;
		MonotypeRelationTable<proc_name> mock_callsPT_table;
		MonotypeRelationTable<StmtInfo> mock_next_table;
		std::vector<CFG*> mock_cfgs;
		std::vector<CFG*> mock_cfgs_to_destroy;
		RelationTable<proc_name, stmt_index> mock_proc_contains_table;
		RelationTable<stmt_index, var_name> mock_if_table;
		RelationTable<stmt_index, var_name> mock_while_table;
		RelationTable<stmt_index, var_name> mock_read_table;
		RelationTable<stmt_index, var_name> mock_print_table;

		stmt_index mock_curr_stmt_id{ 1 };
	};

	class PKBAdapterTest : public testing::Test {
	protected:
		PKBAdapterTest() {
		}

		virtual void SetUp() override {
			mock_pkb.resetCache();
		}
		MockPKB mock_pkb;
		PKBAdapter pkb{ mock_pkb };
	};

	TEST_F(PKBAdapterTest, getVariables) {
		var_name a = "a";
		var_name b = "b";
		var_name c = "c";
		var_name d = "d";
		std::vector<var_name> v{ a };
		mock_pkb.addVariable(a);
		EXPECT_EQ(pkb.getVariables(), v);

		v.push_back(b);
		mock_pkb.addVariable(b);
		EXPECT_EQ(pkb.getVariables(), v);

		v.push_back(c);
		mock_pkb.addVariable(c);
		EXPECT_EQ(pkb.getVariables(), v);

		v.push_back(d);
		mock_pkb.addVariable(d);
		EXPECT_EQ(pkb.getVariables(), v);
	}

	TEST_F(PKBAdapterTest, getStmts) {
		std::vector<StmtInfo> v = { { 1, STMT_WHILE }, { 2, STMT_IF} , { 3, STMT_READ },
			{ 4, STMT_PRINT}, { 5, STMT_CALL }, { 6, STMT_ASSIGN}, { 7, STMT_ASSIGN} };

		mock_pkb.addStmt(STMT_WHILE);
		mock_pkb.addStmt(STMT_IF);
		mock_pkb.addStmt(STMT_READ);
		mock_pkb.addStmt(STMT_PRINT);
		mock_pkb.addStmt(STMT_CALL);
		mock_pkb.addStmt(STMT_ASSIGN);
		mock_pkb.addStmt(STMT_ASSIGN);

		EXPECT_EQ(pkb.getStmts(), v);
	}

	TEST_F(PKBAdapterTest, getConstants) {
		constant a = 1;
		constant b = 2;
		constant c = 3;
		constant d = 4;
		std::vector<constant> v{ a };
		mock_pkb.addConstant(a);
		EXPECT_EQ(pkb.getConstants(), v);

		v.push_back(b);
		mock_pkb.addConstant(b);
		EXPECT_EQ(pkb.getConstants(), v);

		v.push_back(c);
		mock_pkb.addConstant(c);
		EXPECT_EQ(pkb.getConstants(), v);

		v.push_back(d);
		mock_pkb.addConstant(d);
		EXPECT_EQ(pkb.getConstants(), v);
	}

	TEST_F(PKBAdapterTest, getProcedures) {
		proc_name a = "a";
		proc_name b = "b";
		proc_name c = "c";
		proc_name d = "d";
		std::vector<proc_name> v{ a };
		mock_pkb.addProcedure(a);
		EXPECT_EQ(pkb.getProcedures(), v);

		v.push_back(b);
		mock_pkb.addProcedure(b);
		EXPECT_EQ(pkb.getProcedures(), v);

		v.push_back(c);
		mock_pkb.addProcedure(c);
		EXPECT_EQ(pkb.getProcedures(), v);

		v.push_back(d);
		mock_pkb.addProcedure(d);
		EXPECT_EQ(pkb.getProcedures(), v);
	}
	TEST_F(PKBAdapterTest, isVariables) {
		var_name a = "a";
		var_name b = "b";
		var_name c = "c";
		var_name d = "d";
		var_name e = "e";
		mock_pkb.addVariable(a);
		mock_pkb.addVariable(b);
		mock_pkb.addVariable(c);
		mock_pkb.addVariable(d);
		EXPECT_TRUE(pkb.isVariable(a));
		EXPECT_TRUE(pkb.isVariable(b));
		EXPECT_TRUE(pkb.isVariable(c));
		EXPECT_TRUE(pkb.isVariable(d));
		EXPECT_FALSE(pkb.isVariable(e));
	}

	TEST_F(PKBAdapterTest, istStmts) {
		mock_pkb.addStmt(STMT_WHILE);
		mock_pkb.addStmt(STMT_IF);
		mock_pkb.addStmt(STMT_READ);
		mock_pkb.addStmt(STMT_PRINT);
		mock_pkb.addStmt(STMT_CALL);
		mock_pkb.addStmt(STMT_ASSIGN);
		mock_pkb.addStmt(STMT_ASSIGN);

		EXPECT_FALSE(pkb.isStmt(0));
		EXPECT_TRUE(pkb.isStmt(1));
		EXPECT_TRUE(pkb.isStmt(2));
		EXPECT_TRUE(pkb.isStmt(3));
		EXPECT_TRUE(pkb.isStmt(4));
		EXPECT_TRUE(pkb.isStmt(5));
		EXPECT_TRUE(pkb.isStmt(6));
		EXPECT_TRUE(pkb.isStmt(7));
		EXPECT_FALSE(pkb.isStmt(8));
	}

	TEST_F(PKBAdapterTest, isConstants) {
		constant a = 1;
		constant b = 2;
		constant c = 3;
		constant d = 4;
		constant e = 5;
		mock_pkb.addConstant(a);
		mock_pkb.addConstant(b);
		mock_pkb.addConstant(c);
		mock_pkb.addConstant(d);
		EXPECT_TRUE(pkb.isConstant(a));
		EXPECT_TRUE(pkb.isConstant(b));
		EXPECT_TRUE(pkb.isConstant(c));
		EXPECT_TRUE(pkb.isConstant(d));
		EXPECT_FALSE(pkb.isConstant(e));
	}

	TEST_F(PKBAdapterTest, isProcedure) {
		proc_name a = "a";
		proc_name b = "b";
		proc_name c = "c";
		proc_name d = "d";
		proc_name e = "e";
		mock_pkb.addProcedure(a);
		mock_pkb.addProcedure(b);
		mock_pkb.addProcedure(c);
		mock_pkb.addProcedure(d);
		EXPECT_TRUE(pkb.isProcedure(a));
		EXPECT_TRUE(pkb.isProcedure(b));
		EXPECT_TRUE(pkb.isProcedure(c));
		EXPECT_TRUE(pkb.isProcedure(d));
		EXPECT_FALSE(pkb.isProcedure(e));
	}

	TEST_F(PKBAdapterTest, isAssigns) {
		mock_pkb.addStmt(STMT_WHILE);
		mock_pkb.addStmt(STMT_IF);
		mock_pkb.addStmt(STMT_READ);
		mock_pkb.addStmt(STMT_PRINT);
		mock_pkb.addStmt(STMT_CALL);
		mock_pkb.addStmt(STMT_ASSIGN);
		mock_pkb.addStmt(STMT_ASSIGN);

		EXPECT_FALSE(pkb.isAssign(0));
		EXPECT_FALSE(pkb.isAssign(1));
		EXPECT_FALSE(pkb.isAssign(2));
		EXPECT_FALSE(pkb.isAssign(3));
		EXPECT_FALSE(pkb.isAssign(4));
		EXPECT_FALSE(pkb.isAssign(5));
		EXPECT_TRUE(pkb.isAssign(6));
		EXPECT_TRUE(pkb.isAssign(7));
		EXPECT_FALSE(pkb.isAssign(8));
	}

	TEST_F(PKBAdapterTest, isCalls) {
		mock_pkb.addStmt(STMT_WHILE);
		mock_pkb.addStmt(STMT_IF);
		mock_pkb.addStmt(STMT_READ);
		mock_pkb.addStmt(STMT_PRINT);
		mock_pkb.addStmt(STMT_CALL);
		mock_pkb.addStmt(STMT_CALL);
		mock_pkb.addStmt(STMT_CALL);
		mock_pkb.addStmt(STMT_ASSIGN);
		mock_pkb.addStmt(STMT_ASSIGN);
		mock_pkb.addStmt(STMT_CALL);

		EXPECT_FALSE(pkb.isCall(0));
		EXPECT_FALSE(pkb.isCall(1));
		EXPECT_FALSE(pkb.isCall(2));
		EXPECT_FALSE(pkb.isCall(3));
		EXPECT_FALSE(pkb.isCall(4));
		EXPECT_TRUE(pkb.isCall(5));
		EXPECT_TRUE(pkb.isCall(6));
		EXPECT_TRUE(pkb.isCall(7));
		EXPECT_FALSE(pkb.isCall(8));
		EXPECT_FALSE(pkb.isCall(9));
		EXPECT_TRUE(pkb.isCall(10));
		EXPECT_FALSE(pkb.isCall(11));
	}

	TEST_F(PKBAdapterTest, isReads) {
		mock_pkb.addStmt(STMT_WHILE);
		mock_pkb.addStmt(STMT_IF);
		mock_pkb.addStmt(STMT_READ);
		mock_pkb.addStmt(STMT_PRINT);
		mock_pkb.addStmt(STMT_CALL);
		mock_pkb.addStmt(STMT_ASSIGN);
		mock_pkb.addStmt(STMT_ASSIGN);

		EXPECT_FALSE(pkb.isRead(0));
		EXPECT_FALSE(pkb.isRead(1));
		EXPECT_FALSE(pkb.isRead(2));
		EXPECT_TRUE(pkb.isRead(3));
		EXPECT_FALSE(pkb.isRead(4));
		EXPECT_FALSE(pkb.isRead(5));
		EXPECT_FALSE(pkb.isRead(6));
		EXPECT_FALSE(pkb.isRead(7));
		EXPECT_FALSE(pkb.isRead(8));
	}

	TEST_F(PKBAdapterTest, isWhiles) {
		mock_pkb.addStmt(STMT_WHILE);
		mock_pkb.addStmt(STMT_IF);
		mock_pkb.addStmt(STMT_READ);
		mock_pkb.addStmt(STMT_PRINT);
		mock_pkb.addStmt(STMT_CALL);
		mock_pkb.addStmt(STMT_ASSIGN);
		mock_pkb.addStmt(STMT_ASSIGN);

		EXPECT_FALSE(pkb.isWhile(0));
		EXPECT_TRUE(pkb.isWhile(1));
		EXPECT_FALSE(pkb.isWhile(2));
		EXPECT_FALSE(pkb.isWhile(3));
		EXPECT_FALSE(pkb.isWhile(4));
		EXPECT_FALSE(pkb.isWhile(5));
		EXPECT_FALSE(pkb.isWhile(6));
		EXPECT_FALSE(pkb.isWhile(7));
		EXPECT_FALSE(pkb.isWhile(8));
	}

	TEST_F(PKBAdapterTest, isIf) {
		mock_pkb.addStmt(STMT_WHILE);
		mock_pkb.addStmt(STMT_IF);
		mock_pkb.addStmt(STMT_READ);
		mock_pkb.addStmt(STMT_PRINT);
		mock_pkb.addStmt(STMT_CALL);
		mock_pkb.addStmt(STMT_ASSIGN);
		mock_pkb.addStmt(STMT_ASSIGN);

		EXPECT_FALSE(pkb.isIf(0));
		EXPECT_FALSE(pkb.isIf(1));
		EXPECT_TRUE(pkb.isIf(2));
		EXPECT_FALSE(pkb.isIf(3));
		EXPECT_FALSE(pkb.isIf(4));
		EXPECT_FALSE(pkb.isIf(5));
		EXPECT_FALSE(pkb.isIf(6));
		EXPECT_FALSE(pkb.isIf(7));
		EXPECT_FALSE(pkb.isIf(8));
	}

	TEST_F(PKBAdapterTest, getAssignInfo) {
		ExprParser expr_parser;

		std::vector<var_name> v = { "x", "y", "z", "y", "z", "z" };
		std::vector<std::string> e = { "x", "y*x", "x+y", "z%x", "x-x", "x+y-z" };
		std::vector<stmt_index> s = { 1, 2, 3, 4, 5, 6 };

		std::vector<pattern_info> a;

		for (unsigned int i = 0; i < s.size(); i++) {
			mock_pkb.addStmt(STMT_ASSIGN);
			mock_pkb.addVariable(v[i]);
			a.push_back(std::make_pair(s[i], v[i]));
			mock_pkb.addModifiesS(s[i], v[i]);
			mock_pkb.addExprTree(s[i], expr_parser.parse(e[i]));
		}

		EXPECT_EQ(pkb.getAssignInfo(), a);
		std::vector<pattern_info> result = { a[0] };
		EXPECT_EQ(pkb.getAssignInfo(expr_parser.parse("x"), false), result);
		EXPECT_EQ(pkb.getAssignInfo(expr_parser.parse("x "), false), result);
		EXPECT_EQ(pkb.getAssignInfo(expr_parser.parse("x  "), false), result);
		EXPECT_EQ(pkb.getAssignInfo(expr_parser.parse(" x"), false), result);
		EXPECT_EQ(pkb.getAssignInfo(expr_parser.parse("  x"), false), result);
		EXPECT_EQ(pkb.getAssignInfo(expr_parser.parse("  x "), false), result);
		EXPECT_EQ(pkb.getAssignInfo(expr_parser.parse("  x  "), false), result);

		result = a;
		EXPECT_EQ(pkb.getAssignInfo(expr_parser.parse("x"), true), result);
		EXPECT_EQ(pkb.getAssignInfo(expr_parser.parse("x "), true), result);
		EXPECT_EQ(pkb.getAssignInfo(expr_parser.parse("x  "), true), result);
		EXPECT_EQ(pkb.getAssignInfo(expr_parser.parse(" x"), true), result);
		EXPECT_EQ(pkb.getAssignInfo(expr_parser.parse("  x"), true), result);
		EXPECT_EQ(pkb.getAssignInfo(expr_parser.parse("  x "), true), result);
		EXPECT_EQ(pkb.getAssignInfo(expr_parser.parse("  x  "), true), result);

		result = { a[1], a[2], a[5] };
		EXPECT_EQ(pkb.getAssignInfo(expr_parser.parse("y"), true), result);
		EXPECT_EQ(pkb.getAssignInfo(expr_parser.parse("y "), true), result);
		EXPECT_EQ(pkb.getAssignInfo(expr_parser.parse(" y"), true), result);
		EXPECT_EQ(pkb.getAssignInfo(expr_parser.parse("  y  "), true), result);

		result = { a[3], a[5] };
		EXPECT_EQ(pkb.getAssignInfo(expr_parser.parse("z"), true), result);
		EXPECT_EQ(pkb.getAssignInfo(expr_parser.parse("z "), true), result);
		EXPECT_EQ(pkb.getAssignInfo(expr_parser.parse(" z"), true), result);
		EXPECT_EQ(pkb.getAssignInfo(expr_parser.parse("  z  "), true), result);

		result = {};
		EXPECT_EQ(pkb.getAssignInfo(expr_parser.parse("y"), false), result);
		EXPECT_EQ(pkb.getAssignInfo(expr_parser.parse("y "), false), result);
		EXPECT_EQ(pkb.getAssignInfo(expr_parser.parse(" y"), false), result);
		EXPECT_EQ(pkb.getAssignInfo(expr_parser.parse("  y  "), false), result);
		EXPECT_EQ(pkb.getAssignInfo(expr_parser.parse("z"), false), result);
		EXPECT_EQ(pkb.getAssignInfo(expr_parser.parse("z "), false), result);
		EXPECT_EQ(pkb.getAssignInfo(expr_parser.parse(" z"), false), result);
		EXPECT_EQ(pkb.getAssignInfo(expr_parser.parse("  z  "), false), result);
		EXPECT_EQ(pkb.getAssignInfo(expr_parser.parse("c"), true), result);
		EXPECT_EQ(pkb.getAssignInfo(expr_parser.parse("c "), false), result);
		EXPECT_EQ(pkb.getAssignInfo(expr_parser.parse(" c"), false), result);
		EXPECT_EQ(pkb.getAssignInfo(expr_parser.parse("  c  "), false), result);
	}

	TEST_F(PKBAdapterTest, getAssignInfoFiltered) {
		ExprParser expr_parser;

		std::vector<var_name> v = { "x", "y", "z", "y", "z", "z" };
		std::vector<std::string> e = { "x", "y*x", "x+y", "z%x", "x-x", "x+y-z" };
		std::vector<stmt_index> s = { 1, 2, 3, 4, 5, 6 };

		std::vector<stmt_index> a;

		for (unsigned int i = 0; i < s.size(); i++) {
			mock_pkb.addStmt(STMT_ASSIGN);
			mock_pkb.addVariable(v[i]);
			a.push_back(s[i]);
			mock_pkb.addModifiesS(s[i], v[i]);
			mock_pkb.addExprTree(s[i], expr_parser.parse(e[i]));
		}

		EXPECT_EQ(pkb.getAssignInfoFiltered(), a);

		std::vector<stmt_index> result = { a[0] };
		EXPECT_EQ(pkb.getAssignInfoFiltered("x"), result);
		result = { a[1], a[3] };
		EXPECT_EQ(pkb.getAssignInfoFiltered("y"), result);
		result = { a[2], a[4], a[5] };
		EXPECT_EQ(pkb.getAssignInfoFiltered("z"), result);

		result = { a[0] };
		EXPECT_EQ(pkb.getAssignInfoFiltered(expr_parser.parse("x"), false), result);
		EXPECT_EQ(pkb.getAssignInfoFiltered(expr_parser.parse("x "), false), result);
		EXPECT_EQ(pkb.getAssignInfoFiltered(expr_parser.parse("x  "), false), result);
		EXPECT_EQ(pkb.getAssignInfoFiltered(expr_parser.parse(" x"), false), result);
		EXPECT_EQ(pkb.getAssignInfoFiltered(expr_parser.parse("  x"), false), result);
		EXPECT_EQ(pkb.getAssignInfoFiltered(expr_parser.parse("  x "), false), result);
		EXPECT_EQ(pkb.getAssignInfoFiltered(expr_parser.parse("  x  "), false), result);
		EXPECT_EQ(pkb.getAssignInfoFiltered("x", expr_parser.parse("x"), false), result);
		result = { };
		EXPECT_EQ(pkb.getAssignInfoFiltered("y", expr_parser.parse("x"), false), result);
		EXPECT_EQ(pkb.getAssignInfoFiltered("z", expr_parser.parse("x"), false), result);

		result = a;
		EXPECT_EQ(pkb.getAssignInfoFiltered(expr_parser.parse("x"), true), result);
		EXPECT_EQ(pkb.getAssignInfoFiltered(expr_parser.parse("x "), true), result);
		EXPECT_EQ(pkb.getAssignInfoFiltered(expr_parser.parse("x  "), true), result);
		EXPECT_EQ(pkb.getAssignInfoFiltered(expr_parser.parse(" x"), true), result);
		EXPECT_EQ(pkb.getAssignInfoFiltered(expr_parser.parse("  x"), true), result);
		EXPECT_EQ(pkb.getAssignInfoFiltered(expr_parser.parse("  x "), true), result);
		EXPECT_EQ(pkb.getAssignInfoFiltered(expr_parser.parse("  x  "), true), result);
		result = { a[0] };
		EXPECT_EQ(pkb.getAssignInfoFiltered("x", expr_parser.parse("x"), true), result);
		result = { a[1], a[3] };
		EXPECT_EQ(pkb.getAssignInfoFiltered("y", expr_parser.parse("x"), true), result);
		result = { a[2], a[4], a[5] };
		EXPECT_EQ(pkb.getAssignInfoFiltered("z", expr_parser.parse("x"), true), result);

		result = { a[1], a[2], a[5] };
		EXPECT_EQ(pkb.getAssignInfoFiltered(expr_parser.parse("y"), true), result);
		EXPECT_EQ(pkb.getAssignInfoFiltered(expr_parser.parse("y "), true), result);
		EXPECT_EQ(pkb.getAssignInfoFiltered(expr_parser.parse(" y"), true), result);
		EXPECT_EQ(pkb.getAssignInfoFiltered(expr_parser.parse("  y  "), true), result);
		result = { };
		EXPECT_EQ(pkb.getAssignInfoFiltered("x", expr_parser.parse("y"), true), result);
		result = { a[1] };
		EXPECT_EQ(pkb.getAssignInfoFiltered("y", expr_parser.parse("y"), true), result);
		result = { a[2], a[5] };
		EXPECT_EQ(pkb.getAssignInfoFiltered("z", expr_parser.parse("y"), true), result);

		result = { a[3], a[5] };
		EXPECT_EQ(pkb.getAssignInfoFiltered(expr_parser.parse("z"), true), result);
		EXPECT_EQ(pkb.getAssignInfoFiltered(expr_parser.parse("z "), true), result);
		EXPECT_EQ(pkb.getAssignInfoFiltered(expr_parser.parse(" z"), true), result);
		EXPECT_EQ(pkb.getAssignInfoFiltered(expr_parser.parse("  z  "), true), result);

		result = { };
		EXPECT_EQ(pkb.getAssignInfoFiltered("x", expr_parser.parse("z"), true), result);
		result = { a[3] };
		EXPECT_EQ(pkb.getAssignInfoFiltered("y", expr_parser.parse("z"), true), result);
		result = { a[5] };
		EXPECT_EQ(pkb.getAssignInfoFiltered("z", expr_parser.parse("z"), true), result);

		result = {};
		EXPECT_EQ(pkb.getAssignInfoFiltered(expr_parser.parse("y"), false), result);
		EXPECT_EQ(pkb.getAssignInfoFiltered(expr_parser.parse("y "), false), result);
		EXPECT_EQ(pkb.getAssignInfoFiltered(expr_parser.parse(" y"), false), result);
		EXPECT_EQ(pkb.getAssignInfoFiltered(expr_parser.parse("  y  "), false), result);
		EXPECT_EQ(pkb.getAssignInfoFiltered(expr_parser.parse("z"), false), result);
		EXPECT_EQ(pkb.getAssignInfoFiltered(expr_parser.parse("z "), false), result);
		EXPECT_EQ(pkb.getAssignInfoFiltered(expr_parser.parse(" z"), false), result);
		EXPECT_EQ(pkb.getAssignInfoFiltered(expr_parser.parse("  z  "), false), result);
		EXPECT_EQ(pkb.getAssignInfoFiltered(expr_parser.parse("c"), true), result);
		EXPECT_EQ(pkb.getAssignInfoFiltered(expr_parser.parse("c "), false), result);
		EXPECT_EQ(pkb.getAssignInfoFiltered(expr_parser.parse(" c"), false), result);
		EXPECT_EQ(pkb.getAssignInfoFiltered(expr_parser.parse("  c  "), false), result);
		EXPECT_EQ(pkb.getAssignInfoFiltered("x", expr_parser.parse("y"), false), result);
		EXPECT_EQ(pkb.getAssignInfoFiltered("x", expr_parser.parse("z"), false), result);
		EXPECT_EQ(pkb.getAssignInfoFiltered("x", expr_parser.parse("c"), true), result);
		EXPECT_EQ(pkb.getAssignInfoFiltered("y", expr_parser.parse("y"), false), result);
		EXPECT_EQ(pkb.getAssignInfoFiltered("y", expr_parser.parse("z"), false), result);
		EXPECT_EQ(pkb.getAssignInfoFiltered("y", expr_parser.parse("c"), true), result);
		EXPECT_EQ(pkb.getAssignInfoFiltered("z", expr_parser.parse("y"), false), result);
		EXPECT_EQ(pkb.getAssignInfoFiltered("z", expr_parser.parse("z"), false), result);
		EXPECT_EQ(pkb.getAssignInfoFiltered("z", expr_parser.parse("c"), true), result);
	}
	TEST_F(PKBAdapterTest, isFollowEmpty) {
		mock_pkb.addStmt(STMT_READ);
		mock_pkb.addStmt(STMT_PRINT);
		mock_pkb.addStmt(STMT_READ);
		mock_pkb.addStmt(STMT_IF);
		EXPECT_TRUE(pkb.isFollowEmpty());
		mock_pkb.addFollows(1, 2);
		EXPECT_FALSE(pkb.isFollowEmpty());
		mock_pkb.addFollows(2, 3);
		EXPECT_FALSE(pkb.isFollowEmpty());
	}

	TEST_F(PKBAdapterTest, isFollow) {
		mock_pkb.addStmt(STMT_READ);
		mock_pkb.addStmt(STMT_PRINT);
		mock_pkb.addStmt(STMT_READ);
		mock_pkb.addStmt(STMT_IF);
		mock_pkb.addFollows(1, 2);
		mock_pkb.addFollows(2, 3);
		EXPECT_TRUE(pkb.isFollow(1, 2));
		EXPECT_TRUE(pkb.isFollow(2, 3));
		EXPECT_FALSE(pkb.isFollow(1, 1));
		EXPECT_FALSE(pkb.isFollow(1, 3));
		EXPECT_FALSE(pkb.isFollow(1, 4));
		EXPECT_FALSE(pkb.isFollow(2, 1));
		EXPECT_FALSE(pkb.isFollow(2, 2));
		EXPECT_FALSE(pkb.isFollow(2, 4));
		EXPECT_FALSE(pkb.isFollow(3, 1));
		EXPECT_FALSE(pkb.isFollow(3, 2));
		EXPECT_FALSE(pkb.isFollow(3, 3));
		EXPECT_FALSE(pkb.isFollow(3, 4));
		EXPECT_FALSE(pkb.isFollow(4, 1));
		EXPECT_FALSE(pkb.isFollow(4, 2));
		EXPECT_FALSE(pkb.isFollow(4, 3));
		EXPECT_FALSE(pkb.isFollow(4, 4));
	}

	TEST_F(PKBAdapterTest, isFollowed) {
		mock_pkb.addStmt(STMT_READ);
		mock_pkb.addStmt(STMT_PRINT);
		mock_pkb.addStmt(STMT_READ);
		mock_pkb.addStmt(STMT_IF);
		mock_pkb.addFollows(1, 2);
		mock_pkb.addFollows(2, 3);
		EXPECT_TRUE(pkb.isFollowed(1));
		EXPECT_TRUE(pkb.isFollowed(2));
		EXPECT_FALSE(pkb.isFollowed(3));
		EXPECT_FALSE(pkb.isFollowed(4));
	}

	TEST_F(PKBAdapterTest, isFollowing) {
		mock_pkb.addStmt(STMT_READ);
		mock_pkb.addStmt(STMT_PRINT);
		mock_pkb.addStmt(STMT_READ);
		mock_pkb.addStmt(STMT_IF);
		mock_pkb.addFollows(1, 2);
		mock_pkb.addFollows(2, 3);
		EXPECT_FALSE(pkb.isFollowing(1));
		EXPECT_TRUE(pkb.isFollowing(2));
		EXPECT_TRUE(pkb.isFollowing(3));
		EXPECT_FALSE(pkb.isFollowing(4));
	}

	TEST_F(PKBAdapterTest, getFollowed) {
		StmtInfo p1{ 1, STMT_READ };
		StmtInfo p2{ 2, STMT_PRINT };
		StmtInfo p3{ 3, STMT_READ };
		StmtInfo p4{ 4, STMT_IF };

		mock_pkb.addStmt(STMT_READ);
		mock_pkb.addStmt(STMT_PRINT);
		mock_pkb.addStmt(STMT_READ);
		mock_pkb.addStmt(STMT_IF);
		mock_pkb.addFollows(1, 2);
		mock_pkb.addFollows(2, 3);

		std::vector<StmtInfo> v1 = { p1, p2 };
		std::vector<StmtInfo> v2 = pkb.getFollowed();
		std::sort(v1.begin(), v1.end());
		std::sort(v2.begin(), v2.end());
		EXPECT_EQ(v1, v2);

		v1 = { };
		v2 = pkb.getFollowed(1);
		EXPECT_EQ(v1, v2);
		v2 = pkb.getFollowed(4);
		EXPECT_EQ(v1, v2);

		v1 = { p1 };
		v2 = pkb.getFollowed(2);
		EXPECT_EQ(v1, v2);

		v1 = { p2 };
		v2 = pkb.getFollowed(3);
		std::sort(v1.begin(), v1.end());
		std::sort(v2.begin(), v2.end());
		EXPECT_EQ(v1, v2);
	}

	TEST_F(PKBAdapterTest, getFollowing) {
		StmtInfo p1{ 1, STMT_READ };
		StmtInfo p2{ 2, STMT_PRINT };
		StmtInfo p3{ 3, STMT_READ };
		StmtInfo p4{ 4, STMT_IF };

		mock_pkb.addStmt(STMT_READ);
		mock_pkb.addStmt(STMT_PRINT);
		mock_pkb.addStmt(STMT_READ);
		mock_pkb.addStmt(STMT_IF);
		mock_pkb.addFollows(1, 2);
		mock_pkb.addFollows(2, 3);

		std::vector<StmtInfo> v1 = { p2, p3 };
		std::vector<StmtInfo> v2 = pkb.getFollowing();
		std::sort(v1.begin(), v1.end());
		std::sort(v2.begin(), v2.end());
		EXPECT_EQ(v1, v2);

		v1 = { p2 };
		v2 = pkb.getFollowing(1);
		std::sort(v1.begin(), v1.end());
		EXPECT_EQ(v1, v2);

		v1 = { p3 };
		v2 = pkb.getFollowing(2);
		EXPECT_EQ(v1, v2);

		v1 = { };
		v2 = pkb.getFollowing(3);
		EXPECT_EQ(v1, v2);
		v2 = pkb.getFollowing(4);
		EXPECT_EQ(v1, v2);
	}

	TEST_F(PKBAdapterTest, getFollows) {
		StmtInfo p1{ 1, STMT_READ };
		StmtInfo p2{ 2, STMT_PRINT };
		StmtInfo p3{ 3, STMT_READ };
		StmtInfo p4{ 4, STMT_IF };

		mock_pkb.addStmt(STMT_READ);
		mock_pkb.addStmt(STMT_PRINT);
		mock_pkb.addStmt(STMT_READ);
		mock_pkb.addStmt(STMT_IF);
		mock_pkb.addFollows(1, 2);
		mock_pkb.addFollows(2, 3);
		std::vector<std::pair<StmtInfo, StmtInfo>> v = { {p1, p2}, {p2, p3} };
		EXPECT_EQ(pkb.getFollows(), v);
	}

	TEST_F(PKBAdapterTest, isFollowTEmpty) {
		mock_pkb.addStmt(STMT_READ);
		mock_pkb.addStmt(STMT_PRINT);
		mock_pkb.addStmt(STMT_READ);
		mock_pkb.addStmt(STMT_IF);
		EXPECT_TRUE(pkb.isFollowTEmpty());
		mock_pkb.addFollowsT(1, 2);
		EXPECT_FALSE(pkb.isFollowTEmpty());
		mock_pkb.addFollowsT(2, 3);
		EXPECT_FALSE(pkb.isFollowTEmpty());
	}

	TEST_F(PKBAdapterTest, isFollowT) {
		mock_pkb.addStmt(STMT_READ);
		mock_pkb.addStmt(STMT_PRINT);
		mock_pkb.addStmt(STMT_READ);
		mock_pkb.addStmt(STMT_IF);
		mock_pkb.addFollowsT(1, 2);
		mock_pkb.addFollowsT(1, 3);
		mock_pkb.addFollowsT(2, 3);

		EXPECT_TRUE(pkb.isFollowT(1, 2));
		EXPECT_TRUE(pkb.isFollowT(1, 3));
		EXPECT_TRUE(pkb.isFollowT(2, 3));
		EXPECT_FALSE(pkb.isFollowT(1, 1));
		EXPECT_FALSE(pkb.isFollowT(1, 4));
		EXPECT_FALSE(pkb.isFollowT(2, 1));
		EXPECT_FALSE(pkb.isFollowT(2, 2));
		EXPECT_FALSE(pkb.isFollowT(2, 4));
		EXPECT_FALSE(pkb.isFollowT(3, 1));
		EXPECT_FALSE(pkb.isFollowT(3, 2));
		EXPECT_FALSE(pkb.isFollowT(3, 3));
		EXPECT_FALSE(pkb.isFollowT(3, 4));
		EXPECT_FALSE(pkb.isFollowT(4, 1));
		EXPECT_FALSE(pkb.isFollowT(4, 2));
		EXPECT_FALSE(pkb.isFollowT(4, 3));
		EXPECT_FALSE(pkb.isFollowT(4, 4));
	}

	TEST_F(PKBAdapterTest, isFollowedT) {
		mock_pkb.addStmt(STMT_READ);
		mock_pkb.addStmt(STMT_PRINT);
		mock_pkb.addStmt(STMT_READ);
		mock_pkb.addStmt(STMT_IF);
		mock_pkb.addFollows(1, 2);
		mock_pkb.addFollows(2, 3);
		mock_pkb.addFollowsT(1, 2);
		mock_pkb.addFollowsT(1, 3);
		mock_pkb.addFollowsT(2, 3);

		EXPECT_TRUE(pkb.isFollowedT(1));
		EXPECT_TRUE(pkb.isFollowedT(2));
		EXPECT_FALSE(pkb.isFollowedT(3));
		EXPECT_FALSE(pkb.isFollowedT(4));
	}
	TEST_F(PKBAdapterTest, isFollowingT) {
		mock_pkb.addStmt(STMT_READ);
		mock_pkb.addStmt(STMT_PRINT);
		mock_pkb.addStmt(STMT_READ);
		mock_pkb.addStmt(STMT_IF);
		mock_pkb.addFollows(1, 2);
		mock_pkb.addFollows(2, 3);
		mock_pkb.addFollowsT(1, 2);
		mock_pkb.addFollowsT(1, 3);
		mock_pkb.addFollowsT(2, 3);

		EXPECT_FALSE(pkb.isFollowingT(1));
		EXPECT_TRUE(pkb.isFollowingT(2));
		EXPECT_TRUE(pkb.isFollowingT(3));
		EXPECT_FALSE(pkb.isFollowingT(4));
	}

	TEST_F(PKBAdapterTest, getFollowedT) {
		StmtInfo p1{ 1, STMT_READ };
		StmtInfo p2{ 2, STMT_PRINT };
		StmtInfo p3{ 3, STMT_READ };
		StmtInfo p4{ 4, STMT_IF };

		mock_pkb.addStmt(STMT_READ);
		mock_pkb.addStmt(STMT_PRINT);
		mock_pkb.addStmt(STMT_READ);
		mock_pkb.addStmt(STMT_IF);
		mock_pkb.addFollows(1, 2);
		mock_pkb.addFollows(2, 3);
		mock_pkb.addFollowsT(1, 2);
		mock_pkb.addFollowsT(1, 3);
		mock_pkb.addFollowsT(2, 3);

		std::vector<StmtInfo> v1 = { p1, p2 };
		std::vector<StmtInfo> v2 = pkb.getFollowedT();
		std::sort(v1.begin(), v1.end());
		std::sort(v2.begin(), v2.end());
		EXPECT_EQ(v1, v2);

		v1 = { };
		v2 = pkb.getFollowedT(1);
		EXPECT_EQ(v1, v2);
		v2 = pkb.getFollowedT(4);
		EXPECT_EQ(v1, v2);

		v1 = { p1 };
		v2 = pkb.getFollowedT(2);
		EXPECT_EQ(v1, v2);

		v1 = { p1, p2 };
		v2 = pkb.getFollowedT(3);
		std::sort(v1.begin(), v1.end());
		std::sort(v2.begin(), v2.end());
		EXPECT_EQ(v1, v2);
	}

	TEST_F(PKBAdapterTest, getFollowingT) {
		StmtInfo p1{ 1, STMT_READ };
		StmtInfo p2{ 2, STMT_PRINT };
		StmtInfo p3{ 3, STMT_READ };
		StmtInfo p4{ 4, STMT_IF };

		mock_pkb.addStmt(STMT_READ);
		mock_pkb.addStmt(STMT_PRINT);
		mock_pkb.addStmt(STMT_READ);
		mock_pkb.addStmt(STMT_IF);
		mock_pkb.addFollows(1, 2);
		mock_pkb.addFollows(2, 3);
		mock_pkb.addFollowsT(1, 2);
		mock_pkb.addFollowsT(1, 3);
		mock_pkb.addFollowsT(2, 3);

		std::vector<StmtInfo> v1 = { p2, p3 };
		std::vector<StmtInfo> v2 = pkb.getFollowingT();
		std::sort(v1.begin(), v1.end());
		std::sort(v2.begin(), v2.end());
		EXPECT_EQ(v1, v2);

		v1 = { p2, p3 };
		v2 = pkb.getFollowingT(1);
		std::sort(v1.begin(), v1.end());
		EXPECT_EQ(v1, v2);

		v1 = { p3 };
		v2 = pkb.getFollowingT(2);
		EXPECT_EQ(v1, v2);

		v1 = { };
		v2 = pkb.getFollowingT(3);
		EXPECT_EQ(v1, v2);
		v2 = pkb.getFollowingT(4);
		EXPECT_EQ(v1, v2);
	}

	TEST_F(PKBAdapterTest, getFollowsT) {
		StmtInfo p1{ 1, STMT_READ };
		StmtInfo p2{ 2, STMT_PRINT };
		StmtInfo p3{ 3, STMT_READ };
		StmtInfo p4{ 4, STMT_IF };

		mock_pkb.addStmt(STMT_READ);
		mock_pkb.addStmt(STMT_PRINT);
		mock_pkb.addStmt(STMT_READ);
		mock_pkb.addStmt(STMT_IF);
		mock_pkb.addFollows(1, 2);
		mock_pkb.addFollows(2, 3);
		mock_pkb.addFollowsT(1, 2);
		mock_pkb.addFollowsT(1, 3);
		mock_pkb.addFollowsT(2, 3);

		std::vector<std::pair<StmtInfo, StmtInfo>> v1 = { {p1, p2}, {p1, p3}, {p2, p3} };
		std::vector<std::pair<StmtInfo, StmtInfo>> v2 = pkb.getFollowsT();
		std::sort(v2.begin(), v2.end());
		EXPECT_EQ(v1, v2);
	}

	TEST_F(PKBAdapterTest, isParentEmpty) {
		mock_pkb.addStmt(STMT_IF);
		mock_pkb.addStmt(STMT_WHILE);
		mock_pkb.addStmt(STMT_READ);
		mock_pkb.addStmt(STMT_IF);
		EXPECT_TRUE(pkb.isParentEmpty());
		mock_pkb.addParent(1, 2);
		EXPECT_FALSE(pkb.isParentEmpty());
		mock_pkb.addParent(2, 3);
		EXPECT_FALSE(pkb.isParentEmpty());
	}
	TEST_F(PKBAdapterTest, isParent) {
		mock_pkb.addStmt(STMT_IF);
		mock_pkb.addStmt(STMT_WHILE);
		mock_pkb.addStmt(STMT_IF);
		mock_pkb.addStmt(STMT_IF);
		mock_pkb.addStmt(STMT_WHILE);
		mock_pkb.addParent(1, 2);
		mock_pkb.addParent(1, 3);
		mock_pkb.addParent(2, 3);
		mock_pkb.addParent(3, 4);
		EXPECT_TRUE(pkb.isParent(1, 2));
		EXPECT_TRUE(pkb.isParent(1, 3));
		EXPECT_TRUE(pkb.isParent(2, 3));
		EXPECT_TRUE(pkb.isParent(3, 4));
		EXPECT_FALSE(pkb.isParent(1, 1));
		EXPECT_FALSE(pkb.isParent(1, 4));
		EXPECT_FALSE(pkb.isParent(1, 5));
		EXPECT_FALSE(pkb.isParent(2, 1));
		EXPECT_FALSE(pkb.isParent(2, 2));
		EXPECT_FALSE(pkb.isParent(2, 4));
		EXPECT_FALSE(pkb.isParent(2, 5));
		EXPECT_FALSE(pkb.isParent(3, 1));
		EXPECT_FALSE(pkb.isParent(3, 2));
		EXPECT_FALSE(pkb.isParent(3, 3));
		EXPECT_FALSE(pkb.isParent(3, 5));
		EXPECT_FALSE(pkb.isParent(4, 1));
		EXPECT_FALSE(pkb.isParent(4, 2));
		EXPECT_FALSE(pkb.isParent(4, 3));
		EXPECT_FALSE(pkb.isParent(4, 4));
		EXPECT_FALSE(pkb.isParent(4, 5));
		EXPECT_FALSE(pkb.isParent(5, 1));
		EXPECT_FALSE(pkb.isParent(5, 2));
		EXPECT_FALSE(pkb.isParent(5, 3));
		EXPECT_FALSE(pkb.isParent(5, 4));
		EXPECT_FALSE(pkb.isParent(5, 5));

		EXPECT_TRUE(pkb.isParent(1));
		EXPECT_TRUE(pkb.isParent(2));
		EXPECT_TRUE(pkb.isParent(3));
		EXPECT_FALSE(pkb.isParent(4));
		EXPECT_FALSE(pkb.isParent(5));
	}

	TEST_F(PKBAdapterTest, isChild) {
		mock_pkb.addStmt(STMT_IF);
		mock_pkb.addStmt(STMT_WHILE);
		mock_pkb.addStmt(STMT_IF);
		mock_pkb.addStmt(STMT_IF);
		mock_pkb.addStmt(STMT_WHILE);
		mock_pkb.addParent(1, 2);
		mock_pkb.addParent(1, 3);
		mock_pkb.addParent(2, 3);
		mock_pkb.addParent(3, 4);
		EXPECT_FALSE(pkb.isChild(1));
		EXPECT_TRUE(pkb.isChild(2));
		EXPECT_TRUE(pkb.isChild(3));
		EXPECT_TRUE(pkb.isChild(4));
		EXPECT_FALSE(pkb.isChild(5));
	}

	TEST_F(PKBAdapterTest, getChild) {
		StmtInfo p1{ 1, STMT_IF };
		StmtInfo p2{ 2, STMT_WHILE };
		StmtInfo p3{ 3, STMT_IF };
		StmtInfo p4{ 4, STMT_IF };
		StmtInfo p5{ 5, STMT_WHILE };

		mock_pkb.addStmt(STMT_IF);
		mock_pkb.addStmt(STMT_WHILE);
		mock_pkb.addStmt(STMT_IF);
		mock_pkb.addStmt(STMT_IF);
		mock_pkb.addStmt(STMT_WHILE);
		mock_pkb.addParent(1, 2);
		mock_pkb.addParent(1, 3);
		mock_pkb.addParent(2, 3);
		mock_pkb.addParent(3, 4);

		std::vector<StmtInfo> v1 = { p2, p3, p4 };
		std::vector<StmtInfo> v2 = pkb.getChild();
		std::sort(v1.begin(), v1.end());
		std::sort(v2.begin(), v2.end());
		EXPECT_EQ(v1, v2);

		v1 = { p2, p3 };
		v2 = pkb.getChild(1);
		std::sort(v1.begin(), v1.end());
		std::sort(v2.begin(), v2.end());
		EXPECT_EQ(v1, v2);

		v1 = { p3 };
		v2 = pkb.getChild(2);
		EXPECT_EQ(v1, v2);

		v1 = { p4 };
		v2 = pkb.getChild(3);
		EXPECT_EQ(v1, v2);

		v1 = { };
		v2 = pkb.getChild(4);
		EXPECT_EQ(v1, v2);
		v2 = pkb.getChild(5);
		EXPECT_EQ(v1, v2);
	}

	TEST_F(PKBAdapterTest, getParent) {
		StmtInfo p1{ 1, STMT_IF };
		StmtInfo p2{ 2, STMT_WHILE };
		StmtInfo p3{ 3, STMT_IF };
		StmtInfo p4{ 4, STMT_IF };
		StmtInfo p5{ 5, STMT_WHILE };

		mock_pkb.addStmt(STMT_IF);
		mock_pkb.addStmt(STMT_WHILE);
		mock_pkb.addStmt(STMT_IF);
		mock_pkb.addStmt(STMT_IF);
		mock_pkb.addStmt(STMT_WHILE);
		mock_pkb.addParent(1, 2);
		mock_pkb.addParent(1, 3);
		mock_pkb.addParent(2, 3);
		mock_pkb.addParent(3, 4);

		std::vector<StmtInfo> v1 = { p1, p2, p3 };
		std::vector<StmtInfo> v2 = pkb.getParent();
		std::sort(v1.begin(), v1.end());
		std::sort(v2.begin(), v2.end());
		EXPECT_EQ(v1, v2);

		v1 = { p1 };
		v2 = pkb.getParent(2);
		EXPECT_EQ(v1, v2);

		v1 = { p1, p2 };
		v2 = pkb.getParent(3);
		std::sort(v1.begin(), v1.end());
		std::sort(v2.begin(), v2.end());
		EXPECT_EQ(v1, v2);

		v1 = { p3 };
		v2 = pkb.getParent(4);
		EXPECT_EQ(v1, v2);

		v1 = { };
		v2 = pkb.getParent(1);
		EXPECT_EQ(v1, v2);
		v2 = pkb.getParent(5);
		EXPECT_EQ(v1, v2);
	}

	TEST_F(PKBAdapterTest, getAllParentRelation) {
		StmtInfo p1{ 1, STMT_IF };
		StmtInfo p2{ 2, STMT_WHILE };
		StmtInfo p3{ 3, STMT_IF };
		StmtInfo p4{ 4, STMT_IF };
		StmtInfo p5{ 5, STMT_WHILE };

		mock_pkb.addStmt(STMT_IF);
		mock_pkb.addStmt(STMT_WHILE);
		mock_pkb.addStmt(STMT_IF);
		mock_pkb.addStmt(STMT_IF);
		mock_pkb.addStmt(STMT_WHILE);
		mock_pkb.addParent(1, 2);
		mock_pkb.addParent(1, 3);
		mock_pkb.addParent(2, 3);
		mock_pkb.addParent(3, 4);

		std::vector<std::pair<StmtInfo, StmtInfo>> v = { {p1, p2}, {p1, p3}, {p2, p3}, {p3, p4} };
		EXPECT_EQ(pkb.getAllParentRelation(), v);
	}

	TEST_F(PKBAdapterTest, isParentT) {
		mock_pkb.addStmt(STMT_IF);
		mock_pkb.addStmt(STMT_WHILE);
		mock_pkb.addStmt(STMT_IF);
		mock_pkb.addStmt(STMT_IF);
		mock_pkb.addStmt(STMT_WHILE);
		mock_pkb.addParent(1, 2);
		mock_pkb.addParent(1, 3);
		mock_pkb.addParent(2, 3);
		mock_pkb.addParent(3, 4);
		mock_pkb.addParentT(1, 2);
		mock_pkb.addParentT(1, 3);
		mock_pkb.addParentT(1, 4);
		mock_pkb.addParentT(2, 3);
		mock_pkb.addParentT(2, 4);
		mock_pkb.addParentT(3, 4);

		EXPECT_TRUE(pkb.isParentT(1, 2));
		EXPECT_TRUE(pkb.isParentT(1, 3));
		EXPECT_TRUE(pkb.isParentT(1, 4));
		EXPECT_TRUE(pkb.isParentT(2, 3));
		EXPECT_TRUE(pkb.isParentT(2, 4));
		EXPECT_TRUE(pkb.isParentT(3, 4));
		EXPECT_FALSE(pkb.isParentT(1, 1));
		EXPECT_FALSE(pkb.isParentT(1, 5));
		EXPECT_FALSE(pkb.isParentT(2, 1));
		EXPECT_FALSE(pkb.isParentT(2, 2));
		EXPECT_FALSE(pkb.isParentT(2, 5));
		EXPECT_FALSE(pkb.isParentT(3, 1));
		EXPECT_FALSE(pkb.isParentT(3, 2));
		EXPECT_FALSE(pkb.isParentT(3, 3));
		EXPECT_FALSE(pkb.isParentT(3, 5));
		EXPECT_FALSE(pkb.isParentT(4, 1));
		EXPECT_FALSE(pkb.isParentT(4, 2));
		EXPECT_FALSE(pkb.isParentT(4, 3));
		EXPECT_FALSE(pkb.isParentT(4, 4));
		EXPECT_FALSE(pkb.isParentT(4, 5));
		EXPECT_FALSE(pkb.isParentT(5, 1));
		EXPECT_FALSE(pkb.isParentT(5, 2));
		EXPECT_FALSE(pkb.isParentT(5, 3));
		EXPECT_FALSE(pkb.isParentT(5, 4));
		EXPECT_FALSE(pkb.isParentT(5, 5));

		EXPECT_TRUE(pkb.isParentT(1));
		EXPECT_TRUE(pkb.isParentT(2));
		EXPECT_TRUE(pkb.isParentT(3));
		EXPECT_FALSE(pkb.isParentT(4));
		EXPECT_FALSE(pkb.isParentT(5));
	}

	TEST_F(PKBAdapterTest, isChildT) {
		mock_pkb.addStmt(STMT_IF);
		mock_pkb.addStmt(STMT_WHILE);
		mock_pkb.addStmt(STMT_IF);
		mock_pkb.addStmt(STMT_IF);
		mock_pkb.addStmt(STMT_WHILE);
		mock_pkb.addParent(1, 2);
		mock_pkb.addParent(1, 3);
		mock_pkb.addParent(2, 3);
		mock_pkb.addParent(3, 4);
		mock_pkb.addParentT(1, 2);
		mock_pkb.addParentT(1, 3);
		mock_pkb.addParentT(1, 4);
		mock_pkb.addParentT(2, 3);
		mock_pkb.addParentT(2, 4);
		mock_pkb.addParentT(3, 4);

		EXPECT_FALSE(pkb.isChildT(1));
		EXPECT_TRUE(pkb.isChildT(2));
		EXPECT_TRUE(pkb.isChildT(3));
		EXPECT_TRUE(pkb.isChildT(4));
		EXPECT_FALSE(pkb.isChildT(5));
	}

	TEST_F(PKBAdapterTest, getChildT) {
		StmtInfo p1{ 1, STMT_IF };
		StmtInfo p2{ 2, STMT_WHILE };
		StmtInfo p3{ 3, STMT_IF };
		StmtInfo p4{ 4, STMT_IF };
		StmtInfo p5{ 5, STMT_WHILE };

		mock_pkb.addStmt(STMT_IF);
		mock_pkb.addStmt(STMT_WHILE);
		mock_pkb.addStmt(STMT_IF);
		mock_pkb.addStmt(STMT_IF);
		mock_pkb.addStmt(STMT_WHILE);
		mock_pkb.addParent(1, 2);
		mock_pkb.addParent(1, 3);
		mock_pkb.addParent(2, 3);
		mock_pkb.addParent(3, 4);
		mock_pkb.addParentT(1, 2);
		mock_pkb.addParentT(1, 3);
		mock_pkb.addParentT(1, 4);
		mock_pkb.addParentT(2, 3);
		mock_pkb.addParentT(2, 4);
		mock_pkb.addParentT(3, 4);

		std::vector<StmtInfo> v1 = { p2, p3, p4 };
		std::vector<StmtInfo> v2 = pkb.getChildT();
		std::sort(v1.begin(), v1.end());
		std::sort(v2.begin(), v2.end());
		EXPECT_EQ(v1, v2);

		v2 = pkb.getChildT(1);
		std::sort(v2.begin(), v2.end());
		EXPECT_EQ(v1, v2);

		v1 = { p3, p4 };
		v2 = pkb.getChildT(2);
		std::sort(v1.begin(), v1.end());
		std::sort(v2.begin(), v2.end());
		EXPECT_EQ(v1, v2);

		v1 = { p4 };
		v2 = pkb.getChildT(3);
		EXPECT_EQ(v1, v2);

		v1 = { };
		v2 = pkb.getChildT(4);
		EXPECT_EQ(v1, v2);
		v2 = pkb.getChildT(5);
		EXPECT_EQ(v1, v2);
	}

	TEST_F(PKBAdapterTest, getParentT) {
		StmtInfo p1{ 1, STMT_IF };
		StmtInfo p2{ 2, STMT_WHILE };
		StmtInfo p3{ 3, STMT_IF };
		StmtInfo p4{ 4, STMT_IF };
		StmtInfo p5{ 5, STMT_WHILE };

		mock_pkb.addStmt(STMT_IF);
		mock_pkb.addStmt(STMT_WHILE);
		mock_pkb.addStmt(STMT_IF);
		mock_pkb.addStmt(STMT_IF);
		mock_pkb.addStmt(STMT_WHILE);
		mock_pkb.addParent(1, 2);
		mock_pkb.addParent(1, 3);
		mock_pkb.addParent(2, 3);
		mock_pkb.addParent(3, 4);
		mock_pkb.addParentT(1, 2);
		mock_pkb.addParentT(1, 3);
		mock_pkb.addParentT(1, 4);
		mock_pkb.addParentT(2, 3);
		mock_pkb.addParentT(2, 4);
		mock_pkb.addParentT(3, 4);

		std::vector<StmtInfo> v1 = { p1, p2, p3 };
		std::vector<StmtInfo> v2 = pkb.getParentT();
		std::sort(v1.begin(), v1.end());
		std::sort(v2.begin(), v2.end());
		EXPECT_EQ(v1, v2);

		v2 = pkb.getParentT(4);
		std::sort(v2.begin(), v2.end());
		EXPECT_EQ(v1, v2);

		v1 = { p1 };
		v2 = pkb.getParentT(2);
		EXPECT_EQ(v1, v2);

		v1 = { p1, p2 };
		v2 = pkb.getParentT(3);
		std::sort(v1.begin(), v1.end());
		std::sort(v2.begin(), v2.end());
		EXPECT_EQ(v1, v2);

		v1 = { };
		v2 = pkb.getParentT(1);
		EXPECT_EQ(v1, v2);
		v2 = pkb.getParentT(5);
		EXPECT_EQ(v1, v2);
	}

	TEST_F(PKBAdapterTest, getAllParentTRelation) {
		StmtInfo p1{ 1, STMT_IF };
		StmtInfo p2{ 2, STMT_WHILE };
		StmtInfo p3{ 3, STMT_IF };
		StmtInfo p4{ 4, STMT_IF };
		StmtInfo p5{ 5, STMT_WHILE };

		mock_pkb.addStmt(STMT_IF);
		mock_pkb.addStmt(STMT_WHILE);
		mock_pkb.addStmt(STMT_IF);
		mock_pkb.addStmt(STMT_IF);
		mock_pkb.addStmt(STMT_WHILE);
		mock_pkb.addParent(1, 2);
		mock_pkb.addParent(1, 3);
		mock_pkb.addParent(2, 3);
		mock_pkb.addParent(3, 4);
		mock_pkb.addParentT(1, 2);
		mock_pkb.addParentT(1, 3);
		mock_pkb.addParentT(1, 4);
		mock_pkb.addParentT(2, 3);
		mock_pkb.addParentT(2, 4);
		mock_pkb.addParentT(3, 4);

		std::vector<std::pair<StmtInfo, StmtInfo>> v = { {p1, p2}, {p1, p3}, {p1, p4}, {p2, p3}, {p2, p4}, {p3, p4} };
		EXPECT_EQ(pkb.getAllParentTRelation(), v);
	}
	TEST_F(PKBAdapterTest, isModifiesP) {
		proc_name main1 = "main1";
		proc_name sub1 = "sub1";
		proc_name main2 = "main2";
		proc_name sub2 = "sub2";
		var_name x = "x";
		var_name y = "y";
		var_name z = "z";

		mock_pkb.addProcedure(main1);
		mock_pkb.addProcedure(main2);
		mock_pkb.addProcedure(sub1);
		mock_pkb.addProcedure(sub2);
		mock_pkb.addVariable(x);
		mock_pkb.addModifiesP(main1, x);
		mock_pkb.addVariable(y);
		mock_pkb.addModifiesP(main2, y);

		EXPECT_TRUE(pkb.isModifiesP(main1, x));
		EXPECT_FALSE(pkb.isModifiesP(sub1, x));
		EXPECT_FALSE(pkb.isModifiesP(main2, x));
		EXPECT_FALSE(pkb.isModifiesP(sub2, x));
		EXPECT_FALSE(pkb.isModifiesP(main1, y));
		EXPECT_FALSE(pkb.isModifiesP(sub1, y));
		EXPECT_TRUE(pkb.isModifiesP(main2, y));
		EXPECT_FALSE(pkb.isModifiesP(sub2, y));

		EXPECT_TRUE(pkb.isModifiesP(main1));
		EXPECT_FALSE(pkb.isModifiesP(sub1));
		EXPECT_TRUE(pkb.isModifiesP(main2));
		EXPECT_FALSE(pkb.isModifiesP(sub2));
	}
	TEST_F(PKBAdapterTest, getModifiesPRelation) {
		proc_name main1 = "main1";
		proc_name sub1 = "sub1";
		proc_name main2 = "main2";
		proc_name sub2 = "sub2";
		var_name x = "x";
		var_name y = "y";
		var_name z = "z";

		mock_pkb.addProcedure(main1);
		mock_pkb.addProcedure(main2);
		mock_pkb.addProcedure(sub1);
		mock_pkb.addProcedure(sub2);
		mock_pkb.addVariable(x);
		mock_pkb.addModifiesP(main1, x);
		mock_pkb.addVariable(y);
		mock_pkb.addModifiesP(main2, y);

		std::vector<std::pair<proc_name, var_name>> v = { {main1, x}, {main2, y} };
		EXPECT_EQ(pkb.getModifiesPRelation(), v);
	}
	TEST_F(PKBAdapterTest, getModifiesP) {
		proc_name main1 = "main1";
		proc_name sub1 = "sub1";
		proc_name main2 = "main2";
		proc_name sub2 = "sub2";
		var_name x = "x";
		var_name y = "y";
		var_name z = "z";

		mock_pkb.addProcedure(main1);
		mock_pkb.addProcedure(main2);
		mock_pkb.addProcedure(sub1);
		mock_pkb.addProcedure(sub2);
		mock_pkb.addVariable(x);
		mock_pkb.addModifiesP(main1, x);
		mock_pkb.addVariable(y);
		mock_pkb.addModifiesP(sub1, y);
		mock_pkb.addModifiesP(main2, y);

		std::vector<proc_name> v1 = { main1, sub1, main2 };
		std::vector<proc_name> v2 = pkb.getModifiesP();
		std::sort(v1.begin(), v1.end());
		std::sort(v2.begin(), v2.end());
		EXPECT_EQ(v1, v2);

		v1 = { main1 };
		v2 = pkb.getModifiesP(x);
		std::sort(v2.begin(), v2.end());
		EXPECT_EQ(v1, v2);

		v1 = { sub1, main2 };
		v2 = pkb.getModifiesP(y);
		std::sort(v1.begin(), v1.end());
		std::sort(v2.begin(), v2.end());
		EXPECT_EQ(v1, v2);

		v1 = { };
		v2 = pkb.getModifiesP(z);
		EXPECT_EQ(v1, v2);
	}

	TEST_F(PKBAdapterTest, getModifiedP) {
		proc_name main1 = "main1";
		proc_name sub1 = "sub1";
		proc_name main2 = "main2";
		proc_name sub2 = "sub2";
		var_name x = "x";
		var_name y = "y";
		var_name z = "z";

		mock_pkb.addProcedure(main1);
		mock_pkb.addProcedure(main2);
		mock_pkb.addProcedure(sub1);
		mock_pkb.addProcedure(sub2);
		mock_pkb.addVariable(x);
		mock_pkb.addModifiesP(main1, x);
		mock_pkb.addVariable(y);
		mock_pkb.addModifiesP(main2, x);
		mock_pkb.addModifiesP(main2, y);

		std::vector<var_name> v1 = { x };
		std::vector<var_name> v2 = pkb.getModifiedP(main1);
		std::sort(v2.begin(), v2.end());
		EXPECT_EQ(v1, v2);

		v1 = { x, y };
		v2 = pkb.getModifiedP(main2);
		std::sort(v1.begin(), v1.end());
		std::sort(v2.begin(), v2.end());
		EXPECT_EQ(v1, v2);

		v1 = { };
		v2 = pkb.getModifiedP(sub1);
		EXPECT_EQ(v1, v2);
		v2 = pkb.getModifiedP(sub2);
		EXPECT_EQ(v1, v2);
	}
	TEST_F(PKBAdapterTest, isModifiesS) {
		StmtInfo p1{ 1, STMT_READ };
		StmtInfo p2{ 2, STMT_PRINT };
		StmtInfo p3{ 3, STMT_READ };
		StmtInfo p4{ 4, STMT_PRINT };
		var_name x = "x";
		var_name y = "y";
		var_name z = "z";

		mock_pkb.addStmt(STMT_READ);
		mock_pkb.addStmt(STMT_PRINT);
		mock_pkb.addStmt(STMT_READ);
		mock_pkb.addStmt(STMT_PRINT);
		mock_pkb.addVariable(x);
		mock_pkb.addModifiesS(1, x);
		mock_pkb.addVariable(y);
		mock_pkb.addModifiesS(3, y);

		EXPECT_TRUE(pkb.isModifiesS(1, x));
		EXPECT_FALSE(pkb.isModifiesS(2, x));
		EXPECT_FALSE(pkb.isModifiesS(3, x));
		EXPECT_FALSE(pkb.isModifiesS(4, x));
		EXPECT_FALSE(pkb.isModifiesS(1, y));
		EXPECT_FALSE(pkb.isModifiesS(2, y));
		EXPECT_TRUE(pkb.isModifiesS(3, y));
		EXPECT_FALSE(pkb.isModifiesS(4, y));

		EXPECT_TRUE(pkb.isModifiesS(1));
		EXPECT_FALSE(pkb.isModifiesS(2));
		EXPECT_TRUE(pkb.isModifiesS(3));
		EXPECT_FALSE(pkb.isModifiesS(4));
	}
	TEST_F(PKBAdapterTest, getModifiesSRelation) {
		StmtInfo p1{ 1, STMT_READ };
		StmtInfo p2{ 2, STMT_PRINT };
		StmtInfo p3{ 3, STMT_READ };
		StmtInfo p4{ 4, STMT_PRINT };
		var_name x = "x";
		var_name y = "y";
		var_name z = "z";

		mock_pkb.addStmt(STMT_READ);
		mock_pkb.addStmt(STMT_PRINT);
		mock_pkb.addStmt(STMT_READ);
		mock_pkb.addStmt(STMT_PRINT);
		mock_pkb.addVariable(x);
		mock_pkb.addModifiesS(1, x);
		mock_pkb.addVariable(y);
		mock_pkb.addModifiesS(3, y);

		std::vector<std::pair<StmtInfo, var_name>> v = { {p1, x}, {p3, y} };
		EXPECT_EQ(pkb.getModifiesSRelation(), v);
	}
	TEST_F(PKBAdapterTest, getModifiesS) {
		StmtInfo p1{ 1, STMT_READ };
		StmtInfo p2{ 2, STMT_PRINT };
		StmtInfo p3{ 3, STMT_READ };
		StmtInfo p4{ 4, STMT_PRINT };
		var_name x = "x";
		var_name y = "y";
		var_name z = "z";

		mock_pkb.addStmt(STMT_READ);
		mock_pkb.addStmt(STMT_PRINT);
		mock_pkb.addStmt(STMT_READ);
		mock_pkb.addStmt(STMT_PRINT);
		mock_pkb.addVariable(x);
		mock_pkb.addModifiesS(1, x);
		mock_pkb.addVariable(y);
		mock_pkb.addModifiesS(2, y);
		mock_pkb.addModifiesS(3, y);

		std::vector<StmtInfo> v1 = { p1, p2, p3 };
		std::vector<StmtInfo> v2 = pkb.getModifiesS();
		std::sort(v1.begin(), v1.end());
		std::sort(v2.begin(), v2.end());
		EXPECT_EQ(v1, v2);

		v1 = { p1 };
		v2 = pkb.getModifiesS(x);
		std::sort(v2.begin(), v2.end());
		EXPECT_EQ(v1, v2);

		v1 = { p2, p3 };
		v2 = pkb.getModifiesS(y);
		std::sort(v1.begin(), v1.end());
		std::sort(v2.begin(), v2.end());
		EXPECT_EQ(v1, v2);

		v1 = { };
		v2 = pkb.getModifiesS(z);
		EXPECT_EQ(v1, v2);
	}
	TEST_F(PKBAdapterTest, getModifiedS) {
		StmtInfo p1{ 1, STMT_READ };
		StmtInfo p2{ 2, STMT_PRINT };
		StmtInfo p3{ 3, STMT_READ };
		StmtInfo p4{ 4, STMT_PRINT };
		var_name x = "x";
		var_name y = "y";
		var_name z = "z";

		mock_pkb.addStmt(STMT_READ);
		mock_pkb.addStmt(STMT_PRINT);
		mock_pkb.addStmt(STMT_READ);
		mock_pkb.addStmt(STMT_PRINT);
		mock_pkb.addVariable(x);
		mock_pkb.addModifiesS(1, x);
		mock_pkb.addVariable(y);
		mock_pkb.addModifiesS(2, x);
		mock_pkb.addModifiesS(2, y);

		std::vector<var_name> v1 = { x };
		std::vector<var_name> v2 = pkb.getModifiedS(1);
		std::sort(v2.begin(), v2.end());
		EXPECT_EQ(v1, v2);

		v1 = { x, y };
		v2 = pkb.getModifiedS(2);
		std::sort(v1.begin(), v1.end());
		std::sort(v2.begin(), v2.end());
		EXPECT_EQ(v1, v2);

		v1 = { };
		v2 = pkb.getModifiedS(3);
		EXPECT_EQ(v1, v2);
		v2 = pkb.getModifiedS(4);
		EXPECT_EQ(v1, v2);
	}
	TEST_F(PKBAdapterTest, isUsesP) {
		proc_name main1 = "main1";
		proc_name sub1 = "sub1";
		proc_name main2 = "main2";
		proc_name sub2 = "sub2";
		var_name x = "x";
		var_name y = "y";
		var_name z = "z";

		mock_pkb.addProcedure(main1);
		mock_pkb.addProcedure(main2);
		mock_pkb.addProcedure(sub1);
		mock_pkb.addProcedure(sub2);
		mock_pkb.addVariable(x);
		mock_pkb.addUsesP(main1, x);
		mock_pkb.addVariable(y);
		mock_pkb.addUsesP(main2, y);

		EXPECT_TRUE(pkb.isUsesP(main1, x));
		EXPECT_FALSE(pkb.isUsesP(sub1, x));
		EXPECT_FALSE(pkb.isUsesP(main2, x));
		EXPECT_FALSE(pkb.isUsesP(sub2, x));
		EXPECT_FALSE(pkb.isUsesP(main1, y));
		EXPECT_FALSE(pkb.isUsesP(sub1, y));
		EXPECT_TRUE(pkb.isUsesP(main2, y));
		EXPECT_FALSE(pkb.isUsesP(sub2, y));

		EXPECT_TRUE(pkb.isUsesP(main1));
		EXPECT_FALSE(pkb.isUsesP(sub1));
		EXPECT_TRUE(pkb.isUsesP(main2));
		EXPECT_FALSE(pkb.isUsesP(sub2));
	}
	TEST_F(PKBAdapterTest, getUsesPRelation) {
		proc_name main1 = "main1";
		proc_name sub1 = "sub1";
		proc_name main2 = "main2";
		proc_name sub2 = "sub2";
		var_name x = "x";
		var_name y = "y";
		var_name z = "z";

		mock_pkb.addProcedure(main1);
		mock_pkb.addProcedure(main2);
		mock_pkb.addProcedure(sub1);
		mock_pkb.addProcedure(sub2);
		mock_pkb.addVariable(x);
		mock_pkb.addUsesP(main1, x);
		mock_pkb.addVariable(y);
		mock_pkb.addUsesP(main2, y);

		std::vector<std::pair<proc_name, var_name>> v = { {main1, x}, {main2, y} };
		EXPECT_EQ(pkb.getUsesPRelation(), v);
	}

	TEST_F(PKBAdapterTest, getUsesP) {
		proc_name main1 = "main1";
		proc_name sub1 = "sub1";
		proc_name main2 = "main2";
		proc_name sub2 = "sub2";
		var_name x = "x";
		var_name y = "y";
		var_name z = "z";

		mock_pkb.addProcedure(main1);
		mock_pkb.addProcedure(main2);
		mock_pkb.addProcedure(sub1);
		mock_pkb.addProcedure(sub2);
		mock_pkb.addVariable(x);
		mock_pkb.addUsesP(main1, x);
		mock_pkb.addVariable(y);
		mock_pkb.addUsesP(sub1, y);
		mock_pkb.addUsesP(main2, y);

		std::vector<proc_name> v1 = { main1, sub1, main2 };
		std::vector<proc_name> v2 = pkb.getUsesP();
		std::sort(v1.begin(), v1.end());
		std::sort(v2.begin(), v2.end());
		EXPECT_EQ(v1, v2);

		v1 = { main1 };
		v2 = pkb.getUsesP(x);
		std::sort(v2.begin(), v2.end());
		EXPECT_EQ(v1, v2);

		v1 = { sub1, main2 };
		v2 = pkb.getUsesP(y);
		std::sort(v1.begin(), v1.end());
		std::sort(v2.begin(), v2.end());
		EXPECT_EQ(v1, v2);

		v1 = { };
		v2 = pkb.getUsesP(z);
		EXPECT_EQ(v1, v2);
	}

	TEST_F(PKBAdapterTest, getUsedP) {
		proc_name main1 = "main1";
		proc_name sub1 = "sub1";
		proc_name main2 = "main2";
		proc_name sub2 = "sub2";
		var_name x = "x";
		var_name y = "y";
		var_name z = "z";

		mock_pkb.addProcedure(main1);
		mock_pkb.addProcedure(main2);
		mock_pkb.addProcedure(sub1);
		mock_pkb.addProcedure(sub2);
		mock_pkb.addVariable(x);
		mock_pkb.addUsesP(main1, x);
		mock_pkb.addVariable(y);
		mock_pkb.addUsesP(main2, x);
		mock_pkb.addUsesP(main2, y);

		std::vector<var_name> v1 = { x };
		std::vector<var_name> v2 = pkb.getUsedP(main1);
		std::sort(v2.begin(), v2.end());
		EXPECT_EQ(v1, v2);

		v1 = { x, y };
		v2 = pkb.getUsedP(main2);
		std::sort(v1.begin(), v1.end());
		std::sort(v2.begin(), v2.end());
		EXPECT_EQ(v1, v2);

		v1 = { };
		v2 = pkb.getUsedP(sub1);
		EXPECT_EQ(v1, v2);
		v2 = pkb.getUsedP(sub2);
		EXPECT_EQ(v1, v2);
	}
	TEST_F(PKBAdapterTest, isUsesS) {
		StmtInfo p1{ 1, STMT_READ };
		StmtInfo p2{ 2, STMT_PRINT };
		StmtInfo p3{ 3, STMT_READ };
		StmtInfo p4{ 4, STMT_PRINT };
		var_name x = "x";
		var_name y = "y";
		var_name z = "z";

		mock_pkb.addStmt(STMT_READ);
		mock_pkb.addStmt(STMT_PRINT);
		mock_pkb.addStmt(STMT_READ);
		mock_pkb.addStmt(STMT_PRINT);
		mock_pkb.addVariable(x);
		mock_pkb.addUsesS(1, x);
		mock_pkb.addVariable(y);
		mock_pkb.addUsesS(3, y);

		EXPECT_TRUE(pkb.isUsesS(1, x));
		EXPECT_FALSE(pkb.isUsesS(2, x));
		EXPECT_FALSE(pkb.isUsesS(3, x));
		EXPECT_FALSE(pkb.isUsesS(4, x));
		EXPECT_FALSE(pkb.isUsesS(1, y));
		EXPECT_FALSE(pkb.isUsesS(2, y));
		EXPECT_TRUE(pkb.isUsesS(3, y));
		EXPECT_FALSE(pkb.isUsesS(4, y));

		EXPECT_TRUE(pkb.isUsesS(1));
		EXPECT_FALSE(pkb.isUsesS(2));
		EXPECT_TRUE(pkb.isUsesS(3));
		EXPECT_FALSE(pkb.isUsesS(4));
	}

	TEST_F(PKBAdapterTest, getUsesSRelation) {
		StmtInfo p1{ 1, STMT_READ };
		StmtInfo p2{ 2, STMT_PRINT };
		StmtInfo p3{ 3, STMT_READ };
		StmtInfo p4{ 4, STMT_PRINT };
		var_name x = "x";
		var_name y = "y";
		var_name z = "z";

		mock_pkb.addStmt(STMT_READ);
		mock_pkb.addStmt(STMT_PRINT);
		mock_pkb.addStmt(STMT_READ);
		mock_pkb.addStmt(STMT_PRINT);
		mock_pkb.addVariable(x);
		mock_pkb.addUsesS(1, x);
		mock_pkb.addVariable(y);
		mock_pkb.addUsesS(3, y);

		std::vector<std::pair<StmtInfo, var_name>> v = { {p1, x}, {p3, y} };
		EXPECT_EQ(pkb.getUsesSRelation(), v);
	}

	TEST_F(PKBAdapterTest, getUsesS) {
		StmtInfo p1{ 1, STMT_READ };
		StmtInfo p2{ 2, STMT_PRINT };
		StmtInfo p3{ 3, STMT_READ };
		StmtInfo p4{ 4, STMT_PRINT };
		var_name x = "x";
		var_name y = "y";
		var_name z = "z";

		mock_pkb.addStmt(STMT_READ);
		mock_pkb.addStmt(STMT_PRINT);
		mock_pkb.addStmt(STMT_READ);
		mock_pkb.addStmt(STMT_PRINT);
		mock_pkb.addVariable(x);
		mock_pkb.addUsesS(1, x);
		mock_pkb.addVariable(y);
		mock_pkb.addUsesS(2, y);
		mock_pkb.addUsesS(3, y);

		std::vector<StmtInfo> v1 = { p1, p2, p3 };
		std::vector<StmtInfo> v2 = pkb.getUsesS();
		std::sort(v1.begin(), v1.end());
		std::sort(v2.begin(), v2.end());
		EXPECT_EQ(v1, v2);

		v1 = { p1 };
		v2 = pkb.getUsesS(x);
		std::sort(v2.begin(), v2.end());
		EXPECT_EQ(v1, v2);

		v1 = { p2, p3 };
		v2 = pkb.getUsesS(y);
		std::sort(v1.begin(), v1.end());
		std::sort(v2.begin(), v2.end());
		EXPECT_EQ(v1, v2);

		v1 = { };
		v2 = pkb.getUsesS(z);
		EXPECT_EQ(v1, v2);
	}

	TEST_F(PKBAdapterTest, getUsedS) {
		var_name x = "x";
		var_name y = "y";
		var_name z = "z";

		mock_pkb.addStmt(STMT_READ);
		mock_pkb.addStmt(STMT_PRINT);
		mock_pkb.addStmt(STMT_READ);
		mock_pkb.addStmt(STMT_PRINT);
		mock_pkb.addVariable(x);
		mock_pkb.addUsesS(1, x);
		mock_pkb.addVariable(y);
		mock_pkb.addUsesS(2, x);
		mock_pkb.addUsesS(2, y);

		std::vector<var_name> v1 = { x };
		std::vector<var_name> v2 = pkb.getUsedS(1);
		std::sort(v2.begin(), v2.end());
		EXPECT_EQ(v1, v2);

		v1 = { x, y };
		v2 = pkb.getUsedS(2);
		std::sort(v1.begin(), v1.end());
		std::sort(v2.begin(), v2.end());
		EXPECT_EQ(v1, v2);

		v1 = { };
		v2 = pkb.getUsedS(3);
		EXPECT_EQ(v1, v2);
		v2 = pkb.getUsedS(4);
		EXPECT_EQ(v1, v2);
	}

	TEST_F(PKBAdapterTest, isCallsPEmpty) {
		proc_name first = "first";
		proc_name second = "second";
		proc_name third = "third";
		mock_pkb.addProcedure(first);
		mock_pkb.addProcedure(second);
		mock_pkb.addProcedure(third);

		EXPECT_TRUE(pkb.isCallsPEmpty());
		mock_pkb.addCallsP(first, second);
		EXPECT_FALSE(pkb.isCallsPEmpty());
		mock_pkb.addCallsP(second, third);
		EXPECT_FALSE(pkb.isCallsPEmpty());
	}

	TEST_F(PKBAdapterTest, isCallsP) {
		proc_name first = "first";
		proc_name second = "second";
		proc_name third = "third";
		proc_name fourth = "fourth";

		mock_pkb.addProcedure(first);
		mock_pkb.addProcedure(second);
		mock_pkb.addProcedure(third);

		mock_pkb.addCallsP(first, second);
		mock_pkb.addCallsP(second, third);
		EXPECT_TRUE(pkb.isCallsP(first, second));
		EXPECT_TRUE(pkb.isCallsP(second, third));

		EXPECT_FALSE(pkb.isCallsP(first, third));
		EXPECT_FALSE(pkb.isCallsP(first, fourth));
		EXPECT_FALSE(pkb.isCallsP(second, first));
		EXPECT_FALSE(pkb.isCallsP(second, fourth));
		EXPECT_FALSE(pkb.isCallsP(third, first));
		EXPECT_FALSE(pkb.isCallsP(third, fourth));

		EXPECT_FALSE(pkb.isCallsP(first, first));
		EXPECT_FALSE(pkb.isCallsP(second, second));
		EXPECT_FALSE(pkb.isCallsP(third, third));
		EXPECT_FALSE(pkb.isCallsP(fourth, fourth));
	}

	TEST_F(PKBAdapterTest, isCallerP) {
		proc_name first = "first";
		proc_name second = "second";
		proc_name third = "third";
		proc_name fourth = "fourth";

		mock_pkb.addProcedure(first);
		mock_pkb.addProcedure(second);
		mock_pkb.addProcedure(third);

		mock_pkb.addCallsP(first, second);
		mock_pkb.addCallsP(second, third);

		EXPECT_TRUE(pkb.isCallerP(first));
		EXPECT_TRUE(pkb.isCallerP(second));
		EXPECT_FALSE(pkb.isCallerP(third));
		EXPECT_FALSE(pkb.isCallerP(fourth));
	}

	TEST_F(PKBAdapterTest, isCalleeP) {
		proc_name first = "first";
		proc_name second = "second";
		proc_name third = "third";
		proc_name fourth = "fourth";

		mock_pkb.addProcedure(first);
		mock_pkb.addProcedure(second);
		mock_pkb.addProcedure(third);

		mock_pkb.addCallsP(first, second);
		mock_pkb.addCallsP(second, third);

		EXPECT_FALSE(pkb.isCalleeP(first));
		EXPECT_TRUE(pkb.isCalleeP(second));
		EXPECT_TRUE(pkb.isCalleeP(third));
		EXPECT_FALSE(pkb.isCalleeP(fourth));
	}

	TEST_F(PKBAdapterTest, getCallerP) {
		proc_name first = "first";
		proc_name second = "second";
		proc_name third = "third";
		proc_name fourth = "fourth";

		mock_pkb.addProcedure(first);
		mock_pkb.addProcedure(second);
		mock_pkb.addProcedure(third);

		mock_pkb.addCallsP(first, second);
		mock_pkb.addCallsP(second, third);

		std::vector<proc_name> v1 = { first, second };
		std::vector<proc_name> v2 = pkb.getCallerP();
		std::sort(v1.begin(), v1.end());
		std::sort(v2.begin(), v2.end());
		EXPECT_EQ(v1, v2);

		v1 = { };
		v2 = pkb.getCallerP(first);
		EXPECT_EQ(v1, v2);
		v2 = pkb.getCallerP(fourth);
		EXPECT_EQ(v1, v2);

		v1 = { first };
		v2 = pkb.getCallerP(second);
		EXPECT_EQ(v1, v2);

		v1 = { second };
		v2 = pkb.getCallerP(third);
		EXPECT_EQ(v1, v2);
	}

	TEST_F(PKBAdapterTest, getCalleeP) {
		proc_name first = "first";
		proc_name second = "second";
		proc_name third = "third";
		proc_name fourth = "fourth";

		mock_pkb.addProcedure(first);
		mock_pkb.addProcedure(second);
		mock_pkb.addProcedure(third);

		mock_pkb.addCallsP(first, second);
		mock_pkb.addCallsP(second, third);

		std::vector<proc_name> v1 = { second, third };
		std::vector<proc_name> v2 = pkb.getCalleeP();
		std::sort(v1.begin(), v1.end());
		std::sort(v2.begin(), v2.end());
		EXPECT_EQ(v1, v2);

		v1 = { };
		v2 = pkb.getCalleeP(third);
		EXPECT_EQ(v1, v2);
		v2 = pkb.getCalleeP(fourth);
		EXPECT_EQ(v1, v2);

		v1 = { second };
		v2 = pkb.getCalleeP(first);
		EXPECT_EQ(v1, v2);

		v1 = { third };
		v2 = pkb.getCalleeP(second);
		EXPECT_EQ(v1, v2);
	}

	TEST_F(PKBAdapterTest, getCallsPRelation) {
		proc_name first = "first";
		proc_name second = "second";
		proc_name third = "third";

		mock_pkb.addProcedure(first);
		mock_pkb.addProcedure(second);
		mock_pkb.addProcedure(third);

		mock_pkb.addCallsP(first, second);
		mock_pkb.addCallsP(second, third);
		std::vector<std::pair<proc_name, proc_name>> v = { {first, second}, {second, third} };
		EXPECT_EQ(pkb.getCallsPRelation(), v);
	}

	TEST_F(PKBAdapterTest, isCallsPTEmpty) {
		proc_name first = "first";
		proc_name second = "second";
		proc_name third = "third";
		mock_pkb.addProcedure(first);
		mock_pkb.addProcedure(second);
		mock_pkb.addProcedure(third);

		EXPECT_TRUE(pkb.isCallsPTEmpty());

		mock_pkb.addCallsP(first, second);
		mock_pkb.addCallsPT(first, second);
		EXPECT_FALSE(pkb.isCallsPTEmpty());

		mock_pkb.addCallsP(second, third);
		mock_pkb.addCallsPT(second, third);
		mock_pkb.addCallsPT(first, third);
		EXPECT_FALSE(pkb.isCallsPTEmpty());
	}

	TEST_F(PKBAdapterTest, isCallsPT) {
		proc_name first = "first";
		proc_name second = "second";
		proc_name third = "third";

		mock_pkb.addProcedure(first);
		mock_pkb.addProcedure(second);
		mock_pkb.addProcedure(third);

		mock_pkb.addCallsP(first, second);
		mock_pkb.addCallsP(second, third);

		mock_pkb.addCallsPT(first, second);
		mock_pkb.addCallsPT(first, third);
		mock_pkb.addCallsPT(second, third);

		EXPECT_TRUE(pkb.isCallsPT(first, second));
		EXPECT_TRUE(pkb.isCallsPT(first, third));
		EXPECT_TRUE(pkb.isCallsPT(second, third));

		EXPECT_FALSE(pkb.isCallsPT(first, first));
		EXPECT_FALSE(pkb.isCallsPT(second, second));
		EXPECT_FALSE(pkb.isCallsPT(third, third));
	}

	TEST_F(PKBAdapterTest, isCallerPT) {
		proc_name first = "first";
		proc_name second = "second";
		proc_name third = "third";
		proc_name fourth = "fourth";

		mock_pkb.addProcedure(first);
		mock_pkb.addProcedure(second);
		mock_pkb.addProcedure(third);

		mock_pkb.addCallsP(first, second);
		mock_pkb.addCallsP(second, third);
		mock_pkb.addCallsPT(first, second);
		mock_pkb.addCallsPT(first, third);
		mock_pkb.addCallsPT(second, third);

		EXPECT_TRUE(pkb.isCallerPT(first));
		EXPECT_TRUE(pkb.isCallerPT(second));
		EXPECT_FALSE(pkb.isCallerPT(third));
		EXPECT_FALSE(pkb.isCallerPT(fourth));
	}

	TEST_F(PKBAdapterTest, isCalleePT) {
		proc_name first = "first";
		proc_name second = "second";
		proc_name third = "third";
		proc_name fourth = "fourth";

		mock_pkb.addProcedure(first);
		mock_pkb.addProcedure(second);
		mock_pkb.addProcedure(third);

		mock_pkb.addCallsP(first, second);
		mock_pkb.addCallsP(second, third);
		mock_pkb.addCallsPT(first, second);
		mock_pkb.addCallsPT(first, third);
		mock_pkb.addCallsPT(second, third);

		EXPECT_FALSE(pkb.isCalleePT(first));
		EXPECT_TRUE(pkb.isCalleePT(second));
		EXPECT_TRUE(pkb.isCalleePT(third));
		EXPECT_FALSE(pkb.isCalleePT(fourth));
	}

	TEST_F(PKBAdapterTest, getCallerPT) {
		proc_name first = "first";
		proc_name second = "second";
		proc_name third = "third";
		proc_name fourth = "fourth";

		mock_pkb.addProcedure(first);
		mock_pkb.addProcedure(second);
		mock_pkb.addProcedure(third);

		mock_pkb.addCallsP(first, second);
		mock_pkb.addCallsP(second, third);
		mock_pkb.addCallsPT(first, second);
		mock_pkb.addCallsPT(first, third);
		mock_pkb.addCallsPT(second, third);

		std::vector<proc_name> v1 = { first, second };
		std::vector<proc_name> v2 = pkb.getCallerPT();
		std::sort(v1.begin(), v1.end());
		std::sort(v2.begin(), v2.end());
		EXPECT_EQ(v1, v2);

		v1 = { };
		v2 = pkb.getCallerPT(first);
		EXPECT_EQ(v1, v2);
		v2 = pkb.getCallerPT(fourth);
		EXPECT_EQ(v1, v2);

		v1 = { first };
		v2 = pkb.getCallerPT(second);
		EXPECT_EQ(v1, v2);

		v1 = { second, first };
		v2 = pkb.getCallerPT(third);
		std::sort(v1.begin(), v1.end());
		std::sort(v2.begin(), v2.end());
		EXPECT_EQ(v1, v2);
	}

	TEST_F(PKBAdapterTest, getCalleePT) {
		proc_name first = "first";
		proc_name second = "second";
		proc_name third = "third";
		proc_name fourth = "fourth";

		mock_pkb.addProcedure(first);
		mock_pkb.addProcedure(second);
		mock_pkb.addProcedure(third);

		mock_pkb.addCallsP(first, second);
		mock_pkb.addCallsP(second, third);
		mock_pkb.addCallsPT(first, second);
		mock_pkb.addCallsPT(first, third);
		mock_pkb.addCallsPT(second, third);

		std::vector<proc_name> v1 = { second, third };
		std::vector<proc_name> v2 = pkb.getCalleePT();
		std::sort(v1.begin(), v1.end());
		std::sort(v2.begin(), v2.end());
		EXPECT_EQ(v1, v2);

		v1 = { };
		v2 = pkb.getCalleePT(third);
		EXPECT_EQ(v1, v2);
		v2 = pkb.getCalleePT(fourth);
		EXPECT_EQ(v1, v2);

		v1 = { second, third };
		v2 = pkb.getCalleePT(first);
		std::sort(v1.begin(), v1.end());
		std::sort(v2.begin(), v2.end());
		EXPECT_EQ(v1, v2);

		v1 = { third };
		v2 = pkb.getCalleePT(second);
		EXPECT_EQ(v1, v2);
	}

	TEST_F(PKBAdapterTest, getCallsPTRelation) {
		proc_name first = "first";
		proc_name second = "second";
		proc_name third = "third";

		mock_pkb.addProcedure(first);
		mock_pkb.addProcedure(second);
		mock_pkb.addProcedure(third);

		mock_pkb.addCallsP(first, second);
		mock_pkb.addCallsP(second, third);
		mock_pkb.addCallsPT(first, second);
		mock_pkb.addCallsPT(first, third);
		mock_pkb.addCallsPT(second, third);

		std::vector<std::pair<proc_name, proc_name>> v = { {first, second}, {first, third}, {second, third} };
		EXPECT_EQ(pkb.getCallsPTRelation(), v);
	}

	TEST_F(PKBAdapterTest, isNextEmpty) {
		mock_pkb.addStmt(STMT_READ);
		mock_pkb.addStmt(STMT_PRINT);
		mock_pkb.addStmt(STMT_READ);
		EXPECT_TRUE(pkb.isNextEmpty());
		mock_pkb.addNext(1, 2);
		EXPECT_FALSE(pkb.isNextEmpty());
		mock_pkb.addNext(2, 3);
		EXPECT_FALSE(pkb.isNextEmpty());
	}

	TEST_F(PKBAdapterTest, isNext) {
		mock_pkb.addStmt(STMT_READ);
		mock_pkb.addStmt(STMT_WHILE);
		mock_pkb.addStmt(STMT_IF);
		mock_pkb.addStmt(STMT_PRINT);
		mock_pkb.addStmt(STMT_READ);
		mock_pkb.addNext(1, 2);
		mock_pkb.addNext(2, 3);
		mock_pkb.addNext(3, 4);
		mock_pkb.addNext(3, 5);
		mock_pkb.addNext(4, 2);
		mock_pkb.addNext(5, 2);

		EXPECT_FALSE(pkb.isNext(1, 1));
		EXPECT_TRUE(pkb.isNext(1, 2));
		EXPECT_FALSE(pkb.isNext(1, 3));
		EXPECT_FALSE(pkb.isNext(1, 4));
		EXPECT_FALSE(pkb.isNext(1, 5));

		EXPECT_FALSE(pkb.isNext(2, 1));
		EXPECT_FALSE(pkb.isNext(2, 2));
		EXPECT_TRUE(pkb.isNext(2, 3));
		EXPECT_FALSE(pkb.isNext(2, 4));
		EXPECT_FALSE(pkb.isNext(2, 5));

		EXPECT_FALSE(pkb.isNext(3, 1));
		EXPECT_FALSE(pkb.isNext(3, 2));
		EXPECT_FALSE(pkb.isNext(3, 3));
		EXPECT_TRUE(pkb.isNext(3, 4));
		EXPECT_TRUE(pkb.isNext(3, 5));

		EXPECT_FALSE(pkb.isNext(4, 1));
		EXPECT_TRUE(pkb.isNext(4, 2));
		EXPECT_FALSE(pkb.isNext(4, 3));
		EXPECT_FALSE(pkb.isNext(4, 4));
		EXPECT_FALSE(pkb.isNext(4, 5));

		EXPECT_FALSE(pkb.isNext(5, 1));
		EXPECT_TRUE(pkb.isNext(5, 2));
		EXPECT_FALSE(pkb.isNext(5, 3));
		EXPECT_FALSE(pkb.isNext(5, 4));
		EXPECT_FALSE(pkb.isNext(5, 5));

		EXPECT_FALSE(pkb.isNext(1));
		EXPECT_TRUE(pkb.isNext(2));
		EXPECT_TRUE(pkb.isNext(3));
		EXPECT_TRUE(pkb.isNext(4));
		EXPECT_TRUE(pkb.isNext(5));
	}

	TEST_F(PKBAdapterTest, isPrevious) {
		mock_pkb.addStmt(STMT_READ);
		mock_pkb.addStmt(STMT_WHILE);
		mock_pkb.addStmt(STMT_IF);
		mock_pkb.addStmt(STMT_PRINT);
		mock_pkb.addStmt(STMT_READ);
		mock_pkb.addStmt(STMT_READ);
		mock_pkb.addNext(1, 2);
		mock_pkb.addNext(2, 3);
		mock_pkb.addNext(3, 4);
		mock_pkb.addNext(3, 5);
		mock_pkb.addNext(4, 2);
		mock_pkb.addNext(5, 2);
		EXPECT_TRUE(pkb.isPrevious(1));
		EXPECT_TRUE(pkb.isPrevious(2));
		EXPECT_TRUE(pkb.isPrevious(3));
		EXPECT_TRUE(pkb.isPrevious(4));
		EXPECT_TRUE(pkb.isPrevious(5));
		EXPECT_FALSE(pkb.isPrevious(6));
	}

	TEST_F(PKBAdapterTest, getNext) {
		StmtInfo s1{ 1, STMT_READ };
		StmtInfo s2{ 2, STMT_WHILE };
		StmtInfo s3{ 3, STMT_IF };
		StmtInfo s4{ 4, STMT_PRINT };
		StmtInfo s5{ 5, STMT_READ };
		StmtInfo s6{ 6, STMT_READ };

		mock_pkb.addStmt(STMT_READ);
		mock_pkb.addStmt(STMT_WHILE);
		mock_pkb.addStmt(STMT_IF);
		mock_pkb.addStmt(STMT_PRINT);
		mock_pkb.addStmt(STMT_READ);
		mock_pkb.addStmt(STMT_READ);

		mock_pkb.addNext(1, 2);
		mock_pkb.addNext(2, 3);
		mock_pkb.addNext(3, 4);
		mock_pkb.addNext(3, 5);
		mock_pkb.addNext(4, 2);
		mock_pkb.addNext(5, 2);
		mock_pkb.addNext(2, 6);

		std::vector<StmtInfo> v1 = { s2, s3, s4, s5, s6 };
		std::vector<StmtInfo> v2 = pkb.getNext();
		std::sort(v1.begin(), v1.end());
		std::sort(v2.begin(), v2.end());
		EXPECT_EQ(v1, v2);

		v1 = { };
		v2 = pkb.getNext(6);
		EXPECT_EQ(v1, v2);

		v1 = { s2 };
		v2 = pkb.getNext(1);
		EXPECT_EQ(v1, v2);
		v2 = pkb.getNext(4);
		EXPECT_EQ(v1, v2);
		v2 = pkb.getNext(5);
		EXPECT_EQ(v1, v2);

		v1 = { s3, s6 };
		v2 = pkb.getNext(2);
		std::sort(v1.begin(), v1.end());
		std::sort(v2.begin(), v2.end());
		EXPECT_EQ(v1, v2);

		v1 = { s4, s5 };
		v2 = pkb.getNext(3);
		std::sort(v1.begin(), v1.end());
		std::sort(v2.begin(), v2.end());
		EXPECT_EQ(v1, v2);
	}

	TEST_F(PKBAdapterTest, getPrevious) {
		StmtInfo s1{ 1, STMT_READ };
		StmtInfo s2{ 2, STMT_WHILE };
		StmtInfo s3{ 3, STMT_IF };
		StmtInfo s4{ 4, STMT_PRINT };
		StmtInfo s5{ 5, STMT_READ };
		StmtInfo s6{ 6, STMT_READ };

		mock_pkb.addStmt(STMT_READ);
		mock_pkb.addStmt(STMT_WHILE);
		mock_pkb.addStmt(STMT_IF);
		mock_pkb.addStmt(STMT_PRINT);
		mock_pkb.addStmt(STMT_READ);
		mock_pkb.addStmt(STMT_READ);

		mock_pkb.addNext(1, 2);
		mock_pkb.addNext(2, 3);
		mock_pkb.addNext(3, 4);
		mock_pkb.addNext(3, 5);
		mock_pkb.addNext(4, 2);
		mock_pkb.addNext(5, 2);
		mock_pkb.addNext(2, 6);

		std::vector<StmtInfo> v1 = { s2, s3, s4, s5, s6 };
		std::vector<StmtInfo> v2 = pkb.getNext();
		std::sort(v1.begin(), v1.end());
		std::sort(v2.begin(), v2.end());
		EXPECT_EQ(v1, v2);

		v1 = { };
		v2 = pkb.getPrevious(1);
		EXPECT_EQ(v1, v2);

		v1 = { s3 };
		v2 = pkb.getPrevious(4);
		EXPECT_EQ(v1, v2);
		v2 = pkb.getPrevious(5);
		EXPECT_EQ(v1, v2);

		v1 = { s2 };
		v2 = pkb.getPrevious(3);
		EXPECT_EQ(v1, v2);
		v2 = pkb.getPrevious(6);
		EXPECT_EQ(v1, v2);

		v1 = { s1, s4, s5 };
		v2 = pkb.getPrevious(2);
		std::sort(v1.begin(), v1.end());
		std::sort(v2.begin(), v2.end());
		EXPECT_EQ(v1, v2);
	}

	TEST_F(PKBAdapterTest, getAllNextRelation) {
		StmtInfo s1{ 1, STMT_READ };
		StmtInfo s2{ 2, STMT_WHILE };
		StmtInfo s3{ 3, STMT_IF };
		StmtInfo s4{ 4, STMT_PRINT };
		StmtInfo s5{ 5, STMT_READ };
		StmtInfo s6{ 6, STMT_READ };

		mock_pkb.addStmt(STMT_READ);
		mock_pkb.addStmt(STMT_WHILE);
		mock_pkb.addStmt(STMT_IF);
		mock_pkb.addStmt(STMT_PRINT);
		mock_pkb.addStmt(STMT_READ);
		mock_pkb.addStmt(STMT_READ);

		mock_pkb.addNext(1, 2);
		mock_pkb.addNext(2, 3);
		mock_pkb.addNext(3, 4);
		mock_pkb.addNext(3, 5);
		mock_pkb.addNext(4, 2);
		mock_pkb.addNext(5, 2);
		mock_pkb.addNext(2, 6);
		std::vector<std::pair<StmtInfo, StmtInfo>> v1 = { {s1, s2}, {s2, s3}, {s3, s4}, {s3, s5}, {s4, s2}, {s5, s2}, {s2, s6} };
		std::vector<std::pair<StmtInfo, StmtInfo>> v2 = pkb.getAllNextRelation();
		std::sort(v1.begin(), v1.end());
		std::sort(v2.begin(), v2.end());
		EXPECT_EQ(v1, v2);
	}

	TEST_F(PKBAdapterTest, getAllIfUses) {
		var_name x = "x";
		var_name y = "y";
		var_name z = "z";
		var_name a = "a";

		mock_pkb.addStmt(STMT_IF);
		mock_pkb.addStmt(STMT_WHILE);
		mock_pkb.addStmt(STMT_IF);
		mock_pkb.addVariable(x);
		mock_pkb.addIf(1, x);
		mock_pkb.addVariable(y);
		mock_pkb.addIf(3, y);

		std::vector<std::pair<stmt_index, var_name>> v = { {1, x}, {3, y} };
		EXPECT_EQ(pkb.getAllIfUses(), v);
	}

	TEST_F(PKBAdapterTest, getIfUses) {
		var_name x = "x";
		var_name y = "y";
		var_name z = "z";

		mock_pkb.addStmt(STMT_IF);
		mock_pkb.addStmt(STMT_WHILE);
		mock_pkb.addStmt(STMT_IF);
		mock_pkb.addVariable(x);
		mock_pkb.addIf(1, x);
		mock_pkb.addVariable(y);
		mock_pkb.addIf(1, y);
		mock_pkb.addIf(3, y);

		std::vector<stmt_index> v1 = { 1, 3 };
		std::vector<stmt_index> v2 = pkb.getIfUses();
		std::sort(v1.begin(), v1.end());
		std::sort(v2.begin(), v2.end());
		EXPECT_EQ(v1, v2);

		v1 = { 1 };
		v2 = pkb.getIfUses(x);
		std::sort(v2.begin(), v2.end());
		EXPECT_EQ(v1, v2);

		v1 = { 1, 3 };
		v2 = pkb.getIfUses(y);
		std::sort(v1.begin(), v1.end());
		std::sort(v2.begin(), v2.end());
		EXPECT_EQ(v1, v2);

		v1 = { };
		v2 = pkb.getIfUses(z);
		EXPECT_EQ(v1, v2);
	}

	TEST_F(PKBAdapterTest, getAllWhileUses) {
		var_name x = "x";
		var_name y = "y";
		var_name z = "z";
		var_name a = "a";

		mock_pkb.addStmt(STMT_WHILE);
		mock_pkb.addStmt(STMT_IF);
		mock_pkb.addStmt(STMT_WHILE);
		mock_pkb.addVariable(x);
		mock_pkb.addWhile(1, x);
		mock_pkb.addVariable(y);
		mock_pkb.addWhile(3, y);

		std::vector<std::pair<stmt_index, var_name>> v = { {1, x}, {3, y} };
		EXPECT_EQ(pkb.getAllWhileUses(), v);
	}

	TEST_F(PKBAdapterTest, getWhileUses) {
		var_name x = "x";
		var_name y = "y";
		var_name z = "z";

		mock_pkb.addStmt(STMT_WHILE);
		mock_pkb.addStmt(STMT_IF);
		mock_pkb.addStmt(STMT_WHILE);
		mock_pkb.addVariable(x);
		mock_pkb.addWhile(1, x);
		mock_pkb.addVariable(y);
		mock_pkb.addWhile(1, y);
		mock_pkb.addWhile(3, y);

		std::vector<stmt_index> v1 = { 1, 3 };
		std::vector<stmt_index> v2 = pkb.getWhileUses();
		std::sort(v1.begin(), v1.end());
		std::sort(v2.begin(), v2.end());
		EXPECT_EQ(v1, v2);

		v1 = { 1 };
		v2 = pkb.getWhileUses(x);
		std::sort(v2.begin(), v2.end());
		EXPECT_EQ(v1, v2);

		v1 = { 1, 3 };
		v2 = pkb.getWhileUses(y);
		std::sort(v1.begin(), v1.end());
		std::sort(v2.begin(), v2.end());
		EXPECT_EQ(v1, v2);

		v1 = { };
		v2 = pkb.getWhileUses(z);
		EXPECT_EQ(v1, v2);
	}
	TEST_F(PKBAdapterTest, getRead) {
		mock_pkb.resetCache();

		var_name x = "x";
		var_name y = "y";

		mock_pkb.addStmt(STMT_READ);
		mock_pkb.addStmt(STMT_ASSIGN);
		mock_pkb.addStmt(STMT_READ);
		mock_pkb.addVariable(x);
		mock_pkb.addModifiesS(1, x);
		mock_pkb.addVariable(y);
		mock_pkb.addModifiesS(2, x);
		mock_pkb.addModifiesS(2, y);
		mock_pkb.addModifiesS(3, y);

		EXPECT_TRUE(pkb.isReadVar(x));
		EXPECT_TRUE(pkb.isReadVar(y));
		EXPECT_FALSE(pkb.isReadVar("z"));

		std::vector<stmt_index> s1 = { 1 };
		std::vector<stmt_index> s2 = pkb.getRead(x);
		EXPECT_EQ(s1, s2);

		std::vector<var_name> v1 = { x };
		std::vector<var_name> v2 = pkb.getReadVar(1);
		EXPECT_EQ(v1, v2);

		v1 = { y };
		v2 = pkb.getReadVar(3);
		EXPECT_EQ(v1, v2);

		std::vector<std::pair<stmt_index, var_name>> expectedPairs = { {1, x}, {3, y} };
		EXPECT_EQ(expectedPairs, pkb.getAllReadVars());

		mock_pkb.resetCache();
	}

	TEST_F(PKBAdapterTest, getPrint) {
		mock_pkb.resetCache();

		var_name x = "x";
		var_name y = "y";

		mock_pkb.addStmt(STMT_PRINT);
		mock_pkb.addStmt(STMT_ASSIGN);
		mock_pkb.addStmt(STMT_PRINT);
		mock_pkb.addVariable(x);
		mock_pkb.addUsesS(1, x);
		mock_pkb.addVariable(y);
		mock_pkb.addUsesS(2, x);
		mock_pkb.addUsesS(2, y);
		mock_pkb.addUsesS(3, y);

		EXPECT_TRUE(pkb.isPrintVar(x));
		EXPECT_TRUE(pkb.isPrintVar(y));
		EXPECT_FALSE(pkb.isPrintVar("z"));

		std::vector<stmt_index> s1 = { 1 };
		std::vector<stmt_index> s2 = pkb.getPrint(x);
		EXPECT_EQ(s1, s2);

		std::vector<var_name> v1 = { x };
		std::vector<var_name> v2 = pkb.getPrintVar(1);
		EXPECT_EQ(v1, v2);

		v1 = { y };
		v2 = pkb.getPrintVar(3);
		EXPECT_EQ(v1, v2);

		std::vector<std::pair<stmt_index, var_name>> expectedPairs = { {1, x}, {3, y} };
		EXPECT_EQ(expectedPairs, pkb.getAllPrintVars());

		mock_pkb.resetCache();
	}

	TEST_F(PKBAdapterTest, getCallS) {
		mock_pkb.resetCache();

		proc_name first = "first";
		proc_name second = "second";
		proc_name third = "third";

		mock_pkb.addStmt(STMT_CALL);
		mock_pkb.addStmt(STMT_CALL);
		mock_pkb.addStmt(STMT_READ);
		mock_pkb.addProcedure(first);
		mock_pkb.addProcedure(second);
		mock_pkb.addCallsS(1, first);
		mock_pkb.addCallsS(2, second);

		EXPECT_TRUE(pkb.isCalledS(first));
		EXPECT_TRUE(pkb.isCalledS(second));
		EXPECT_FALSE(pkb.isCalledS(third));

		std::vector<stmt_index> s1 = { 1 };
		std::vector<stmt_index> s2 = pkb.getCalleeS(first);
		EXPECT_EQ(s1, s2);

		std::vector<var_name> v1 = { first };
		std::vector<var_name> v2 = pkb.getCalledS(1);
		EXPECT_EQ(v1, v2);

		v1 = { };
		v2 = pkb.getCalledS(3);
		EXPECT_EQ(v1, v2);

		std::vector<std::pair<stmt_index, var_name>> expectedPairs = { {1, first}, {2, second} };
		EXPECT_EQ(expectedPairs, pkb.getAllCallS());

		mock_pkb.resetCache();
	}
}