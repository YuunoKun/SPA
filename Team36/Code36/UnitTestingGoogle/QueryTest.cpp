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

		Entity e(STMT, "test1");
		q.addEntity(e);
		o.insert({ "test1" , e });
		EXPECT_EQ(q.getEntities(), o);

		e = Entity(WHILE, "test1231");
		q.addEntity(e);
		o.insert({ "test1231" , e });
		EXPECT_EQ(q.getEntities(), o);

		e = Entity(IF, "test14151");
		q.addEntity(e);
		o.insert({ "test14151" , e });
		EXPECT_EQ(q.getEntities(), o);

		e = Entity(WILD, "test123");
		q.addEntity(e);
		o.insert({ "test123" , e });
		EXPECT_EQ(q.getEntities(), o);

		e = Entity(READ, "test14151");
		q.addEntity(e);
		EXPECT_EQ(q.getEntities(), o);
		o.insert({ "test14151" , e });
		EXPECT_EQ(q.getEntities(), o);

		e = Entity(READ, "test141511");
		q.addEntity(e);
		EXPECT_NE(q.getEntities(), o);
		o.insert({ "test1415111" , e });
		EXPECT_NE(q.getEntities(), o);
	}

	TEST(Query, relations) {
		Query q;
		std::vector<RelRef> o;

		EXPECT_EQ(q.getRelations(), o);

		RelRef e({ MODIFIES_S, {} });
		q.addRelation(e);
		EXPECT_NE(q.getRelations(), o);
		o.push_back(e);
		EXPECT_EQ(q.getRelations(), o);

		e = RelRef({ FOLLOWS, {{STMT,"test1"}} });
		q.addRelation(e);
		EXPECT_NE(q.getRelations(), o);
		o.push_back(e);
		EXPECT_EQ(q.getRelations(), o);

		e = RelRef({ FOLLOWS, {{STMT,"test1"},{READ, "test2"}} });
		q.addRelation(e);
		EXPECT_NE(q.getRelations(), o);
		o.push_back(e);
		EXPECT_EQ(q.getRelations(), o);

		e = RelRef({ FOLLOWS, {{STMT,"test1"},{READ, "test2"}} });
		q.addRelation(e);
		EXPECT_NE(q.getRelations(), o);
		e = RelRef({ FOLLOWS_T, {{STMT,"test1"},{READ, "test2"}} });
		o.push_back(e);
		EXPECT_NE(q.getRelations(), o);
	}

	TEST(Query, pattern) {
		Query q;
		std::vector<Pattern> o;

		EXPECT_EQ(q.getPatterns(), o);

		Pattern e({ { STMT, "test" }, {}, true });
		q.addPattern(e);
		EXPECT_NE(q.getPatterns(), o);
		o.push_back(e);
		EXPECT_EQ(q.getPatterns(), o);

		e = Pattern({ { WHILE, "test1" }, {}, false });
		q.addPattern(e);
		EXPECT_NE(q.getPatterns(), o);
		o.push_back(e);
		EXPECT_EQ(q.getPatterns(), o);

		e = Pattern({ { READ, "test1" }, {}, true });
		q.addPattern(e);
		EXPECT_NE(q.getPatterns(), o);
		e = Pattern({ { PRINT, "test1" }, {}, true });
		o.push_back(e);
		EXPECT_NE(q.getPatterns(), o);
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