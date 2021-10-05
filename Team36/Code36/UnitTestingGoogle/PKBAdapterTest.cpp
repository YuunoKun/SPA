#include "pch.h"

#include "PKBAdapter.h"
#include "PKB.h"
#include "RelationTable.cpp"

namespace UnitTesting {
	class PKBAdapterTest : public testing::Test {
	protected:
		PKBAdapterTest() {
		}

		// If the constructor and destructor are not enough for setting up
		// and cleaning up each test, you can define the following methods:

		virtual void SetUp() override {
			// Code here will be called immediately after the constructor (right
			// before each test).
			PKB::getInstance().resetCache();
		}

		void TearDown() override {
			// Code here will be called immediately after each test (right
			// before the destructor).
		}
		PKBAdapter pkb;
	};

	TEST_F(PKBAdapterTest, getVariables) {
		var_name a = "a";
		var_name b = "b";
		var_name c = "c";
		var_name d = "d";
		var_name e = "e";
		std::vector<var_name> v{ a };
		PKB::getInstance().addVariable(a);
		EXPECT_EQ(pkb.getVariables(), v);

		v.push_back(b);
		PKB::getInstance().addVariable(b);
		EXPECT_EQ(pkb.getVariables(), v);

		v.push_back(c);
		PKB::getInstance().addVariable(c);
		EXPECT_EQ(pkb.getVariables(), v);

		v.push_back(d);
		PKB::getInstance().addVariable(d);
		EXPECT_EQ(pkb.getVariables(), v);

		v.push_back(e);
		PKB::getInstance().addVariable(e);
		EXPECT_EQ(pkb.getVariables(), v);

		PKB::getInstance().addVariable(e);
		EXPECT_EQ(pkb.getVariables(), v);
	}

	TEST_F(PKBAdapterTest, getStmts) {
		std::vector<StmtInfo> v = { { 1, STMT_WHILE }, { 2, STMT_IF} , { 3, STMT_READ },
			{ 4, STMT_PRINT}, { 5, STMT_CALL }, { 6, STMT_ASSIGN}, { 7, STMT_ASSIGN} };

		PKB::getInstance().addStmt(STMT_WHILE);
		PKB::getInstance().addStmt(STMT_IF);
		PKB::getInstance().addStmt(STMT_READ);
		PKB::getInstance().addStmt(STMT_PRINT);
		PKB::getInstance().addStmt(STMT_CALL);
		PKB::getInstance().addStmt(STMT_ASSIGN);
		PKB::getInstance().addStmt(STMT_ASSIGN);

		EXPECT_EQ(pkb.getStmts(), v);
	}

	TEST_F(PKBAdapterTest, getConstants) {
		constant a = 1;
		constant b = 2;
		constant c = 3;
		constant d = 4;
		constant e = 5;
		std::vector<constant> v{ a };
		PKB::getInstance().addConstant(a);
		EXPECT_EQ(pkb.getConstants(), v);

		v.push_back(b);
		PKB::getInstance().addConstant(b);
		EXPECT_EQ(pkb.getConstants(), v);

		v.push_back(c);
		PKB::getInstance().addConstant(c);
		EXPECT_EQ(pkb.getConstants(), v);

		v.push_back(d);
		PKB::getInstance().addConstant(d);
		EXPECT_EQ(pkb.getConstants(), v);

		v.push_back(e);
		PKB::getInstance().addConstant(e);
		EXPECT_EQ(pkb.getConstants(), v);

		PKB::getInstance().addConstant(e);
		EXPECT_EQ(pkb.getConstants(), v);
	}

	TEST_F(PKBAdapterTest, getAssigns) {
		std::vector<StmtInfo> v = { { 6, STMT_ASSIGN}, { 7, STMT_ASSIGN} };

		PKB::getInstance().addStmt(STMT_WHILE);
		PKB::getInstance().addStmt(STMT_IF);
		PKB::getInstance().addStmt(STMT_READ);
		PKB::getInstance().addStmt(STMT_PRINT);
		PKB::getInstance().addStmt(STMT_CALL);
		PKB::getInstance().addStmt(STMT_ASSIGN);
		PKB::getInstance().addStmt(STMT_ASSIGN);

		EXPECT_EQ(pkb.getAssigns(), v);
	}

	TEST_F(PKBAdapterTest, getCalls) {
		std::vector<StmtInfo> v = { { 5, STMT_CALL }, { 6, STMT_CALL },{ 7, STMT_CALL },{ 10, STMT_CALL } };

		PKB::getInstance().addStmt(STMT_WHILE);
		PKB::getInstance().addStmt(STMT_IF);
		PKB::getInstance().addStmt(STMT_READ);
		PKB::getInstance().addStmt(STMT_PRINT);
		PKB::getInstance().addStmt(STMT_CALL);
		PKB::getInstance().addStmt(STMT_CALL);
		PKB::getInstance().addStmt(STMT_CALL);
		PKB::getInstance().addStmt(STMT_ASSIGN);
		PKB::getInstance().addStmt(STMT_ASSIGN);
		PKB::getInstance().addStmt(STMT_CALL);

		EXPECT_EQ(pkb.getCalls(), v);
	}

