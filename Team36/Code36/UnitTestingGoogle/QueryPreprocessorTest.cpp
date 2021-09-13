#include "pch.h"

#include "QueryPreprocessor.h"
#include "Entity.h"

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

	TEST(parse, validAddSinglePrint) {
		QueryPreprocessor qp;
		Query test = qp.parse("print pr;");

		Query q;
		q.addEntity(Entity(EntityType::PRINT, Synonym{ "pr" }));

		EXPECT_EQ(test.getEntities(), q.getEntities());

		q.addEntity(Entity(EntityType::PRINT, Synonym{ "pr1" }));

		EXPECT_NE(test.getEntities(), q.getEntities());
	}

	TEST(parse, validAddMultiplePrints) {
		QueryPreprocessor qp;
		Query test = qp.parse("print pr, pr1;");

		Query q;
		q.addEntity(Entity(EntityType::PRINT, Synonym{ "pr" }));
		q.addEntity(Entity(EntityType::PRINT, Synonym{ "pr1" }));

		EXPECT_EQ(test.getEntities(), q.getEntities());

		q.addEntity(Entity(EntityType::PRINT, Synonym{ "pr2" }));

		EXPECT_NE(test.getEntities(), q.getEntities());
	}

	TEST(parse, validAddSingleCall) {
		QueryPreprocessor qp;
		Query test = qp.parse("call c;");

		Query q;
		q.addEntity(Entity(EntityType::CALL, Synonym{ "c" }));

		EXPECT_EQ(test.getEntities(), q.getEntities());

		q.addEntity(Entity(EntityType::CALL, Synonym{ "c1" }));

		EXPECT_NE(test.getEntities(), q.getEntities());
	}

	TEST(parse, validAddMultipleCalls) {
		QueryPreprocessor qp;
		Query test = qp.parse("call c, c1;");

		Query q;
		q.addEntity(Entity(EntityType::CALL, Synonym{ "c" }));
		q.addEntity(Entity(EntityType::CALL, Synonym{ "c1" }));

		EXPECT_EQ(test.getEntities(), q.getEntities());

		q.addEntity(Entity(EntityType::CALL, Synonym{ "c2" }));

		EXPECT_NE(test.getEntities(), q.getEntities());
	}

	TEST(parse, validAddSingleWhile) {
		QueryPreprocessor qp;
		Query test = qp.parse("while w;");

		Query q;
		q.addEntity(Entity(EntityType::WHILE, Synonym{ "w" }));

		EXPECT_EQ(test.getEntities(), q.getEntities());

		q.addEntity(Entity(EntityType::WHILE, Synonym{ "w1" }));

		EXPECT_NE(test.getEntities(), q.getEntities());
	}

	TEST(parse, validAddMultipleWhiles) {
		QueryPreprocessor qp;
		Query test = qp.parse("while w, w1;");

		Query q;
		q.addEntity(Entity(EntityType::WHILE, Synonym{ "w" }));
		q.addEntity(Entity(EntityType::WHILE, Synonym{ "w1" }));

		EXPECT_EQ(test.getEntities(), q.getEntities());

		q.addEntity(Entity(EntityType::WHILE, Synonym{ "w2" }));

		EXPECT_NE(test.getEntities(), q.getEntities());
	}

	TEST(parse, validAddSingleIf) {
		QueryPreprocessor qp;
		Query test = qp.parse("ifs if;");

		Query q;
		q.addEntity(Entity(EntityType::IF, Synonym{ "if" }));

		EXPECT_EQ(test.getEntities(), q.getEntities());

		q.addEntity(Entity(EntityType::IF, Synonym{ "if1" }));

		EXPECT_NE(test.getEntities(), q.getEntities());
	}

	TEST(parse, validAddMultipleIfs) {
		QueryPreprocessor qp;
		Query test = qp.parse("ifs if, if1;");

		Query q;
		q.addEntity(Entity(EntityType::IF, Synonym{ "if" }));
		q.addEntity(Entity(EntityType::IF, Synonym{ "if1" }));

		EXPECT_EQ(test.getEntities(), q.getEntities());

		q.addEntity(Entity(EntityType::IF, Synonym{ "if2" }));

		EXPECT_NE(test.getEntities(), q.getEntities());
	}

	TEST(parse, validAddSingleAssign) {
		QueryPreprocessor qp;
		Query test = qp.parse("assign a;");

		Query q;
		q.addEntity(Entity(EntityType::ASSIGN, Synonym{ "a" }));

		EXPECT_EQ(test.getEntities(), q.getEntities());

		q.addEntity(Entity(EntityType::ASSIGN, Synonym{ "a1" }));

		EXPECT_NE(test.getEntities(), q.getEntities());
	}

	TEST(parse, validAddMultipleAssigns) {
		QueryPreprocessor qp;
		Query test = qp.parse("assign a, a1;");

		Query q;
		q.addEntity(Entity(EntityType::ASSIGN, Synonym{ "a" }));
		q.addEntity(Entity(EntityType::ASSIGN, Synonym{ "a1" }));

		EXPECT_EQ(test.getEntities(), q.getEntities());

		q.addEntity(Entity(EntityType::ASSIGN, Synonym{ "a2" }));

		EXPECT_NE(test.getEntities(), q.getEntities());
	}

	TEST(parse, validAddSingleVariable) {
		QueryPreprocessor qp;
		Query test = qp.parse("variable v;");

		Query q;
		q.addEntity(Entity(EntityType::VARIABLE, Synonym{ "v" }));

		EXPECT_EQ(test.getEntities(), q.getEntities());

		q.addEntity(Entity(EntityType::VARIABLE, Synonym{ "v1" }));

		EXPECT_NE(test.getEntities(), q.getEntities());
	}

	TEST(parse, validAddMultipleVariables) {
		QueryPreprocessor qp;
		Query test = qp.parse("variable v, v1;");

		Query q;
		q.addEntity(Entity(EntityType::VARIABLE, Synonym{ "v" }));
		q.addEntity(Entity(EntityType::VARIABLE, Synonym{ "v1" }));

		EXPECT_EQ(test.getEntities(), q.getEntities());

		q.addEntity(Entity(EntityType::VARIABLE, Synonym{ "v2" }));

		EXPECT_NE(test.getEntities(), q.getEntities());
	}

	TEST(parse, validAddSingleConstant) {
		QueryPreprocessor qp;
		Query test = qp.parse("constant c;");

		Query q;
		q.addEntity(Entity(EntityType::CONSTANT, Synonym{ "c" }));

		EXPECT_EQ(test.getEntities(), q.getEntities());

		q.addEntity(Entity(EntityType::VARIABLE, Synonym{ "c1" }));

		EXPECT_NE(test.getEntities(), q.getEntities());
	}

	TEST(parse, validAddMultipleConstants) {
		QueryPreprocessor qp;
		Query test = qp.parse("constant c, c1;");

		Query q;
		q.addEntity(Entity(EntityType::CONSTANT, Synonym{ "c" }));
		q.addEntity(Entity(EntityType::CONSTANT, Synonym{ "c1" }));

		EXPECT_EQ(test.getEntities(), q.getEntities());

		q.addEntity(Entity(EntityType::CONSTANT, Synonym{ "c2" }));

		EXPECT_NE(test.getEntities(), q.getEntities());
	}

	TEST(parse, validAddMultipleDeclarations) {
		QueryPreprocessor qp;
		Query test = qp.parse("variable v; procedure p;");

		Query q;
		q.addEntity(Entity(EntityType::VARIABLE, Synonym{ "v" }));
		q.addEntity(Entity(EntityType::PROCEDURE, Synonym{ "p" }));

		EXPECT_EQ(test.getEntities(), q.getEntities());

		q.addEntity(Entity(EntityType::PROCEDURE, Synonym{ "p1" }));

		EXPECT_NE(test.getEntities(), q.getEntities());
	}

	TEST(parse, validAddSelect) {
		//TODO
	}

	TEST(parse, validSuchthatUses) {
		//TODO
	}

	TEST(parse, validSuchthatModifies) {
		//TODO
	}

	TEST(parse, validSuchthatParent) {
		//TODO
	}

	TEST(parse, validSuchthatParentT) {
		//TODO
	}

	TEST(parse, validSuchthatFollows) {
		//TODO
	}

	TEST(parse, validSuchthatFollowsT) {
		//TODO
	}

	TEST(parse, validPattern) {
		//TODO
	}
}