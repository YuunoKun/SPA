#include "pch.h"

#include "QueryPreprocessor.h"
#include "Entity.h"
#include "RelRef.h"

namespace UnitTesting {
	TEST(parse, addSingleStatement) {
		QueryPreprocessor qp;
		Query test = qp.parse("stmt s;");

		Query q;
		q.addEntity(Entity(EntityType::STMT, Synonym{ "s" }));

		EXPECT_EQ(test.getEntities(), q.getEntities());

		q.addEntity(Entity(EntityType::STMT, Synonym{ "s1" }));

		EXPECT_NE(test.getEntities(), q.getEntities());
	}

	TEST(parse, addMultipleStatements) {
		QueryPreprocessor qp;
		Query test = qp.parse("stmt s, s1;");

		Query q;
		q.addEntity(Entity(EntityType::STMT, Synonym{ "s" }));
		q.addEntity(Entity(EntityType::STMT, Synonym{ "s1" }));

		EXPECT_EQ(test.getEntities(), q.getEntities());

		q.addEntity(Entity(EntityType::STMT, Synonym{ "s2" }));
		EXPECT_NE(test.getEntities(), q.getEntities());
	}

	TEST(parse, addSingleProcedure) {
		QueryPreprocessor qp;
		Query test = qp.parse("procedure p;");

		Query q;
		q.addEntity(Entity(EntityType::PROCEDURE, Synonym{ "p" }));

		EXPECT_EQ(test.getEntities(), q.getEntities());

		q.addEntity(Entity(EntityType::STMT, Synonym{ "p1" }));

		EXPECT_NE(test.getEntities(), q.getEntities());
	}

	TEST(parse, addMultipleProcedures) {
		QueryPreprocessor qp;
		Query test = qp.parse("procedure p, p1;");

		Query q;
		q.addEntity(Entity(EntityType::PROCEDURE, Synonym{ "p" }));
		q.addEntity(Entity(EntityType::PROCEDURE, Synonym{ "p1" }));

		EXPECT_EQ(test.getEntities(), q.getEntities());

		q.addEntity(Entity(EntityType::PROCEDURE, Synonym{ "p2" }));

		EXPECT_NE(test.getEntities(), q.getEntities());
	}

	TEST(parse, addSingleRead) {
		QueryPreprocessor qp;
		Query test = qp.parse("read r;");

		Query q;
		q.addEntity(Entity(EntityType::READ, Synonym{ "r" }));

		EXPECT_EQ(test.getEntities(), q.getEntities());

		q.addEntity(Entity(EntityType::READ, Synonym{ "r1" }));

		EXPECT_NE(test.getEntities(), q.getEntities());
	}

	TEST(parse, addMultipleReads) {
		QueryPreprocessor qp;
		Query test = qp.parse("read r, r1;");

		Query q;
		q.addEntity(Entity(EntityType::READ, Synonym{ "r" }));
		q.addEntity(Entity(EntityType::READ, Synonym{ "r1" }));

		EXPECT_EQ(test.getEntities(), q.getEntities());

		q.addEntity(Entity(EntityType::READ, Synonym{ "r2" }));

		EXPECT_NE(test.getEntities(), q.getEntities());
	}

	TEST(parse, addSinglePrint) {
		QueryPreprocessor qp;
		Query test = qp.parse("print pr;");

		Query q;
		q.addEntity(Entity(EntityType::PRINT, Synonym{ "pr" }));

		EXPECT_EQ(test.getEntities(), q.getEntities());

		q.addEntity(Entity(EntityType::PRINT, Synonym{ "pr1" }));

		EXPECT_NE(test.getEntities(), q.getEntities());
	}

	TEST(parse, addMultiplePrints) {
		QueryPreprocessor qp;
		Query test = qp.parse("print pr, pr1;");

		Query q;
		q.addEntity(Entity(EntityType::PRINT, Synonym{ "pr" }));
		q.addEntity(Entity(EntityType::PRINT, Synonym{ "pr1" }));

		EXPECT_EQ(test.getEntities(), q.getEntities());

		q.addEntity(Entity(EntityType::PRINT, Synonym{ "pr2" }));

		EXPECT_NE(test.getEntities(), q.getEntities());
	}

	TEST(parse, addSingleCall) {
		QueryPreprocessor qp;
		Query test = qp.parse("call c;");

		Query q;
		q.addEntity(Entity(EntityType::CALL, Synonym{ "c" }));

		EXPECT_EQ(test.getEntities(), q.getEntities());

		q.addEntity(Entity(EntityType::CALL, Synonym{ "c1" }));

		EXPECT_NE(test.getEntities(), q.getEntities());
	}

