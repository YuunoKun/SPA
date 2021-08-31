#include "pch.h"

#include "Common.h"
#include "Entity.h"
#include "Query.h"
#include <Query.cpp>

namespace UnitTesting {
	TEST(Query, entities) {
		Query q;
		std::unordered_map<std::string, Entity> o;

		EXPECT_EQ(q.getEntities(), o);

		q.addEntity({ STMT,"test1" });
		o.insert({ "test1" ,{ STMT,"test1" } });
		EXPECT_EQ(q.getEntities(), o);

		q.addEntity({ WHILE,"test1231" });
		o.insert({ "test1231" ,{ WHILE,"test1231" } });
		EXPECT_EQ(q.getEntities(), o);

		q.addEntity({ IF,"test14151" });
		o.insert({ "test14151" ,{ IF,"test14151" } });
		EXPECT_EQ(q.getEntities(), o);

		q.addEntity({ WILD,"test123" });
		o.insert({ "test123" ,{ WILD,"test123" } });
		EXPECT_EQ(q.getEntities(), o);

		q.addEntity({ READ,"test14151" });
		EXPECT_EQ(q.getEntities(), o);
		o.insert({ "test14151" ,{ READ,"test14151" } });
		EXPECT_EQ(q.getEntities(), o);

		q.addEntity({ READ,"test141511" });
		EXPECT_NE(q.getEntities(), o);
		o.insert({ "test1415111" ,{ READ,"test141511" } });
		EXPECT_NE(q.getEntities(), o);
	}

	TEST(Query, relations) {
		Query q;
		std::vector<RelRef> o;

		EXPECT_EQ(q.getRelations(), o);

		q.addRelation({ MODIFIES_S, {} });
		EXPECT_NE(q.getRelations(), o);
		o.push_back({ MODIFIES_S,{} });
		EXPECT_EQ(q.getRelations(), o);

		q.addRelation({ FOLLOWS, {{STMT,"test1"}} });
		EXPECT_NE(q.getRelations(), o);
		o.push_back({ FOLLOWS,{{STMT,"test1"}} });
		EXPECT_EQ(q.getRelations(), o);

		q.addRelation({ FOLLOWS, {{STMT,"test1"},{READ, "test2"}} });
		EXPECT_NE(q.getRelations(), o);
		o.push_back({ FOLLOWS,{{STMT,"test1"},{READ, "test2"}} });
		EXPECT_EQ(q.getRelations(), o);

		q.addRelation({ FOLLOWS, {{STMT,"test1"},{READ, "test2"}} });
		EXPECT_NE(q.getRelations(), o);
		o.push_back({ FOLLOWS_T,{{STMT,"test1"},{READ, "test2"}} });
		EXPECT_NE(q.getRelations(), o);
	}

	TEST(Query, pattern) {
		Query q;
		std::vector<Pattern> o;

		TNode tnode;

		EXPECT_EQ(q.getPatterns(), o);

		q.addPattern({ { ASSIGN, "patternType" }, { VARIABLE, "leftExpr"}, {}, true });
		EXPECT_NE(q.getPatterns(), o);
		o.push_back({ { ASSIGN, "patternType" }, { VARIABLE, "leftExpr"}, {}, true });
		EXPECT_EQ(q.getPatterns(), o);
	}

	TEST(Query, selected) {
		Query q;

		Entity e(STMT, "test1");
		q.setSelected({ STMT,"test1" });
		EXPECT_EQ(q.getSelected(), e);

		e = Entity(WHILE, "test1231");
		q.setSelected(e);
		EXPECT_EQ(q.getSelected(), e);

		e = Entity(IF, "test14151");
		q.setSelected({ IF,"test14151" });
		EXPECT_EQ(q.getSelected(), e);

		e = Entity(WILD, "test123");
		q.setSelected(e);
		EXPECT_EQ(q.getSelected(), e);

		e = Entity(READ, "test123");
		q.setSelected({ READ,"test123" });
		EXPECT_EQ(q.getSelected(), e);

		q.setSelected({ READ,"test141511" });
		EXPECT_TRUE(!(q.getSelected() == e));
	}
}