	TEST_F(PKBAdapterTest, getReads) {
		std::vector<StmtInfo> v = { { 3, STMT_READ } };

		PKB::getInstance().addStmt(STMT_WHILE);
		PKB::getInstance().addStmt(STMT_IF);
		PKB::getInstance().addStmt(STMT_READ);
		PKB::getInstance().addStmt(STMT_PRINT);
		PKB::getInstance().addStmt(STMT_CALL);
		PKB::getInstance().addStmt(STMT_ASSIGN);
		PKB::getInstance().addStmt(STMT_ASSIGN);

		EXPECT_EQ(pkb.getReads(), v);
	}

	TEST_F(PKBAdapterTest, getWhiles) {
		std::vector<StmtInfo> v = { { 1, STMT_WHILE } };

		PKB::getInstance().addStmt(STMT_WHILE);
		PKB::getInstance().addStmt(STMT_IF);
		PKB::getInstance().addStmt(STMT_READ);
		PKB::getInstance().addStmt(STMT_PRINT);
		PKB::getInstance().addStmt(STMT_CALL);
		PKB::getInstance().addStmt(STMT_ASSIGN);
		PKB::getInstance().addStmt(STMT_ASSIGN);

		EXPECT_EQ(pkb.getWhiles(), v);
	}

	TEST_F(PKBAdapterTest, getIfs) {
		std::vector<StmtInfo> v = { { 2, STMT_IF} };

		PKB::getInstance().addStmt(STMT_WHILE);
		PKB::getInstance().addStmt(STMT_IF);
		PKB::getInstance().addStmt(STMT_READ);
		PKB::getInstance().addStmt(STMT_PRINT);
		PKB::getInstance().addStmt(STMT_CALL);
		PKB::getInstance().addStmt(STMT_ASSIGN);
		PKB::getInstance().addStmt(STMT_ASSIGN);

		EXPECT_EQ(pkb.getIfs(), v);
	}

	// Todo: Hoe Keat to update after change of expr typedef
	//TEST_F(PKBAdapterTest, getAssignInfo) {
	//	std::vector<var_name> v = { "x", "y", "z", "y", "z", "z" };
	//	std::vector<expr> e = { "x", "y*x", "x+y", "z%x", "x-x", "x+y-z" };
	//	std::vector<stmt_index> s = { 1, 2, 3, 4, 5, 6 };

	//	std::vector<StmtInfo> si;
	//	std::vector<assign_info> a;

	//	for (unsigned int i = 0; i < s.size(); i++) {
	//		PKB::getInstance().addStmt(STMT_ASSIGN);
	//		PKB::getInstance().addVariable(v[i]);
	//		si.push_back({ s[i], STMT_ASSIGN });
	//		a.push_back(std::make_pair(si[i], v[i]));
	//		PKB::getInstance().addModifiesS(s[i], v[i]);
	//		PKB::getInstance().addExprTree(s[i], e[i]);
	//	}

	//	EXPECT_EQ(pkb.getAssignInfo(), a);

	//	std::vector<assign_info> result = { a[0] };
	//	EXPECT_EQ(pkb.getAssignInfo("x", false), result);
	//	EXPECT_EQ(pkb.getAssignInfo("x ", false), result);
	//	EXPECT_EQ(pkb.getAssignInfo("x  ", false), result);
	//	EXPECT_EQ(pkb.getAssignInfo(" x", false), result);
	//	EXPECT_EQ(pkb.getAssignInfo("  x", false), result);
	//	EXPECT_EQ(pkb.getAssignInfo("  x ", false), result);
	//	EXPECT_EQ(pkb.getAssignInfo("  x  ", false), result);

	//	result = a;
	//	EXPECT_EQ(pkb.getAssignInfo("x", true), result);
	//	EXPECT_EQ(pkb.getAssignInfo("x ", true), result);
	//	EXPECT_EQ(pkb.getAssignInfo("x  ", true), result);
	//	EXPECT_EQ(pkb.getAssignInfo(" x", true), result);
	//	EXPECT_EQ(pkb.getAssignInfo("  x", true), result);
	//	EXPECT_EQ(pkb.getAssignInfo("  x ", true), result);
	//	EXPECT_EQ(pkb.getAssignInfo("  x  ", true), result);

	//	result = { a[1], a[2], a[5] };
	//	EXPECT_EQ(pkb.getAssignInfo("y", true), result);
	//	EXPECT_EQ(pkb.getAssignInfo("y ", true), result);
	//	EXPECT_EQ(pkb.getAssignInfo(" y", true), result);
	//	EXPECT_EQ(pkb.getAssignInfo("  y  ", true), result);

