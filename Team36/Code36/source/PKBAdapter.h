#pragma once

#include <iostream>
#include <vector>

#include "Stmt.h"
#include "Common.h"

class PKBAdapter {
public:
	PKBAdapter();

	std::vector<variable_name> getVariables();
	std::vector<Stmt> getStmts();
	std::vector<constant> getConstants();
	std::vector<procedure_name> getProcedures();
	std::vector<Stmt> getAssigns();
	std::vector<Stmt> getPrints();
	std::vector<Stmt> getCalls();
	std::vector<Stmt> getReads();
	std::vector<Stmt> getWhiles();
	std::vector<Stmt> getIfs();

	//Follow relation function
	bool isFollow(stmt_index, stmt_index);
	bool isFollowed(stmt_index);
	bool isFollowing(stmt_index);
	std::vector<Stmt> getFollowed();
	std::vector<Stmt> getFollowing();
	std::vector<Stmt> getFollowed(stmt_index);
	std::vector<Stmt> getFollowing(stmt_index);
	std::vector<std::vector<Stmt>> getFollows();

	//FollowT relation function
	bool isFollowT(stmt_index, stmt_index);
	bool isFollowedT(stmt_index);
	bool isFollowingT(stmt_index);
	std::vector<Stmt> getFollowedT();
	std::vector<Stmt> getFollowingT();
	std::vector<Stmt> getFollowedT(stmt_index);
	std::vector<Stmt> getFollowingT(stmt_index);
	std::vector<std::vector<Stmt>> getFollowsT();


	//Parent relation function
	bool isParent(stmt_index, stmt_index);
	bool isChild(stmt_index);
	bool isParent(stmt_index);
	std::vector<Stmt> getChild();
	std::vector<Stmt> getParent();
	std::vector<Stmt> getChild(stmt_index);
	std::vector<Stmt> getParent(stmt_index);
	std::vector<std::vector<Stmt>> getAllParentRelation();

	//ParentT relation function
	bool isParentT(stmt_index, stmt_index);
	bool isChildT(stmt_index);
	bool isParentT(stmt_index);
	std::vector<Stmt> getChildT();
	std::vector<Stmt> getParentT();
	std::vector<Stmt> getChildT(stmt_index);
	std::vector<Stmt> getParentT(stmt_index);
	std::vector<std::vector<Stmt>> getAllParentTRelation();
};