	TEST(parse, addMultipleCalls) {
		QueryPreprocessor qp;
		Query test = qp.parse("call c, c1;");

		Query q;
		q.addEntity(Entity(EntityType::CALL, Synonym{ "c" }));
		q.addEntity(Entity(EntityType::CALL, Synonym{ "c1" }));

		EXPECT_EQ(test.getEntities(), q.getEntities());

		q.addEntity(Entity(EntityType::CALL, Synonym{ "c2" }));

		EXPECT_NE(test.getEntities(), q.getEntities());
	}

	TEST(parse, addSingleWhile) {
		QueryPreprocessor qp;
		Query test = qp.parse("while w;");

		Query q;
		q.addEntity(Entity(EntityType::WHILE, Synonym{ "w" }));

		EXPECT_EQ(test.getEntities(), q.getEntities());

		q.addEntity(Entity(EntityType::WHILE, Synonym{ "w1" }));

		EXPECT_NE(test.getEntities(), q.getEntities());
	}

	TEST(parse, addMultipleWhiles) {
		QueryPreprocessor qp;
		Query test = qp.parse("while w, w1;");

		Query q;
		q.addEntity(Entity(EntityType::WHILE, Synonym{ "w" }));
		q.addEntity(Entity(EntityType::WHILE, Synonym{ "w1" }));

		EXPECT_EQ(test.getEntities(), q.getEntities());

		q.addEntity(Entity(EntityType::WHILE, Synonym{ "w2" }));

		EXPECT_NE(test.getEntities(), q.getEntities());
	}

	TEST(parse, addSingleIf) {
		QueryPreprocessor qp;
		Query test = qp.parse("if if;");

		Query q;
		q.addEntity(Entity(EntityType::IF, Synonym{ "if" }));

		EXPECT_EQ(test.getEntities(), q.getEntities());

		q.addEntity(Entity(EntityType::IF, Synonym{ "if1" }));

		EXPECT_NE(test.getEntities(), q.getEntities());
	}

	TEST(parse, addMultipleIfs) {
		QueryPreprocessor qp;
		Query test = qp.parse("if if, if1;");

		Query q;
		q.addEntity(Entity(EntityType::IF, Synonym{ "if" }));
		q.addEntity(Entity(EntityType::IF, Synonym{ "if1" }));

		EXPECT_EQ(test.getEntities(), q.getEntities());

		q.addEntity(Entity(EntityType::IF, Synonym{ "if2" }));

		EXPECT_NE(test.getEntities(), q.getEntities());
	}

	TEST(parse, addSingleAssign) {
		QueryPreprocessor qp;
		Query test = qp.parse("assign a;");

		Query q;
		q.addEntity(Entity(EntityType::ASSIGN, Synonym{ "a" }));

		EXPECT_EQ(test.getEntities(), q.getEntities());

		q.addEntity(Entity(EntityType::ASSIGN, Synonym{ "a1" }));

		EXPECT_NE(test.getEntities(), q.getEntities());
	}

	TEST(parse, addMultipleAssigns) {
		QueryPreprocessor qp;
		Query test = qp.parse("assign a, a1;");

		Query q;
		q.addEntity(Entity(EntityType::ASSIGN, Synonym{ "a" }));
		q.addEntity(Entity(EntityType::ASSIGN, Synonym{ "a1" }));

		EXPECT_EQ(test.getEntities(), q.getEntities());

		q.addEntity(Entity(EntityType::ASSIGN, Synonym{ "a2" }));

		EXPECT_NE(test.getEntities(), q.getEntities());
	}

	TEST(parse, addSingleVariable) {
		QueryPreprocessor qp;
		Query test = qp.parse("variable v;");

		Query q;
		q.addEntity(Entity(EntityType::VARIABLE, Synonym{ "v" }));

		EXPECT_EQ(test.getEntities(), q.getEntities());

		q.addEntity(Entity(EntityType::VARIABLE, Synonym{ "v1" }));

		EXPECT_NE(test.getEntities(), q.getEntities());
	}

	TEST(parse, addMultipleVariables) {
		QueryPreprocessor qp;
		Query test = qp.parse("variable v, v1;");

		Query q;
		q.addEntity(Entity(EntityType::VARIABLE, Synonym{ "v" }));
		q.addEntity(Entity(EntityType::VARIABLE, Synonym{ "v1" }));

		EXPECT_EQ(test.getEntities(), q.getEntities());

		q.addEntity(Entity(EntityType::VARIABLE, Synonym{ "v2" }));

		EXPECT_NE(test.getEntities(), q.getEntities());
	}