	//	result = { a[3], a[5] };
	//	EXPECT_EQ(pkb.getAssignInfo("z", true), result);
	//	EXPECT_EQ(pkb.getAssignInfo("z ", true), result);
	//	EXPECT_EQ(pkb.getAssignInfo(" z", true), result);
	//	EXPECT_EQ(pkb.getAssignInfo("  z  ", true), result);

	//	result = {};
	//	EXPECT_EQ(pkb.getAssignInfo("y", false), result);
	//	EXPECT_EQ(pkb.getAssignInfo("y ", false), result);
	//	EXPECT_EQ(pkb.getAssignInfo(" y", false), result);
	//	EXPECT_EQ(pkb.getAssignInfo("  y  ", false), result);
	//	EXPECT_EQ(pkb.getAssignInfo("z", false), result);
	//	EXPECT_EQ(pkb.getAssignInfo("z ", false), result);
	//	EXPECT_EQ(pkb.getAssignInfo(" z", false), result);
	//	EXPECT_EQ(pkb.getAssignInfo("  z  ", false), result);
	//	EXPECT_EQ(pkb.getAssignInfo("c", true), result);
	//	EXPECT_EQ(pkb.getAssignInfo("c ", false), result);
	//	EXPECT_EQ(pkb.getAssignInfo(" c", false), result);
	//	EXPECT_EQ(pkb.getAssignInfo("  c  ", false), result);
	//}
	TEST_F(PKBAdapterTest, isFollowEmpty) {
		PKB::getInstance().addStmt(STMT_READ);
		PKB::getInstance().addStmt(STMT_PRINT);
		PKB::getInstance().addStmt(STMT_READ);
		PKB::getInstance().addStmt(STMT_IF);
		EXPECT_TRUE(pkb.isFollowEmpty());
		PKB::getInstance().addFollows(1, 2);
		EXPECT_FALSE(pkb.isFollowEmpty());
		PKB::getInstance().addFollows(2, 3);
		EXPECT_FALSE(pkb.isFollowEmpty());
	}

	TEST_F(PKBAdapterTest, isFollow) {
		PKB::getInstance().addStmt(STMT_READ);
		PKB::getInstance().addStmt(STMT_PRINT);
		PKB::getInstance().addStmt(STMT_READ);
		PKB::getInstance().addStmt(STMT_IF);
		PKB::getInstance().addFollows(1, 2);
		PKB::getInstance().addFollows(2, 3);
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
		PKB::getInstance().addStmt(STMT_READ);
		PKB::getInstance().addStmt(STMT_PRINT);
		PKB::getInstance().addStmt(STMT_READ);
		PKB::getInstance().addStmt(STMT_IF);
		PKB::getInstance().addFollows(1, 2);
		PKB::getInstance().addFollows(2, 3);
		EXPECT_TRUE(pkb.isFollowed(1));
		EXPECT_TRUE(pkb.isFollowed(2));
		EXPECT_FALSE(pkb.isFollowed(3));
		EXPECT_FALSE(pkb.isFollowed(4));
	}

	TEST_F(PKBAdapterTest, isFollowing) {
		PKB::getInstance().addStmt(STMT_READ);
		PKB::getInstance().addStmt(STMT_PRINT);
		PKB::getInstance().addStmt(STMT_READ);
		PKB::getInstance().addStmt(STMT_IF);
		PKB::getInstance().addFollows(1, 2);
		PKB::getInstance().addFollows(2, 3);
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

		PKB::getInstance().addStmt(STMT_READ);
		PKB::getInstance().addStmt(STMT_PRINT);
		PKB::getInstance().addStmt(STMT_READ);
		PKB::getInstance().addStmt(STMT_IF);
		PKB::getInstance().addFollows(1, 2);
		PKB::getInstance().addFollows(2, 3);

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

		PKB::getInstance().addStmt(STMT_READ);
		PKB::getInstance().addStmt(STMT_PRINT);
		PKB::getInstance().addStmt(STMT_READ);
		PKB::getInstance().addStmt(STMT_IF);
		PKB::getInstance().addFollows(1, 2);
		PKB::getInstance().addFollows(2, 3);

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

		PKB::getInstance().addStmt(STMT_READ);
		PKB::getInstance().addStmt(STMT_PRINT);
		PKB::getInstance().addStmt(STMT_READ);
		PKB::getInstance().addStmt(STMT_IF);
		PKB::getInstance().addFollows(1, 2);
		PKB::getInstance().addFollows(2, 3);
		std::vector<std::pair<StmtInfo, StmtInfo>> v = { {p1, p2}, {p2, p3} };
		EXPECT_EQ(pkb.getFollows(), v);
	}

