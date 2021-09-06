#include "pch.h"

#include "PKBAdapter.h"
#include "PKB.h"

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
		std::vector<var_name> v{a};
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
		std::vector<StmtInfo> v = {{ 2, STMT_IF}};

		PKB::getInstance().addStmt(STMT_WHILE);
		PKB::getInstance().addStmt(STMT_IF);
		PKB::getInstance().addStmt(STMT_READ);
		PKB::getInstance().addStmt(STMT_PRINT);
		PKB::getInstance().addStmt(STMT_CALL);
		PKB::getInstance().addStmt(STMT_ASSIGN);
		PKB::getInstance().addStmt(STMT_ASSIGN);

		EXPECT_EQ(pkb.getIfs(), v);
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
		std::vector<std::pair<StmtInfo, StmtInfo>> v = {{p1, p2}, {p2, p3} };
		EXPECT_EQ(pkb.getFollows(), v);
	}

	TEST_F(PKBAdapterTest, isFollowT) {
		//Todo
	}

	TEST_F(PKBAdapterTest, isFollowedT) {
		//Todo
	}

	TEST_F(PKBAdapterTest, isFollowingT) {
		//Todo
	}

	TEST_F(PKBAdapterTest, getFollowedT) {
		//Todo
	}

	TEST_F(PKBAdapterTest, getFollowingT) {
		//Todo
	}

	TEST_F(PKBAdapterTest, getFollowsT) {
		//Todo
	}

	TEST_F(PKBAdapterTest, isParent) {
		//Todo
	}

	TEST_F(PKBAdapterTest, isChild) {
		//Todo
	}

	TEST_F(PKBAdapterTest, getChild) {
		//Todo
	}

	TEST_F(PKBAdapterTest, getParent) {
		//Todo
	}

	TEST_F(PKBAdapterTest, getAllParentRelation) {
		//Todo
	}

	TEST_F(PKBAdapterTest, isParentT) {
		//Todo
	}

	TEST_F(PKBAdapterTest, isChildT) {
		//Todo
	}

	TEST_F(PKBAdapterTest, getParentT) {
		//Todo
	}

	TEST_F(PKBAdapterTest, isModifies) {
		//Todo
	}
	TEST_F(PKBAdapterTest, getModifiesPRelation) {
		//Todo
	}
	TEST_F(PKBAdapterTest, getModifiesP) {
		//Todo
	}
	TEST_F(PKBAdapterTest, getModifiedP) {
		//Todo
	}
	TEST_F(PKBAdapterTest, getModifiesSRelation) {
		//Todo
	}
	TEST_F(PKBAdapterTest, getModifiesS) {
		//Todo
	}
	TEST_F(PKBAdapterTest, getModifiedS) {
		//Todo
	}
	TEST_F(PKBAdapterTest, isUses) {
		//Todo
	}
	TEST_F(PKBAdapterTest, getUsesPRelation) {
		//Todo
	}
	TEST_F(PKBAdapterTest, getUsesP) {
		//Todo
	}
	TEST_F(PKBAdapterTest, getUsedP) {
		//Todo
	}
	TEST_F(PKBAdapterTest, getUsesSRelation) {
		//Todo
	}
	TEST_F(PKBAdapterTest, getUsesS) {
		//Todo
	}
	TEST_F(PKBAdapterTest, getUsedS) {
		//Todo
	}
}
