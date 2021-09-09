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
	std::vector<AssignInfo> getAssignInfo();
	std::vector<AssignInfo> getAssignInfo(std::string, bool);

	//Follow relation function
	bool isFollowEmpty();
	bool isFollow(stmt_index, stmt_index);
	bool isFollowed(stmt_index);
	bool isFollowing(stmt_index);
	std::vector<StmtInfo> getFollowed();
	std::vector<StmtInfo> getFollowing();
	std::vector<StmtInfo> getFollowed(stmt_index);
	std::vector<StmtInfo> getFollowing(stmt_index);
	std::vector<std::pair<StmtInfo, StmtInfo>> getFollows();

	//FollowT relation function
	bool isFollowTEmpty();
	bool isFollowT(stmt_index, stmt_index);
	bool isFollowedT(stmt_index);
	bool isFollowingT(stmt_index);
	std::vector<StmtInfo> getFollowedT();
	std::vector<StmtInfo> getFollowingT();
	std::vector<StmtInfo> getFollowedT(stmt_index);
	std::vector<StmtInfo> getFollowingT(stmt_index);
	std::vector<std::pair<StmtInfo, StmtInfo>> getFollowsT();

	//Parent relation function
	bool isParentEmpty();
	bool isParent(stmt_index, stmt_index);
	bool isChild(stmt_index);
	bool isParent(stmt_index);
	std::vector<StmtInfo> getChild();
	std::vector<StmtInfo> getParent();
	std::vector<StmtInfo> getChild(stmt_index);
	std::vector<StmtInfo> getParent(stmt_index);
	std::vector<std::pair<StmtInfo, StmtInfo>> getAllParentRelation();

	//ParentT relation function
	bool isParentTEmpty();
	bool isParentT(stmt_index, stmt_index);
	bool isChildT(stmt_index);
	bool isParentT(stmt_index);
	std::vector<StmtInfo> getChildT();
	std::vector<StmtInfo> getParentT();
	std::vector<StmtInfo> getChildT(stmt_index);
	std::vector<StmtInfo> getParentT(stmt_index);
	std::vector<std::pair<StmtInfo, StmtInfo>> getAllParentTRelation();

	//ModifiesP relation Function
	bool isModifiesP(proc_name, var_name);
	bool isModifiesP(proc_name);
	std::vector<std::pair<proc_name, var_name>> getModifiesPRelation();
	std::vector<proc_name> getModifiesP();
	std::vector<proc_name> getModifiesP(var_name);
	std::vector<var_name> getModifiedP(proc_name);

	//ModifiesS relation Function
	bool isModifiesSEmpty();
	bool isModifiesS(stmt_index, var_name);
	bool isModifiesS(stmt_index);
	bool isModifiesS(var_name);
	std::vector<std::pair<StmtInfo, var_name>> getModifiesSRelation();
	std::vector<StmtInfo> getModifiesS();
	std::vector<StmtInfo> getModifiesS(var_name);
	std::vector<var_name> getModifiedS();
	std::vector<var_name> getModifiedS(stmt_index);

	//UsesP relation Function
	bool isUsesP(proc_name, var_name);
	bool isUsesP(proc_name);
	std::vector<std::pair<proc_name, var_name>> getUsesPRelation();
	std::vector<proc_name> getUsesP();
	std::vector<proc_name> getUsesP(var_name);
	std::vector<var_name> getUsedP(proc_name);

	//UsesS relation Function
	bool isUsesSEmpty();
	bool isUsesS(stmt_index, var_name);
	bool isUsesS(stmt_index);
	bool isUsesS(var_name);
	std::vector<std::pair<StmtInfo, var_name>> getUsesSRelation();
	std::vector<StmtInfo> getUsesS();
	std::vector<StmtInfo> getUsesS(var_name);
	std::vector<var_name> getUsedS();
	std::vector<var_name> getUsedS(stmt_index);
};