	TEST_F(PKBAdapterTest, isFollowTEmpty) {
		PKB::getInstance().addStmt(STMT_READ);
		PKB::getInstance().addStmt(STMT_PRINT);
		PKB::getInstance().addStmt(STMT_READ);
		PKB::getInstance().addStmt(STMT_IF);
		PKB::getInstance().generateFollowsT();
		EXPECT_TRUE(pkb.isFollowTEmpty());
		PKB::getInstance().addFollows(1, 2);
		PKB::getInstance().generateFollowsT();
		EXPECT_FALSE(pkb.isFollowTEmpty());
		PKB::getInstance().addFollows(2, 3);
		PKB::getInstance().generateFollowsT();
		EXPECT_FALSE(pkb.isFollowTEmpty());
	}

	TEST_F(PKBAdapterTest, isFollowT) {
		PKB::getInstance().addStmt(STMT_READ);
		PKB::getInstance().addStmt(STMT_PRINT);
		PKB::getInstance().addStmt(STMT_READ);
		PKB::getInstance().addStmt(STMT_IF);
		PKB::getInstance().addFollows(1, 2);
		PKB::getInstance().addFollows(2, 3);
		PKB::getInstance().generateFollowsT();

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
		PKB::getInstance().addStmt(STMT_READ);
		PKB::getInstance().addStmt(STMT_PRINT);
		PKB::getInstance().addStmt(STMT_READ);
		PKB::getInstance().addStmt(STMT_IF);
		PKB::getInstance().addFollows(1, 2);
		PKB::getInstance().addFollows(2, 3);
		PKB::getInstance().generateFollowsT();

		EXPECT_TRUE(pkb.isFollowedT(1));
		EXPECT_TRUE(pkb.isFollowedT(2));
		EXPECT_FALSE(pkb.isFollowedT(3));
		EXPECT_FALSE(pkb.isFollowedT(4));
	}
	TEST_F(PKBAdapterTest, isFollowingT) {
		PKB::getInstance().addStmt(STMT_READ);
		PKB::getInstance().addStmt(STMT_PRINT);
		PKB::getInstance().addStmt(STMT_READ);
		PKB::getInstance().addStmt(STMT_IF);
		PKB::getInstance().addFollows(1, 2);
		PKB::getInstance().addFollows(2, 3);
		PKB::getInstance().generateFollowsT();
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

		PKB::getInstance().addStmt(STMT_READ);
		PKB::getInstance().addStmt(STMT_PRINT);
		PKB::getInstance().addStmt(STMT_READ);
		PKB::getInstance().addStmt(STMT_IF);
		PKB::getInstance().addFollows(1, 2);
		PKB::getInstance().addFollows(2, 3);
		PKB::getInstance().generateFollowsT();

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

		PKB::getInstance().addStmt(STMT_READ);
		PKB::getInstance().addStmt(STMT_PRINT);
		PKB::getInstance().addStmt(STMT_READ);
		PKB::getInstance().addStmt(STMT_IF);
		PKB::getInstance().addFollows(1, 2);
		PKB::getInstance().addFollows(2, 3);
		PKB::getInstance().generateFollowsT();

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

		PKB::getInstance().addStmt(STMT_READ);
		PKB::getInstance().addStmt(STMT_PRINT);
		PKB::getInstance().addStmt(STMT_READ);
		PKB::getInstance().addStmt(STMT_IF);
		PKB::getInstance().addFollows(1, 2);
		PKB::getInstance().addFollows(2, 3);
		PKB::getInstance().generateFollowsT();
		std::vector<std::pair<StmtInfo, StmtInfo>> v1 = { {p1, p2}, {p1, p3}, {p2, p3} };
		std::vector<std::pair<StmtInfo, StmtInfo>> v2 = pkb.getFollowsT();
		std::sort(v2.begin(), v2.end());
		EXPECT_EQ(v1, v2);
	}

