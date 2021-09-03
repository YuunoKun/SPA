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
};