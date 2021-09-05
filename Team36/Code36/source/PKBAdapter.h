#pragma once

#include <iostream>
#include <vector>

#include "Stmt.h"
#include "Common.h"

class PKBAdapter {
public:
	PKBAdapter();

	std::vector<var_name> getVariables();
	std::vector<StmtInfo> getStmts();
	std::vector<constant> getConstants();
	std::vector<proc_name> getProcedures();
	std::vector<StmtInfo> getAssigns();
	std::vector<StmtInfo> getPrints();
	std::vector<StmtInfo> getCalls();
	std::vector<StmtInfo> getReads();
	std::vector<StmtInfo> getWhiles();
	std::vector<StmtInfo> getIfs();

	//Follow relation function
	bool isFollow(stmt_index, stmt_index);
	bool isFollowed(stmt_index);
	bool isFollowing(stmt_index);
	std::vector<StmtInfo> getFollowed();
	std::vector<StmtInfo> getFollowing();
	std::vector<StmtInfo> getFollowed(stmt_index);
	std::vector<StmtInfo> getFollowing(stmt_index);
	std::vector<std::vector<StmtInfo>> getFollows();

	//FollowT relation function
	bool isFollowT(stmt_index, stmt_index);
	bool isFollowedT(stmt_index);
	bool isFollowingT(stmt_index);
	std::vector<StmtInfo> getFollowedT();
	std::vector<StmtInfo> getFollowingT();
	std::vector<StmtInfo> getFollowedT(stmt_index);
	std::vector<StmtInfo> getFollowingT(stmt_index);
	std::vector<std::vector<StmtInfo>> getFollowsT();


	//Parent relation function
	bool isParent(stmt_index, stmt_index);
	bool isChild(stmt_index);
	bool isParent(stmt_index);
	std::vector<StmtInfo> getChild();
	std::vector<StmtInfo> getParent();
	std::vector<StmtInfo> getChild(stmt_index);
	std::vector<StmtInfo> getParent(stmt_index);
	std::vector<std::vector<StmtInfo>> getAllParentRelation();

	//ParentT relation function
	bool isParentT(stmt_index, stmt_index);
	bool isChildT(stmt_index);
	bool isParentT(stmt_index);
	std::vector<StmtInfo> getChildT();
	std::vector<StmtInfo> getParentT();
	std::vector<StmtInfo> getChildT(stmt_index);
	std::vector<StmtInfo> getParentT(stmt_index);
	std::vector<std::vector<StmtInfo>> getAllParentTRelation();

	//ModifiesP relation Function
	bool isModifies(proc_name, var_name);
	bool isModifies(proc_name);
	std::vector<std::pair<std::string, std::string>> getModifiesPRelation();
	std::vector<std::string> getModifiesP();
	std::vector<std::string> getModifiesP(var_name);
	std::vector<std::string> getModifiedP(proc_name);

	//ModifiesS relation Function
	bool isModifies(stmt_index, var_name);
	bool isModifies(stmt_index);
	std::vector<std::pair<StmtInfo, var_name>> getModifiesSRelation();
	std::vector<StmtInfo> getModifiesS();
	std::vector<StmtInfo> getModifiesS(var_name);
	std::vector<std::string> getModifiedS(stmt_index);

	//UsesP relation Function
	bool isUses(proc_name, var_name);
	bool isUses(proc_name);
	std::vector<std::pair<std::string, std::string>> getUsesPRelation();
	std::vector<std::string> getUsesP();
	std::vector<std::string> getUsesP(var_name);
	std::vector<std::string> getUsedP(proc_name);

	//UsesS relation Function
	bool isUses(stmt_index, var_name);
	bool isUses(stmt_index);
	std::vector<std::pair<StmtInfo, var_name>> getUsesSRelation();
	std::vector<StmtInfo> getUsesS();
	std::vector<StmtInfo> getUsesS(var_name);
	std::vector<std::string> getUsedS(stmt_index);
};