	TEST_F(PKBAdapterTest, isParentEmpty) {
		PKB::getInstance().addStmt(STMT_IF);
		PKB::getInstance().addStmt(STMT_WHILE);
		PKB::getInstance().addStmt(STMT_READ);
		PKB::getInstance().addStmt(STMT_IF);
		EXPECT_TRUE(pkb.isParentEmpty());
		PKB::getInstance().addParent(1, 2);
		EXPECT_FALSE(pkb.isParentEmpty());
		PKB::getInstance().addParent(2, 3);
		EXPECT_FALSE(pkb.isParentEmpty());
	}
	TEST_F(PKBAdapterTest, isParent) {
		PKB::getInstance().addStmt(STMT_IF);
		PKB::getInstance().addStmt(STMT_WHILE);
		PKB::getInstance().addStmt(STMT_IF);
		PKB::getInstance().addStmt(STMT_IF);
		PKB::getInstance().addStmt(STMT_WHILE);
		PKB::getInstance().addParent(1, 2);
		PKB::getInstance().addParent(1, 3);
		PKB::getInstance().addParent(2, 3);
		PKB::getInstance().addParent(3, 4);
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
		PKB::getInstance().addStmt(STMT_IF);
		PKB::getInstance().addStmt(STMT_WHILE);
		PKB::getInstance().addStmt(STMT_IF);
		PKB::getInstance().addStmt(STMT_IF);
		PKB::getInstance().addStmt(STMT_WHILE);
		PKB::getInstance().addParent(1, 2);
		PKB::getInstance().addParent(1, 3);
		PKB::getInstance().addParent(2, 3);
		PKB::getInstance().addParent(3, 4);
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

		PKB::getInstance().addStmt(STMT_IF);
		PKB::getInstance().addStmt(STMT_WHILE);
		PKB::getInstance().addStmt(STMT_IF);
		PKB::getInstance().addStmt(STMT_IF);
		PKB::getInstance().addStmt(STMT_WHILE);
		PKB::getInstance().addParent(1, 2);
		PKB::getInstance().addParent(1, 3);
		PKB::getInstance().addParent(2, 3);
		PKB::getInstance().addParent(3, 4);

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

		PKB::getInstance().addStmt(STMT_IF);
		PKB::getInstance().addStmt(STMT_WHILE);
		PKB::getInstance().addStmt(STMT_IF);
		PKB::getInstance().addStmt(STMT_IF);
		PKB::getInstance().addStmt(STMT_WHILE);
		PKB::getInstance().addParent(1, 2);
		PKB::getInstance().addParent(1, 3);
		PKB::getInstance().addParent(2, 3);
		PKB::getInstance().addParent(3, 4);

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

		PKB::getInstance().addStmt(STMT_IF);
		PKB::getInstance().addStmt(STMT_WHILE);
		PKB::getInstance().addStmt(STMT_IF);
		PKB::getInstance().addStmt(STMT_IF);
		PKB::getInstance().addStmt(STMT_WHILE);
		PKB::getInstance().addParent(1, 2);
		PKB::getInstance().addParent(1, 3);
		PKB::getInstance().addParent(2, 3);
		PKB::getInstance().addParent(3, 4);

		std::vector<std::pair<StmtInfo, StmtInfo>> v = { {p1, p2}, {p1, p3}, {p2, p3}, {p3, p4} };
		EXPECT_EQ(pkb.getAllParentRelation(), v);
	}

