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

		Entity e(STMT, Synonym{ "test1" });
		q.addEntity(e);
		o.insert({ "test1" , e });
		EXPECT_EQ(q.getEntities(), o);

		e = Entity(WHILE, Synonym{ "test1231" });
		q.addEntity(e);
		o.insert({ "test1231" , e });
		EXPECT_EQ(q.getEntities(), o);

		e = Entity(IF, Synonym{ "test14151" });
		q.addEntity(e);
		o.insert({ "test14151" , e });
		EXPECT_EQ(q.getEntities(), o);

		e = Entity(WILD, Synonym{ "test123" });
		q.addEntity(e);
		o.insert({ "test123" , e });
		EXPECT_EQ(q.getEntities(), o);

		e = Entity(READ, Synonym{ "test14151" });
		q.addEntity(e);
		EXPECT_EQ(q.getEntities(), o);
		o.insert({ "test14151" , e });
		EXPECT_EQ(q.getEntities(), o);

		e = Entity(READ, Synonym{ "test141511" });
		q.addEntity(e);
		EXPECT_NE(q.getEntities(), o);
		o.insert({ "test1415111" , e });
		EXPECT_NE(q.getEntities(), o);
	}

	TEST(Query, relations) {
		Query q;
		std::vector<Clause> o;

		EXPECT_EQ(q.getClauses(), o);

		RelRef e({ MODIFIES_P, {ASSIGN,Synonym{"test4"}}, {ASSIGN,Synonym{"test4"}} });
		q.addRelation(e);
		EXPECT_NE(q.getClauses(), o);
		o.push_back({ e });
		EXPECT_EQ(q.getClauses(), o);

		e = RelRef({ FOLLOWS, {STMT,Synonym{"test1"}}, {STMT,Synonym{"test1"}} });
		q.addRelation(e);
		EXPECT_NE(q.getClauses(), o);
		o.push_back({ e });
		EXPECT_EQ(q.getClauses(), o);

		e = RelRef({ FOLLOWS, {STMT,Synonym{"test1"}},{READ, Synonym{"test2"}} });
		q.addRelation(e);
		EXPECT_NE(q.getClauses(), o);
		o.push_back({ e });
		EXPECT_EQ(q.getClauses(), o);

		e = RelRef({ FOLLOWS, {STMT,Synonym{"test1"}},{READ, Synonym{"test2"}} });
		q.addRelation(e);
		EXPECT_NE(q.getClauses(), o);
		e = RelRef({ FOLLOWS_T, {STMT,Synonym{"test1"}},{READ, Synonym{"test2"}} });
		o.push_back({ e });
		EXPECT_NE(q.getClauses(), o);
	}

	TEST(Query, pattern) {
		Query q;
		std::vector<Clause> o;

		EXPECT_EQ(q.getClauses(), o);

		Pattern e({ { ASSIGN, Synonym{"patternType"} }, { VARIABLE, Synonym{"leftExpr"}}, {}, true });
		q.addPattern(e);
		EXPECT_NE(q.getClauses(), o);
		o.push_back({ e });
		EXPECT_EQ(q.getClauses(), o);
	}

	TEST(Query, selected) {
		Query q;
		std::vector<Entity> v;

		Entity e(STMT, Synonym{ "test1" });
		v.push_back(e);
		q.addSelected(e);
		EXPECT_EQ(q.getSelected(), v);

		e = Entity(WHILE, Synonym{ "test1231" });

		v.push_back(e);
		q.addSelected(e);
		EXPECT_EQ(q.getSelected(), v);

		e = Entity(IF, Synonym{ "test14151" });
		v.push_back(e);
		q.addSelected(e);
		EXPECT_EQ(q.getSelected(), v);

		e = Entity(WILD, Synonym{ "test123" });
		v.push_back(e);
		q.addSelected(e);
		EXPECT_EQ(q.getSelected(), v);

		e = Entity(READ, Synonym{ "test123" });
		v.push_back(e);
		q.addSelected(e);
		EXPECT_EQ(q.getSelected(), v);
	}
}