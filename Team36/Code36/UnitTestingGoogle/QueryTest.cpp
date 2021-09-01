#include "pch.h"

#include "Common.h"
#include "Entity.h"
#include "Query.h"
#include <Query.cpp>

namespace UnitTesting {
	TEST(Query, entities) {
		Query q;
		std::unordered_map<std::string, Entity*> o;

		EXPECT_EQ(q.getEntities(), o);

		Entity e(STMT, "test1");
		q.addEntity(e);
		o.insert({ "test1" , &e });
		EXPECT_EQ(q.getEntities(), o);

		q.addEntity({ WHILE,"test1231" });
		o.insert({ "test1231" ,{ WHILE,"test1231" } });
		EXPECT_EQ(q.getEntities(), o);

		q.addEntity({ IF,"test14151" });
		o.insert({ "test14151" ,{ IF,"test14151" } });
		EXPECT_EQ(q.getEntities(), o);

		e = Entity(WILD, "test123");
		q.addEntity(e);
		o.insert({ "test123" , &e });
		EXPECT_EQ(q.getEntities(), o);

		q.addEntity({ READ,"test14151" });
		EXPECT_EQ(q.getEntities(), o);
		o.insert({ "test14151" , &e });
		EXPECT_EQ(q.getEntities(), o);

		q.addEntity({ READ,"test141511" });
		EXPECT_NE(q.getEntities(), o);
		o.insert({ "test1415111" , &e });
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
		q.setSelected(e);
		EXPECT_EQ(q.getSelected(), e);

		e = Entity(WHILE, "test1231");
		q.setSelected(e);
		EXPECT_EQ(q.getSelected(), e);

		e = Entity(IF, "test14151");
		q.setSelected(e);
		EXPECT_EQ(q.getSelected(), e);

		e = Entity(WILD, "test123");
		q.setSelected(e);
		EXPECT_EQ(q.getSelected(), e);

		e = Entity(READ, "test123");
		q.setSelected(e);
		EXPECT_EQ(q.getSelected(), e);

		e = Entity({ READ,"test141511" });
		Entity e1(IF,"test141511");
		q.setSelected(e);
		EXPECT_TRUE(!(q.getSelected() == e1));
	}
}