	TEST_F(PKBAdapterTest, isParentT) {
		PKB::getInstance().addStmt(STMT_IF);
		PKB::getInstance().addStmt(STMT_WHILE);
		PKB::getInstance().addStmt(STMT_IF);
		PKB::getInstance().addStmt(STMT_IF);
		PKB::getInstance().addStmt(STMT_WHILE);
		PKB::getInstance().addParent(1, 2);
		PKB::getInstance().addParent(1, 3);
		PKB::getInstance().addParent(2, 3);
		PKB::getInstance().addParent(3, 4);
		PKB::getInstance().generateParentT();

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
		PKB::getInstance().addStmt(STMT_IF);
		PKB::getInstance().addStmt(STMT_WHILE);
		PKB::getInstance().addStmt(STMT_IF);
		PKB::getInstance().addStmt(STMT_IF);
		PKB::getInstance().addStmt(STMT_WHILE);
		PKB::getInstance().addParent(1, 2);
		PKB::getInstance().addParent(1, 3);
		PKB::getInstance().addParent(2, 3);
		PKB::getInstance().addParent(3, 4);
		PKB::getInstance().generateParentT();

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

		PKB::getInstance().addStmt(STMT_IF);
		PKB::getInstance().addStmt(STMT_WHILE);
		PKB::getInstance().addStmt(STMT_IF);
		PKB::getInstance().addStmt(STMT_IF);
		PKB::getInstance().addStmt(STMT_WHILE);
		PKB::getInstance().addParent(1, 2);
		PKB::getInstance().addParent(1, 3);
		PKB::getInstance().addParent(2, 3);
		PKB::getInstance().addParent(3, 4);
		PKB::getInstance().generateParentT();

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

		PKB::getInstance().addStmt(STMT_IF);
		PKB::getInstance().addStmt(STMT_WHILE);
		PKB::getInstance().addStmt(STMT_IF);
		PKB::getInstance().addStmt(STMT_IF);
		PKB::getInstance().addStmt(STMT_WHILE);
		PKB::getInstance().addParent(1, 2);
		PKB::getInstance().addParent(1, 3);
		PKB::getInstance().addParent(2, 3);
		PKB::getInstance().addParent(3, 4);
		PKB::getInstance().generateParentT();

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

		PKB::getInstance().addStmt(STMT_IF);
		PKB::getInstance().addStmt(STMT_WHILE);
		PKB::getInstance().addStmt(STMT_IF);
		PKB::getInstance().addStmt(STMT_IF);
		PKB::getInstance().addStmt(STMT_WHILE);
		PKB::getInstance().addParent(1, 2);
		PKB::getInstance().addParent(1, 3);
		PKB::getInstance().addParent(2, 3);
		PKB::getInstance().addParent(3, 4);
		PKB::getInstance().generateParentT();

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

		PKB::getInstance().addProcedure(main1);
		PKB::getInstance().addProcedure(main2);
		PKB::getInstance().addProcedure(sub1);
		PKB::getInstance().addProcedure(sub2);
		PKB::getInstance().addVariable(x);
		PKB::getInstance().addModifiesP(main1, x);
		PKB::getInstance().addVariable(y);
		PKB::getInstance().addModifiesP(main2, y);

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

		PKB::getInstance().addProcedure(main1);
		PKB::getInstance().addProcedure(main2);
		PKB::getInstance().addProcedure(sub1);
		PKB::getInstance().addProcedure(sub2);
		PKB::getInstance().addVariable(x);
		PKB::getInstance().addModifiesP(main1, x);
		PKB::getInstance().addVariable(y);
		PKB::getInstance().addModifiesP(main2, y);

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

		PKB::getInstance().addProcedure(main1);
		PKB::getInstance().addProcedure(main2);
		PKB::getInstance().addProcedure(sub1);
		PKB::getInstance().addProcedure(sub2);
		PKB::getInstance().addVariable(x);
		PKB::getInstance().addModifiesP(main1, x);
		PKB::getInstance().addVariable(y);
		PKB::getInstance().addModifiesP(sub1, y);
		PKB::getInstance().addModifiesP(main2, y);

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

		PKB::getInstance().addProcedure(main1);
		PKB::getInstance().addProcedure(main2);
		PKB::getInstance().addProcedure(sub1);
		PKB::getInstance().addProcedure(sub2);
		PKB::getInstance().addVariable(x);
		PKB::getInstance().addModifiesP(main1, x);
		PKB::getInstance().addVariable(y);
		PKB::getInstance().addModifiesP(main2, x);
		PKB::getInstance().addModifiesP(main2, y);

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

		PKB::getInstance().addStmt(STMT_READ);
		PKB::getInstance().addStmt(STMT_PRINT);
		PKB::getInstance().addStmt(STMT_READ);
		PKB::getInstance().addStmt(STMT_PRINT);
		PKB::getInstance().addVariable(x);
		PKB::getInstance().addModifiesS(1, x);
		PKB::getInstance().addVariable(y);
		PKB::getInstance().addModifiesS(3, y);

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

		PKB::getInstance().addStmt(STMT_READ);
		PKB::getInstance().addStmt(STMT_PRINT);
		PKB::getInstance().addStmt(STMT_READ);
		PKB::getInstance().addStmt(STMT_PRINT);
		PKB::getInstance().addVariable(x);
		PKB::getInstance().addModifiesS(1, x);
		PKB::getInstance().addVariable(y);
		PKB::getInstance().addModifiesS(3, y);

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

		PKB::getInstance().addStmt(STMT_READ);
		PKB::getInstance().addStmt(STMT_PRINT);
		PKB::getInstance().addStmt(STMT_READ);
		PKB::getInstance().addStmt(STMT_PRINT);
		PKB::getInstance().addVariable(x);
		PKB::getInstance().addModifiesS(1, x);
		PKB::getInstance().addVariable(y);
		PKB::getInstance().addModifiesS(2, y);
		PKB::getInstance().addModifiesS(3, y);

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

		PKB::getInstance().addStmt(STMT_READ);
		PKB::getInstance().addStmt(STMT_PRINT);
		PKB::getInstance().addStmt(STMT_READ);
		PKB::getInstance().addStmt(STMT_PRINT);
		PKB::getInstance().addVariable(x);
		PKB::getInstance().addModifiesS(1, x);
		PKB::getInstance().addVariable(y);
		PKB::getInstance().addModifiesS(2, x);
		PKB::getInstance().addModifiesS(2, y);

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

		PKB::getInstance().addProcedure(main1);
		PKB::getInstance().addProcedure(main2);
		PKB::getInstance().addProcedure(sub1);
		PKB::getInstance().addProcedure(sub2);
		PKB::getInstance().addVariable(x);
		PKB::getInstance().addUsesP(main1, x);
		PKB::getInstance().addVariable(y);
		PKB::getInstance().addUsesP(main2, y);

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

		PKB::getInstance().addProcedure(main1);
		PKB::getInstance().addProcedure(main2);
		PKB::getInstance().addProcedure(sub1);
		PKB::getInstance().addProcedure(sub2);
		PKB::getInstance().addVariable(x);
		PKB::getInstance().addUsesP(main1, x);
		PKB::getInstance().addVariable(y);
		PKB::getInstance().addUsesP(main2, y);

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

		PKB::getInstance().addProcedure(main1);
		PKB::getInstance().addProcedure(main2);
		PKB::getInstance().addProcedure(sub1);
		PKB::getInstance().addProcedure(sub2);
		PKB::getInstance().addVariable(x);
		PKB::getInstance().addUsesP(main1, x);
		PKB::getInstance().addVariable(y);
		PKB::getInstance().addUsesP(sub1, y);
		PKB::getInstance().addUsesP(main2, y);

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

		PKB::getInstance().addProcedure(main1);
		PKB::getInstance().addProcedure(main2);
		PKB::getInstance().addProcedure(sub1);
		PKB::getInstance().addProcedure(sub2);
		PKB::getInstance().addVariable(x);
		PKB::getInstance().addUsesP(main1, x);
		PKB::getInstance().addVariable(y);
		PKB::getInstance().addUsesP(main2, x);
		PKB::getInstance().addUsesP(main2, y);

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

		PKB::getInstance().addStmt(STMT_READ);
		PKB::getInstance().addStmt(STMT_PRINT);
		PKB::getInstance().addStmt(STMT_READ);
		PKB::getInstance().addStmt(STMT_PRINT);
		PKB::getInstance().addVariable(x);
		PKB::getInstance().addUsesS(1, x);
		PKB::getInstance().addVariable(y);
		PKB::getInstance().addUsesS(3, y);

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

		PKB::getInstance().addStmt(STMT_READ);
		PKB::getInstance().addStmt(STMT_PRINT);
		PKB::getInstance().addStmt(STMT_READ);
		PKB::getInstance().addStmt(STMT_PRINT);
		PKB::getInstance().addVariable(x);
		PKB::getInstance().addUsesS(1, x);
		PKB::getInstance().addVariable(y);
		PKB::getInstance().addUsesS(3, y);

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

		PKB::getInstance().addStmt(STMT_READ);
		PKB::getInstance().addStmt(STMT_PRINT);
		PKB::getInstance().addStmt(STMT_READ);
		PKB::getInstance().addStmt(STMT_PRINT);
		PKB::getInstance().addVariable(x);
		PKB::getInstance().addUsesS(1, x);
		PKB::getInstance().addVariable(y);
		PKB::getInstance().addUsesS(2, y);
		PKB::getInstance().addUsesS(3, y);

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

		PKB::getInstance().addStmt(STMT_READ);
		PKB::getInstance().addStmt(STMT_PRINT);
		PKB::getInstance().addStmt(STMT_READ);
		PKB::getInstance().addStmt(STMT_PRINT);
		PKB::getInstance().addVariable(x);
		PKB::getInstance().addUsesS(1, x);
		PKB::getInstance().addVariable(y);
		PKB::getInstance().addUsesS(2, x);
		PKB::getInstance().addUsesS(2, y);

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
		PKB::getInstance().addProcedure(first);
		PKB::getInstance().addProcedure(second);
		PKB::getInstance().addProcedure(third);

		EXPECT_TRUE(pkb.isCallsPEmpty());
		PKB::getInstance().addCallsP(first, second);
		EXPECT_FALSE(pkb.isCallsPEmpty());
		PKB::getInstance().addCallsP(second, third);
		EXPECT_FALSE(pkb.isCallsPEmpty());
	}