	TEST(parse, addSingleConstant) {
		QueryPreprocessor qp;
		Query test = qp.parse("constant c;");

		Query q;
		q.addEntity(Entity(EntityType::CONSTANT, Synonym{ "c" }));

		EXPECT_EQ(test.getEntities(), q.getEntities());

		q.addEntity(Entity(EntityType::VARIABLE, Synonym{ "c1" }));

		EXPECT_NE(test.getEntities(), q.getEntities());
	}

	TEST(parse, addMultipleConstants) {
		QueryPreprocessor qp;
		Query test = qp.parse("constant c, c1;");

		Query q;
		q.addEntity(Entity(EntityType::CONSTANT, Synonym{ "c" }));
		q.addEntity(Entity(EntityType::CONSTANT, Synonym{ "c1" }));

		EXPECT_EQ(test.getEntities(), q.getEntities());

		q.addEntity(Entity(EntityType::CONSTANT, Synonym{ "c2" }));

		EXPECT_NE(test.getEntities(), q.getEntities());
	}

	TEST(parse, addMultipleDeclarations) {
		QueryPreprocessor qp;
		Query test = qp.parse("variable v; procedure p;");

		Query q;
		q.addEntity(Entity(EntityType::VARIABLE, Synonym{ "v" }));
		q.addEntity(Entity(EntityType::PROCEDURE, Synonym{ "p" }));

		EXPECT_EQ(test.getEntities(), q.getEntities());

		q.addEntity(Entity(EntityType::PROCEDURE, Synonym{ "p1" }));

		EXPECT_NE(test.getEntities(), q.getEntities());
	}

	TEST(parse, addValidSelect) {
		QueryPreprocessor qp;
		Query test = qp.parse("procedure p; Select p");

		Query q;
		q.addSelected(Entity(EntityType::PROCEDURE, Synonym{ "p" }));

		EXPECT_EQ(test.getSelected(), q.getSelected());
	}

	TEST(parse, oneSuchThatClause) {
		QueryPreprocessor qp;
		Query test = qp.parse("stmt s; Select s such that Follows* (6, s)");

		Query q;
		q.addEntity(Entity(EntityType::STMT, Synonym{ "s" }));
		q.addSelected(Entity(EntityType::STMT, Synonym{ "s" }));
		q.addRelation(RelRef(RelType::FOLLOWS_T, Entity(EntityType::CONSTANT, "6"), Entity(EntityType::STMT, Synonym{ "s" })));

		EXPECT_EQ(test, q);

		Query test2 = qp.parse("variable v; Select v such that Modifies (6, v)");

		Query q2;
		q.addEntity(Entity(EntityType::VARIABLE, Synonym{ "v" }));
		q.addSelected(Entity(EntityType::VARIABLE, Synonym{ "v" }));
		q.addRelation(RelRef(RelType::MODIFIES_S, Entity(EntityType::CONSTANT, "6"), Entity(EntityType::VARIABLE, Synonym{ "v" })));

		EXPECT_EQ(test2, q2);

		//Query test3 = qp.parse("variable v; Select v such that Uses (14, v)");

		//Query q3;
		//q.addEntity(Entity(EntityType::VARIABLE, Synonym{ "v" }));
		//q.addSelected(Entity(EntityType::VARIABLE, Synonym{ "v" }));
		//q.addRelation(RelRef(RelType::USES_S, Entity(EntityType::CONSTANT, "14"), Entity(EntityType::VARIABLE, Synonym{ "v" })));

		//EXPECT_EQ(test3, q3);

		//Query test4 = qp.parse("variable v; procedure p; Select p such that Modifies (p, “x”)");

		//Query q4;
		//q.addEntity(Entity(EntityType::VARIABLE, Synonym{ "v" }));
		//q.addEntity(Entity(EntityType::PROCEDURE, Synonym{ "p" }));
		//q.addSelected(Entity(EntityType::PROCEDURE, Synonym{ "p" }));
		//q.addRelation(RelRef(RelType::MODIFIES_S, Entity(EntityType::PROCEDURE, Synonym{ "p" }), Entity(EntityType::VARIABLE, "x")));

		//EXPECT_EQ(test4, q4);

		//Query test5 = qp.parse("assign a; while w; Select a such that Parent* (w, a)");

		//Query q5;
		//q.addEntity(Entity(EntityType::ASSIGN, Synonym{ "a" }));
		//q.addEntity(Entity(EntityType::WHILE, Synonym{ "w" }));
		//q.addSelected(Entity(EntityType::ASSIGN, Synonym{ "a" }));
		//q.addRelation(RelRef(RelType::PARENT_T, Entity(EntityType::WHILE, Synonym{ "w" }), Entity(EntityType::ASSIGN, Synonym{ "a" })));

		//EXPECT_EQ(test5, q5);
	}
}