	TEST_F(PKBAdapterTest, isCallsP) {
		proc_name first = "first";
		proc_name second = "second";
		proc_name third = "third";
		proc_name fourth = "fourth";

		PKB::getInstance().addProcedure(first);
		PKB::getInstance().addProcedure(second);
		PKB::getInstance().addProcedure(third);

		PKB::getInstance().addCallsP(first, second);
		PKB::getInstance().addCallsP(second, third);
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

		PKB::getInstance().addProcedure(first);
		PKB::getInstance().addProcedure(second);
		PKB::getInstance().addProcedure(third);

		PKB::getInstance().addCallsP(first, second);
		PKB::getInstance().addCallsP(second, third);

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

		PKB::getInstance().addProcedure(first);
		PKB::getInstance().addProcedure(second);
		PKB::getInstance().addProcedure(third);

		PKB::getInstance().addCallsP(first, second);
		PKB::getInstance().addCallsP(second, third);

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

		PKB::getInstance().addProcedure(first);
		PKB::getInstance().addProcedure(second);
		PKB::getInstance().addProcedure(third);

		PKB::getInstance().addCallsP(first, second);
		PKB::getInstance().addCallsP(second, third);

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

		PKB::getInstance().addProcedure(first);
		PKB::getInstance().addProcedure(second);
		PKB::getInstance().addProcedure(third);

		PKB::getInstance().addCallsP(first, second);
		PKB::getInstance().addCallsP(second, third);

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

		PKB::getInstance().addProcedure(first);
		PKB::getInstance().addProcedure(second);
		PKB::getInstance().addProcedure(third);

		PKB::getInstance().addCallsP(first, second);
		PKB::getInstance().addCallsP(second, third);
		std::vector<std::pair<proc_name, proc_name>> v = { {first, second}, {second, third} };
		EXPECT_EQ(pkb.getCallsPRelation(), v);
	}

	TEST_F(PKBAdapterTest, isCallsPTEmpty) {
		proc_name first = "first";
		proc_name second = "second";
		proc_name third = "third";
		PKB::getInstance().addProcedure(first);
		PKB::getInstance().addProcedure(second);
		PKB::getInstance().addProcedure(third);
		PKB::getInstance().generateCallsPT();

		EXPECT_TRUE(pkb.isCallsPTEmpty());

		PKB::getInstance().addCallsP(first, second);
		PKB::getInstance().generateCallsPT();
		EXPECT_FALSE(pkb.isCallsPTEmpty());

		PKB::getInstance().addCallsP(second, third);
		PKB::getInstance().generateCallsPT();
		EXPECT_FALSE(pkb.isCallsPTEmpty());
	}

	TEST_F(PKBAdapterTest, isCallsPT) {
		proc_name first = "first";
		proc_name second = "second";
		proc_name third = "third";
		proc_name fourth = "fourth";
		proc_name fifth = "fifth";

		PKB::getInstance().addProcedure(first);
		PKB::getInstance().addProcedure(second);
		PKB::getInstance().addProcedure(third);
		PKB::getInstance().addProcedure(fourth);

		PKB::getInstance().addCallsP(first, second);
		PKB::getInstance().addCallsP(second, third);
		PKB::getInstance().addCallsP(third, fourth);
		PKB::getInstance().generateCallsPT();

		EXPECT_TRUE(pkb.isCallsPT(first, second));
		EXPECT_TRUE(pkb.isCallsPT(first, third));
		EXPECT_TRUE(pkb.isCallsPT(first, fourth));
		EXPECT_TRUE(pkb.isCallsPT(second, third));
		EXPECT_TRUE(pkb.isCallsPT(second, fourth));
		EXPECT_TRUE(pkb.isCallsPT(third, fourth));

		EXPECT_FALSE(pkb.isCallsPT(first, fifth));
		EXPECT_FALSE(pkb.isCallsPT(second, fifth));
		EXPECT_FALSE(pkb.isCallsPT(third, fifth));
		EXPECT_FALSE(pkb.isCallsPT(fourth, fifth));

		EXPECT_FALSE(pkb.isCallsPT(first, first));
		EXPECT_FALSE(pkb.isCallsPT(second, second));
		EXPECT_FALSE(pkb.isCallsPT(third, third));
		EXPECT_FALSE(pkb.isCallsPT(fourth, fourth));
		EXPECT_FALSE(pkb.isCallsPT(fifth, fifth));
	}

	TEST_F(PKBAdapterTest, isCallerPT) {
		proc_name first = "first";
		proc_name second = "second";
		proc_name third = "third";
		proc_name fourth = "fourth";

		PKB::getInstance().addProcedure(first);
		PKB::getInstance().addProcedure(second);
		PKB::getInstance().addProcedure(third);

		PKB::getInstance().addCallsP(first, second);
		PKB::getInstance().addCallsP(second, third);
		PKB::getInstance().generateCallsPT();

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

		PKB::getInstance().addProcedure(first);
		PKB::getInstance().addProcedure(second);
		PKB::getInstance().addProcedure(third);

		PKB::getInstance().addCallsP(first, second);
		PKB::getInstance().addCallsP(second, third);
		PKB::getInstance().generateCallsPT();

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

		PKB::getInstance().addProcedure(first);
		PKB::getInstance().addProcedure(second);
		PKB::getInstance().addProcedure(third);

		PKB::getInstance().addCallsP(first, second);
		PKB::getInstance().addCallsP(second, third);
		PKB::getInstance().generateCallsPT();

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

		PKB::getInstance().addProcedure(first);
		PKB::getInstance().addProcedure(second);
		PKB::getInstance().addProcedure(third);

		PKB::getInstance().addCallsP(first, second);
		PKB::getInstance().addCallsP(second, third);
		PKB::getInstance().generateCallsPT();

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

		PKB::getInstance().addProcedure(first);
		PKB::getInstance().addProcedure(second);
		PKB::getInstance().addProcedure(third);

		PKB::getInstance().addCallsP(first, second);
		PKB::getInstance().addCallsP(second, third);
		PKB::getInstance().generateCallsPT();

		std::vector<std::pair<proc_name, proc_name>> v = { {first, second}, {first, third}, {second, third} };
		EXPECT_EQ(pkb.getCallsPTRelation(), v);
	}
}