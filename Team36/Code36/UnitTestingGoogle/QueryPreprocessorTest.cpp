#include "pch.h"

#include "QueryPreprocessor.h"
#include "Entity.h"
#include "RelRef.h"

namespace UnitTesting {
	TEST(QueryPreprocessor, addSingleStatement) {
		QueryPreprocessor qp;
		Query test = qp.parse("stmt s; Select s");

		Query q;
		q.addEntity(Entity(EntityType::STMT, Synonym{ "s" }));

		EXPECT_EQ(test.getEntities(), q.getEntities());

		q.addEntity(Entity(EntityType::STMT, Synonym{ "s1" }));

		EXPECT_NE(test.getEntities(), q.getEntities());
	}

	TEST(QueryPreprocessor, addMultipleStatements) {
		QueryPreprocessor qp;
		Query test = qp.parse("stmt s, s1; Select s");

		Query q;
		q.addEntity(Entity(EntityType::STMT, Synonym{ "s" }));
		q.addEntity(Entity(EntityType::STMT, Synonym{ "s1" }));

		EXPECT_EQ(test.getEntities(), q.getEntities());

		q.addEntity(Entity(EntityType::STMT, Synonym{ "s2" }));
		EXPECT_NE(test.getEntities(), q.getEntities());
	}

	TEST(QueryPreprocessor, addSingleProcedure) {
		QueryPreprocessor qp;
		Query test = qp.parse("procedure p; Select p");

		Query q;
		q.addEntity(Entity(EntityType::PROCEDURE, Synonym{ "p" }));

		EXPECT_EQ(test.getEntities(), q.getEntities());

		q.addEntity(Entity(EntityType::STMT, Synonym{ "p1" }));

		EXPECT_NE(test.getEntities(), q.getEntities());
	}

	TEST(QueryPreprocessor, addMultipleProcedures) {
		QueryPreprocessor qp;
		Query test = qp.parse("procedure p, p1; Select p1");

		Query q;
		q.addEntity(Entity(EntityType::PROCEDURE, Synonym{ "p" }));
		q.addEntity(Entity(EntityType::PROCEDURE, Synonym{ "p1" }));

		EXPECT_EQ(test.getEntities(), q.getEntities());

		q.addEntity(Entity(EntityType::PROCEDURE, Synonym{ "p2" }));

		EXPECT_NE(test.getEntities(), q.getEntities());
	}

	TEST(QueryPreprocessor, addSingleRead) {
		QueryPreprocessor qp;
		Query test = qp.parse("read r; Select r");

		Query q;
		q.addEntity(Entity(EntityType::READ, Synonym{ "r" }));

		EXPECT_EQ(test.getEntities(), q.getEntities());

		q.addEntity(Entity(EntityType::READ, Synonym{ "r1" }));

		EXPECT_NE(test.getEntities(), q.getEntities());
	}

	TEST(QueryPreprocessor, addMultipleReads) {
		QueryPreprocessor qp;
		Query test = qp.parse("read r, r1; Select r1");

		Query q;
		q.addEntity(Entity(EntityType::READ, Synonym{ "r" }));
		q.addEntity(Entity(EntityType::READ, Synonym{ "r1" }));

		EXPECT_EQ(test.getEntities(), q.getEntities());

		q.addEntity(Entity(EntityType::READ, Synonym{ "r2" }));

		EXPECT_NE(test.getEntities(), q.getEntities());
	}

	TEST(QueryPreprocessor, addSinglePrint) {
		QueryPreprocessor qp;
		Query test = qp.parse("print pr; Select pr");

		Query q;
		q.addEntity(Entity(EntityType::PRINT, Synonym{ "pr" }));

		EXPECT_EQ(test.getEntities(), q.getEntities());

		q.addEntity(Entity(EntityType::PRINT, Synonym{ "pr1" }));

		EXPECT_NE(test.getEntities(), q.getEntities());
	}

	TEST(QueryPreprocessor, addMultiplePrints) {
		QueryPreprocessor qp;
		Query test = qp.parse("print pr, pr1; Select pr");

		Query q;
		q.addEntity(Entity(EntityType::PRINT, Synonym{ "pr" }));
		q.addEntity(Entity(EntityType::PRINT, Synonym{ "pr1" }));

		EXPECT_EQ(test.getEntities(), q.getEntities());

		q.addEntity(Entity(EntityType::PRINT, Synonym{ "pr2" }));

		EXPECT_NE(test.getEntities(), q.getEntities());
	}

	TEST(QueryPreprocessor, addSingleCall) {
		QueryPreprocessor qp;
		Query test = qp.parse("call c; Select c");

		Query q;
		q.addEntity(Entity(EntityType::CALL, Synonym{ "c" }));

		EXPECT_EQ(test.getEntities(), q.getEntities());

		q.addEntity(Entity(EntityType::CALL, Synonym{ "c1" }));

		EXPECT_NE(test.getEntities(), q.getEntities());
	}

	TEST(QueryPreprocessor, addMultipleCalls) {
		QueryPreprocessor qp;
		Query test = qp.parse("call c, c1; Select c");

		Query q;
		q.addEntity(Entity(EntityType::CALL, Synonym{ "c" }));
		q.addEntity(Entity(EntityType::CALL, Synonym{ "c1" }));

		EXPECT_EQ(test.getEntities(), q.getEntities());

		q.addEntity(Entity(EntityType::CALL, Synonym{ "c2" }));

		EXPECT_NE(test.getEntities(), q.getEntities());
	}

	TEST(QueryPreprocessor, addSingleWhile) {
		QueryPreprocessor qp;
		Query test = qp.parse("while w; Select w");

		Query q;
		q.addEntity(Entity(EntityType::WHILE, Synonym{ "w" }));

		EXPECT_EQ(test.getEntities(), q.getEntities());

		q.addEntity(Entity(EntityType::WHILE, Synonym{ "w1" }));

		EXPECT_NE(test.getEntities(), q.getEntities());
	}

	TEST(QueryPreprocessor, addMultipleWhiles) {
		QueryPreprocessor qp;
		Query test = qp.parse("while w, w1; Select w");

		Query q;
		q.addEntity(Entity(EntityType::WHILE, Synonym{ "w" }));
		q.addEntity(Entity(EntityType::WHILE, Synonym{ "w1" }));

		EXPECT_EQ(test.getEntities(), q.getEntities());

		q.addEntity(Entity(EntityType::WHILE, Synonym{ "w2" }));

		EXPECT_NE(test.getEntities(), q.getEntities());
	}

	TEST(QueryPreprocessor, addSingleIf) {
		QueryPreprocessor qp;
		Query test = qp.parse("if if; Select if");

		Query q;
		q.addEntity(Entity(EntityType::IF, Synonym{ "if" }));

		EXPECT_EQ(test.getEntities(), q.getEntities());

		q.addEntity(Entity(EntityType::IF, Synonym{ "if1" }));

		EXPECT_NE(test.getEntities(), q.getEntities());
	}

	TEST(QueryPreprocessor, addMultipleIfs) {
		QueryPreprocessor qp;
		Query test = qp.parse("if if, if1; Select if");

		Query q;
		q.addEntity(Entity(EntityType::IF, Synonym{ "if" }));
		q.addEntity(Entity(EntityType::IF, Synonym{ "if1" }));

		EXPECT_EQ(test.getEntities(), q.getEntities());

		q.addEntity(Entity(EntityType::IF, Synonym{ "if2" }));

		EXPECT_NE(test.getEntities(), q.getEntities());
	}

	TEST(QueryPreprocessor, addSingleAssign) {
		QueryPreprocessor qp;
		Query test = qp.parse("assign a; Select a");

		Query q;
		q.addEntity(Entity(EntityType::ASSIGN, Synonym{ "a" }));

		EXPECT_EQ(test.getEntities(), q.getEntities());

		q.addEntity(Entity(EntityType::ASSIGN, Synonym{ "a1" }));

		EXPECT_NE(test.getEntities(), q.getEntities());
	}

	TEST(QueryPreprocessor, addMultipleAssigns) {
		QueryPreprocessor qp;
		Query test = qp.parse("assign a, a1; Select a");

		Query q;
		q.addEntity(Entity(EntityType::ASSIGN, Synonym{ "a" }));
		q.addEntity(Entity(EntityType::ASSIGN, Synonym{ "a1" }));

		EXPECT_EQ(test.getEntities(), q.getEntities());

		q.addEntity(Entity(EntityType::ASSIGN, Synonym{ "a2" }));

		EXPECT_NE(test.getEntities(), q.getEntities());
	}

	TEST(QueryPreprocessor, addSingleVariable) {
		QueryPreprocessor qp;
		Query test = qp.parse("variable v; Select v");

		Query q;
		q.addEntity(Entity(EntityType::VARIABLE, Synonym{ "v" }));

		EXPECT_EQ(test.getEntities(), q.getEntities());

		q.addEntity(Entity(EntityType::VARIABLE, Synonym{ "v1" }));

		EXPECT_NE(test.getEntities(), q.getEntities());
	}

	TEST(QueryPreprocessor, addMultipleVariables) {
		QueryPreprocessor qp;
		Query test = qp.parse("variable v, v1; Select v");

		Query q;
		q.addEntity(Entity(EntityType::VARIABLE, Synonym{ "v" }));
		q.addEntity(Entity(EntityType::VARIABLE, Synonym{ "v1" }));

		EXPECT_EQ(test.getEntities(), q.getEntities());

		q.addEntity(Entity(EntityType::VARIABLE, Synonym{ "v2" }));

		EXPECT_NE(test.getEntities(), q.getEntities());
	}

	TEST(QueryPreprocessor, addSingleConstant) {
		QueryPreprocessor qp;
		Query test = qp.parse("constant c; Select c");

		Query q;
		q.addEntity(Entity(EntityType::CONSTANT, Synonym{ "c" }));

		EXPECT_EQ(test.getEntities(), q.getEntities());

		q.addEntity(Entity(EntityType::VARIABLE, Synonym{ "c1" }));

		EXPECT_NE(test.getEntities(), q.getEntities());
	}

	TEST(QueryPreprocessor, addMultipleConstants) {
		QueryPreprocessor qp;
		Query test = qp.parse("constant c, c1; Select c");

		Query q;
		q.addEntity(Entity(EntityType::CONSTANT, Synonym{ "c" }));
		q.addEntity(Entity(EntityType::CONSTANT, Synonym{ "c1" }));

		EXPECT_EQ(test.getEntities(), q.getEntities());

		q.addEntity(Entity(EntityType::CONSTANT, Synonym{ "c2" }));

		EXPECT_NE(test.getEntities(), q.getEntities());
	}

	TEST(QueryPreprocessor, addMultipleDeclarations) {
		QueryPreprocessor qp;
		Query test = qp.parse("variable v; procedure p; Select v");

		Query q;
		q.addEntity(Entity(EntityType::VARIABLE, Synonym{ "v" }));
		q.addEntity(Entity(EntityType::PROCEDURE, Synonym{ "p" }));

		EXPECT_EQ(test.getEntities(), q.getEntities());

		q.addEntity(Entity(EntityType::PROCEDURE, Synonym{ "p1" }));

		EXPECT_NE(test.getEntities(), q.getEntities());
	}

	TEST(QueryPreprocessor, addValidSelect) {
		QueryPreprocessor qp;
		Query test = qp.parse("procedure p; Select p");

		Query q;
		q.addSelected(Entity(EntityType::PROCEDURE, Synonym{ "p" }));

		EXPECT_EQ(test.getSelected(), q.getSelected());
	}

	TEST(QueryPreprocessor, similarDeclarationName) {
		QueryPreprocessor qp;
		Query test = qp.parse("procedure procedure; if if; stmt stmt; call call; while while; Select procedure");

		Query q;
		q.addEntity(Entity(EntityType::PROCEDURE, Synonym{ "procedure" }));
		q.addEntity(Entity(EntityType::IF, Synonym{ "if" }));
		q.addEntity(Entity(EntityType::STMT, Synonym{ "stmt" }));
		q.addEntity(Entity(EntityType::CALL, Synonym{ "call" }));
		q.addEntity(Entity(EntityType::WHILE, Synonym{ "while" }));

		EXPECT_EQ(test.getEntities(), q.getEntities());
	}

	TEST(QueryPreprocessor, similarSelectName) {
		QueryPreprocessor qp;
		Query test = qp.parse("procedure select; Select select");

		Query q;
		q.addEntity(Entity(EntityType::PROCEDURE, Synonym{ "select" }));
		q.addSelected(Entity(EntityType::PROCEDURE, Synonym{ "select" }));

		EXPECT_EQ(test, q);
	}

	TEST(QueryPreprocessor, oneSuchThatClause) {
		QueryPreprocessor qp;
		Query test = qp.parse("stmt s; Select s such that Follows* (6, s)");

		Query q = Query();
		q.addEntity(Entity(EntityType::STMT, Synonym{ "s" }));
		q.addSelected(Entity(EntityType::STMT, Synonym{ "s" }));
		q.addRelation(RelRef(RelType::FOLLOWS_T, Entity(EntityType::CONSTANT, "6"), Entity(EntityType::STMT, Synonym{ "s" })));

		EXPECT_EQ(test, q);

		Query test2 = qp.parse("variable v; Select v such that Modifies (6, v)");

		Query q2 = Query();
		q2.addEntity(Entity(EntityType::VARIABLE, Synonym{ "v" }));
		q2.addSelected(Entity(EntityType::VARIABLE, Synonym{ "v" }));
		q2.addRelation(RelRef(RelType::MODIFIES_S, Entity(EntityType::CONSTANT, "6"), Entity(EntityType::VARIABLE, Synonym{ "v" })));

		EXPECT_EQ(test2, q2);

		Query test3 = qp.parse("variable v; Select v such that Uses (14, v)");

		Query q3 = Query();
		q3.addEntity(Entity(EntityType::VARIABLE, Synonym{ "v" }));
		q3.addSelected(Entity(EntityType::VARIABLE, Synonym{ "v" }));
		q3.addRelation(RelRef(RelType::USES_S, Entity(EntityType::CONSTANT, "14"), Entity(EntityType::VARIABLE, Synonym{ "v" })));

		EXPECT_EQ(test3, q3);

		Query test4 = qp.parse("variable v; procedure p; Select p such that Modifies (p, \"x\")");

		Query q4 = Query();
		q4.addEntity(Entity(EntityType::VARIABLE, Synonym{ "v" }));
		q4.addEntity(Entity(EntityType::PROCEDURE, Synonym{ "p" }));
		q4.addSelected(Entity(EntityType::PROCEDURE, Synonym{ "p" }));
		q4.addRelation(RelRef(RelType::MODIFIES_S, Entity(EntityType::PROCEDURE, Synonym{ "p" }), Entity(EntityType::VARIABLE, "x")));

		EXPECT_EQ(test4, q4);

		Query test5 = qp.parse("assign a; while w; Select a such that Parent* (w, a)");

		Query q5;
		q5.addEntity(Entity(EntityType::ASSIGN, Synonym{ "a" }));
		q5.addEntity(Entity(EntityType::WHILE, Synonym{ "w" }));
		q5.addSelected(Entity(EntityType::ASSIGN, Synonym{ "a" }));
		q5.addRelation(RelRef(RelType::PARENT_T, Entity(EntityType::WHILE, Synonym{ "w" }), Entity(EntityType::ASSIGN, Synonym{ "a" })));

		EXPECT_EQ(test5, q5);

		Query test6 = qp.parse("read r; variable v; Select r such that Modifies(r, v)");

		Query q6;
		q6.addEntity(Entity(EntityType::READ, Synonym{ "r" }));
		q6.addEntity(Entity(EntityType::VARIABLE, Synonym{ "v" }));
		q6.addSelected(Entity(EntityType::READ, Synonym{ "r" }));
		q6.addRelation(RelRef(RelType::MODIFIES_S, Entity(EntityType::READ, Synonym{ "r" }), Entity(EntityType::VARIABLE, Synonym{ "v" })));

		EXPECT_EQ(test6, q6);

		Query test7 = qp.parse("while w; Select w such that Modifies(w,_)");

		Query q7;
		q7.addEntity(Entity(EntityType::WHILE, Synonym{ "w" }));
		q7.addSelected(Entity(EntityType::WHILE, Synonym{ "w" }));
		q7.addRelation(RelRef(RelType::MODIFIES_S, Entity(EntityType::WHILE, Synonym{ "w" }), Entity(EntityType::WILD, Synonym{ "" })));

		EXPECT_EQ(test7, q7);

		Query test8 = qp.parse("variable v; Select v such that Modifies(_, v)");

		Query q8;
		q8.addEntity(Entity(EntityType::VARIABLE, Synonym{ "v" }));
		q8.addSelected(Entity(EntityType::VARIABLE, Synonym{ "v" }));
		q8.addRelation(RelRef(RelType::MODIFIES_S, Entity(EntityType::WILD, Synonym{ "" }), Entity(EntityType::VARIABLE, Synonym{ "v" })));

		EXPECT_EQ(test8, q8);

		Query test9 = qp.parse("stmt s; Select s such that Parent(1, 2)");

		Query q9;
		q9.addEntity(Entity(EntityType::STMT, Synonym{ "s" }));
		q9.addSelected(Entity(EntityType::STMT, Synonym{ "s" }));
		q9.addRelation(RelRef(RelType::PARENT, Entity(EntityType::CONSTANT, "1"), Entity(EntityType::CONSTANT, "2")));

		EXPECT_EQ(test9, q9);

		Query test10 = qp.parse("stmt s; Select s such that Follows(1, _)");

		Query q10;
		q10.addEntity(Entity(EntityType::STMT, Synonym{ "s" }));
		q10.addSelected(Entity(EntityType::STMT, Synonym{ "s" }));
		q10.addRelation(RelRef(RelType::FOLLOWS, Entity(EntityType::CONSTANT, "1"), Entity(EntityType::WILD, "")));

		EXPECT_EQ(test10, q10);

		Query test11 = qp.parse("stmt s; Select s such that Follows(_, 1)");

		Query q11;
		q11.addEntity(Entity(EntityType::STMT, Synonym{ "s" }));
		q11.addSelected(Entity(EntityType::STMT, Synonym{ "s" }));
		q11.addRelation(RelRef(RelType::FOLLOWS, Entity(EntityType::WILD, ""), Entity(EntityType::CONSTANT, "1")));

		EXPECT_EQ(test11, q11);

		Query test12 = qp.parse("stmt s; Select s such that Follows(_, _)");

		Query q12;
		q12.addEntity(Entity(EntityType::STMT, Synonym{ "s" }));
		q12.addSelected(Entity(EntityType::STMT, Synonym{ "s" }));
		q12.addRelation(RelRef(RelType::FOLLOWS, Entity(EntityType::WILD, ""), Entity(EntityType::WILD, "")));

		EXPECT_EQ(test12, q12);
	}

	TEST(QueryPreprocessor, onePatternClause) {
		QueryPreprocessor qp;
		Query test = qp.parse("assign a; Select a pattern a(\"y\", _)");

		Query q;
		q.addEntity(Entity(EntityType::ASSIGN, Synonym{ "a" }));
		q.addSelected(Entity(EntityType::ASSIGN, Synonym{ "a" }));
		q.addPattern(Pattern(Entity(EntityType::ASSIGN, Synonym{ "a" }), Entity(EntityType::VARIABLE, "y"), "", true));

		EXPECT_EQ(test, q);

		Query test2 = qp.parse("assign a; Select a pattern a(\"y\", _\"z\"_)");

		Query q2;
		q2.addEntity(Entity(EntityType::ASSIGN, Synonym{ "a" }));
		q2.addSelected(Entity(EntityType::ASSIGN, Synonym{ "a" }));
		q2.addPattern(Pattern(Entity(EntityType::ASSIGN, Synonym{ "a" }), Entity(EntityType::VARIABLE, "y"), "z", true));

		EXPECT_EQ(test2, q2);

		Query test3 = qp.parse("assign a; Select a pattern a(\"x\", _\"1\"_)");

		Query q3;
		q3.addEntity(Entity(EntityType::ASSIGN, Synonym{ "a" }));
		q3.addSelected(Entity(EntityType::ASSIGN, Synonym{ "a" }));
		q3.addPattern(Pattern(Entity(EntityType::ASSIGN, Synonym{ "a" }), Entity(EntityType::VARIABLE, "x"), "1", true));

		EXPECT_EQ(test3, q3);

		Query test4 = qp.parse("assign a; Select a pattern a(_,_)");

		Query q4;
		q4.addEntity(Entity(EntityType::ASSIGN, Synonym{ "a" }));
		q4.addSelected(Entity(EntityType::ASSIGN, Synonym{ "a" }));
		q4.addPattern(Pattern(Entity(EntityType::ASSIGN, Synonym{ "a" }), Entity(EntityType::WILD, ""), "", true));

		EXPECT_EQ(test4, q4);

		Query test5 = qp.parse("assign a; Select a pattern a(_, _\"x\"_)");

		Query q5;
		q5.addEntity(Entity(EntityType::ASSIGN, Synonym{ "a" }));
		q5.addSelected(Entity(EntityType::ASSIGN, Synonym{ "a" }));
		q5.addPattern(Pattern(Entity(EntityType::ASSIGN, Synonym{ "a" }), Entity(EntityType::WILD, ""), "x", true));

		EXPECT_EQ(test5, q5);

		Query test6 = qp.parse("assign a; variable v; Select v pattern a(v, _\"x\"_)");

		Query q6;
		q6.addEntity(Entity(EntityType::ASSIGN, Synonym{ "a" }));
		q6.addEntity(Entity(EntityType::VARIABLE, Synonym{ "v" }));
		q6.addSelected(Entity(EntityType::VARIABLE, Synonym{ "v" }));
		q6.addPattern(Pattern(Entity(EntityType::ASSIGN, Synonym{ "a" }), Entity(EntityType::VARIABLE, Synonym{ "v" }), "x", true));

		EXPECT_EQ(test6, q6);

		Query test7 = qp.parse("assign a; variable v; Select v pattern a(v, \"x\")");

		Query q7;
		q7.addEntity(Entity(EntityType::ASSIGN, Synonym{ "a" }));
		q7.addEntity(Entity(EntityType::VARIABLE, Synonym{ "v" }));
		q7.addSelected(Entity(EntityType::VARIABLE, Synonym{ "v" }));
		q7.addPattern(Pattern(Entity(EntityType::ASSIGN, Synonym{ "a" }), Entity(EntityType::VARIABLE, Synonym{ "v" }), "x", false));

		EXPECT_EQ(test7, q7);
	}

	TEST(QueryPreprocessor, oneSuchThatAndOnePatternClauseModifies) {
		QueryPreprocessor qp;
		Query test = qp.parse("assign a, a1; Select a such that Modifies(a, \"x\") pattern a1(\"y\", _\"x\"_)");

		Query q;
		q.addEntity(Entity(EntityType::ASSIGN, Synonym{ "a" }));
		q.addEntity(Entity(EntityType::ASSIGN, Synonym{ "a1" }));
		q.addSelected(Entity(EntityType::ASSIGN, Synonym{ "a" }));
		q.addRelation(RelRef(RelType::MODIFIES_S, Entity(EntityType::ASSIGN, Synonym{ "a" }), Entity(EntityType::VARIABLE, "x")));
		q.addPattern(Pattern(Entity(EntityType::ASSIGN, Synonym{ "a1" }), Entity(EntityType::VARIABLE, "y"), "x", true));

		EXPECT_EQ(test, q);

		Query test2 = qp.parse("assign a; variable v; Select a such that Modifies(6, \"z\") pattern a(v, _);");

		Query q2;
		q2.addEntity(Entity(EntityType::ASSIGN, Synonym{ "a" }));
		q2.addEntity(Entity(EntityType::VARIABLE, Synonym{ "v" }));
		q2.addSelected(Entity(EntityType::ASSIGN, Synonym{ "a" }));
		q2.addRelation(RelRef(RelType::MODIFIES_S, Entity(EntityType::CONSTANT, "6"), Entity(EntityType::VARIABLE, "z")));
		q2.addPattern(Pattern(Entity(EntityType::ASSIGN, Synonym{ "a" }), Entity(EntityType::VARIABLE, Synonym{ "v" }), "", true));

		EXPECT_EQ(test2, q2);

		Query test3 = qp.parse("assign a; variable v; Select a such that Modifies(15, \"p\") pattern a(v, _\"y\"_)");

		Query q3;
		q3.addEntity(Entity(EntityType::ASSIGN, Synonym{ "a" }));
		q3.addEntity(Entity(EntityType::VARIABLE, Synonym{ "v" }));
		q3.addSelected(Entity(EntityType::ASSIGN, Synonym{ "a" }));
		q3.addRelation(RelRef(RelType::MODIFIES_S, Entity(EntityType::CONSTANT, "15"), Entity(EntityType::VARIABLE, "p")));
		q3.addPattern(Pattern(Entity(EntityType::ASSIGN, Synonym{ "a" }), Entity(EntityType::VARIABLE, Synonym{ "v" }), "y", true));

		EXPECT_EQ(test3, q3);

		Query test4 = qp.parse("assign a, a1; variable v; Select v such that Modifies(a, \"q\") pattern a1(v, \"p\")");

		Query q4;
		q4.addEntity(Entity(EntityType::ASSIGN, Synonym{ "a" }));
		q4.addEntity(Entity(EntityType::ASSIGN, Synonym{ "a1" }));
		q4.addEntity(Entity(EntityType::VARIABLE, Synonym{ "v" }));
		q4.addSelected(Entity(EntityType::VARIABLE, Synonym{ "v" }));
		q4.addRelation(RelRef(RelType::MODIFIES_S, Entity(EntityType::ASSIGN, Synonym{ "a" }), Entity(EntityType::VARIABLE, "q")));
		q4.addPattern(Pattern(Entity(EntityType::ASSIGN, Synonym{ "a1" }), Entity(EntityType::VARIABLE, Synonym{ "v" }), "p", false));

		EXPECT_EQ(test4, q4);

		Query test5 = qp.parse("assign a, a1; variable v, v1; Select v1 such that Modifies(a, v) pattern a1(v1, _\"5\"_)");

		Query q5;
		q5.addEntity(Entity(EntityType::ASSIGN, Synonym{ "a" }));
		q5.addEntity(Entity(EntityType::ASSIGN, Synonym{ "a1" }));
		q5.addEntity(Entity(EntityType::VARIABLE, Synonym{ "v" }));
		q5.addEntity(Entity(EntityType::VARIABLE, Synonym{ "v1" }));
		q5.addSelected(Entity(EntityType::VARIABLE, Synonym{ "v1" }));
		q5.addRelation(RelRef(RelType::MODIFIES_S, Entity(EntityType::ASSIGN, Synonym{ "a" }), Entity(EntityType::VARIABLE, Synonym{ "v" })));
		q5.addPattern(Pattern(Entity(EntityType::ASSIGN, Synonym{ "a1" }), Entity(EntityType::VARIABLE, Synonym{ "v1" }), "5", true));

		EXPECT_EQ(test5, q5);
	}

	TEST(QueryPreprocessor, oneSuchThatAndOnePatternClauseUses) {
		QueryPreprocessor qp;
		Query test = qp.parse("assign a, a1; Select a such that Uses(a, \"x\") pattern a1(\"y\", _\"x\"_)");

		Query q;
		q.addEntity(Entity(EntityType::ASSIGN, Synonym{ "a" }));
		q.addEntity(Entity(EntityType::ASSIGN, Synonym{ "a1" }));
		q.addSelected(Entity(EntityType::ASSIGN, Synonym{ "a" }));
		q.addRelation(RelRef(RelType::USES_S, Entity(EntityType::ASSIGN, Synonym{ "a" }), Entity(EntityType::VARIABLE, "x")));
		q.addPattern(Pattern(Entity(EntityType::ASSIGN, Synonym{ "a1" }), Entity(EntityType::VARIABLE, "y"), "x", true));

		EXPECT_EQ(test, q);

		Query test2 = qp.parse("assign a; variable v; Select a such that Uses(6, \"z\") pattern a(v, _);");

		Query q2;
		q2.addEntity(Entity(EntityType::ASSIGN, Synonym{ "a" }));
		q2.addEntity(Entity(EntityType::VARIABLE, Synonym{ "v" }));
		q2.addSelected(Entity(EntityType::ASSIGN, Synonym{ "a" }));
		q2.addRelation(RelRef(RelType::USES_S, Entity(EntityType::CONSTANT, "6"), Entity(EntityType::VARIABLE, "z")));
		q2.addPattern(Pattern(Entity(EntityType::ASSIGN, Synonym{ "a" }), Entity(EntityType::VARIABLE, Synonym{ "v" }), "", true));

		EXPECT_EQ(test2, q2);

		Query test3 = qp.parse("assign a; variable v; Select a such that Uses(15, \"p\") pattern a(v, _\"y\"_)");

		Query q3;
		q3.addEntity(Entity(EntityType::ASSIGN, Synonym{ "a" }));
		q3.addEntity(Entity(EntityType::VARIABLE, Synonym{ "v" }));
		q3.addSelected(Entity(EntityType::ASSIGN, Synonym{ "a" }));
		q3.addRelation(RelRef(RelType::USES_S, Entity(EntityType::CONSTANT, "15"), Entity(EntityType::VARIABLE, "p")));
		q3.addPattern(Pattern(Entity(EntityType::ASSIGN, Synonym{ "a" }), Entity(EntityType::VARIABLE, Synonym{ "v" }), "y", true));

		EXPECT_EQ(test3, q3);

		Query test4 = qp.parse("assign a, a1; variable v; Select v such that Uses(a, \"q\") pattern a1(v, \"p\")");

		Query q4;
		q4.addEntity(Entity(EntityType::ASSIGN, Synonym{ "a" }));
		q4.addEntity(Entity(EntityType::ASSIGN, Synonym{ "a1" }));
		q4.addEntity(Entity(EntityType::VARIABLE, Synonym{ "v" }));
		q4.addSelected(Entity(EntityType::VARIABLE, Synonym{ "v" }));
		q4.addRelation(RelRef(RelType::USES_S, Entity(EntityType::ASSIGN, Synonym{ "a" }), Entity(EntityType::VARIABLE, "q")));
		q4.addPattern(Pattern(Entity(EntityType::ASSIGN, Synonym{ "a1" }), Entity(EntityType::VARIABLE, Synonym{ "v" }), "p", false));

		EXPECT_EQ(test4, q4);

		Query test5 = qp.parse("assign a, a1; variable v, v1; Select v1 such that Uses(a, v) pattern a1(v1, _\"5\"_)");

		Query q5;
		q5.addEntity(Entity(EntityType::ASSIGN, Synonym{ "a" }));
		q5.addEntity(Entity(EntityType::ASSIGN, Synonym{ "a1" }));
		q5.addEntity(Entity(EntityType::VARIABLE, Synonym{ "v" }));
		q5.addEntity(Entity(EntityType::VARIABLE, Synonym{ "v1" }));
		q5.addSelected(Entity(EntityType::VARIABLE, Synonym{ "v1" }));
		q5.addRelation(RelRef(RelType::USES_S, Entity(EntityType::ASSIGN, Synonym{ "a" }), Entity(EntityType::VARIABLE, Synonym{ "v" })));
		q5.addPattern(Pattern(Entity(EntityType::ASSIGN, Synonym{ "a1" }), Entity(EntityType::VARIABLE, Synonym{ "v1" }), "5", true));

		EXPECT_EQ(test5, q5);
	}

	TEST(QueryPreprocessor, oneSuchThatAndOnePatternClauseFollows) {
		QueryPreprocessor qp;
		Query test = qp.parse("stmt s; assign a, a1; variable v; Select a such that Follows(s, a) pattern a1(v, _)");

		Query q;
		q.addEntity(Entity(EntityType::STMT, Synonym{ "s" }));
		q.addEntity(Entity(EntityType::ASSIGN, Synonym{ "a" }));
		q.addEntity(Entity(EntityType::ASSIGN, Synonym{ "a1" }));
		q.addEntity(Entity(EntityType::VARIABLE, Synonym{ "v" }));
		q.addSelected(Entity(EntityType::ASSIGN, Synonym{ "a" }));
		q.addRelation(RelRef(RelType::FOLLOWS, Entity(EntityType::STMT, Synonym{ "s" }), Entity(EntityType::ASSIGN, Synonym{ "a" })));
		q.addPattern(Pattern(Entity(EntityType::ASSIGN, Synonym{ "a1" }), Entity(EntityType::VARIABLE, Synonym{ "v" }), "", true));

		EXPECT_EQ(test, q);

		Query test2 = qp.parse("assign a; stmt s; Select a such that Follows(s, 9) pattern a(\"q\", _\"p\"_)");

		Query q2;
		q2.addEntity(Entity(EntityType::ASSIGN, Synonym{ "a" }));
		q2.addEntity(Entity(EntityType::STMT, Synonym{ "s" }));
		q2.addSelected(Entity(EntityType::ASSIGN, Synonym{ "a" }));
		q2.addRelation(RelRef(RelType::FOLLOWS, Entity(EntityType::STMT, Synonym{ "s" }), Entity(EntityType::CONSTANT, "9")));
		q2.addPattern(Pattern(Entity(EntityType::ASSIGN, Synonym{ "a" }), Entity(EntityType::VARIABLE, "q"), "p", true));

		EXPECT_EQ(test2, q2);

		Query test3 = qp.parse("assign a, a1; stmt s; Select a such that Follows(s, a) pattern a1(\"i\", \"10\")");

		Query q3;
		q3.addEntity(Entity(EntityType::ASSIGN, Synonym{ "a" }));
		q3.addEntity(Entity(EntityType::ASSIGN, Synonym{ "a1" }));
		q3.addEntity(Entity(EntityType::STMT, Synonym{ "s" }));
		q3.addSelected(Entity(EntityType::ASSIGN, Synonym{ "a" }));
		q3.addRelation(RelRef(RelType::FOLLOWS, Entity(EntityType::STMT, Synonym{ "s" }), Entity(EntityType::ASSIGN, Synonym{ "a" })));
		q3.addPattern(Pattern(Entity(EntityType::ASSIGN, Synonym{ "a1" }), Entity(EntityType::VARIABLE, "i"), "10", false));

		EXPECT_EQ(test3, q3);
	}

	TEST(QueryPreprocessor, oneSuchThatAndOnePatternClauseFollowsT) {
		QueryPreprocessor qp;
		Query test = qp.parse("stmt s; assign a, a1; variable v; Select a such that Follows*(s, a) pattern a1(v, _)");

		Query q;
		q.addEntity(Entity(EntityType::STMT, Synonym{ "s" }));
		q.addEntity(Entity(EntityType::ASSIGN, Synonym{ "a" }));
		q.addEntity(Entity(EntityType::ASSIGN, Synonym{ "a1" }));
		q.addEntity(Entity(EntityType::VARIABLE, Synonym{ "v" }));
		q.addSelected(Entity(EntityType::ASSIGN, Synonym{ "a" }));
		q.addRelation(RelRef(RelType::FOLLOWS_T, Entity(EntityType::STMT, Synonym{ "s" }), Entity(EntityType::ASSIGN, Synonym{ "a" })));
		q.addPattern(Pattern(Entity(EntityType::ASSIGN, Synonym{ "a1" }), Entity(EntityType::VARIABLE, Synonym{ "v" }), "", true));

		EXPECT_EQ(test, q);

		Query test2 = qp.parse("assign a; stmt s; Select a such that Follows*(s, 9) pattern a(\"q\", _\"p\"_)");

		Query q2;
		q2.addEntity(Entity(EntityType::ASSIGN, Synonym{ "a" }));
		q2.addEntity(Entity(EntityType::STMT, Synonym{ "s" }));
		q2.addSelected(Entity(EntityType::ASSIGN, Synonym{ "a" }));
		q2.addRelation(RelRef(RelType::FOLLOWS_T, Entity(EntityType::STMT, Synonym{ "s" }), Entity(EntityType::CONSTANT, "9")));
		q2.addPattern(Pattern(Entity(EntityType::ASSIGN, Synonym{ "a" }), Entity(EntityType::VARIABLE, "q"), "p", true));

		EXPECT_EQ(test2, q2);

		Query test3 = qp.parse("assign a, a1; stmt s; Select a such that Follows*(s, a) pattern a1(\"i\", \"10\")");

		Query q3;
		q3.addEntity(Entity(EntityType::ASSIGN, Synonym{ "a" }));
		q3.addEntity(Entity(EntityType::ASSIGN, Synonym{ "a1" }));
		q3.addEntity(Entity(EntityType::STMT, Synonym{ "s" }));
		q3.addSelected(Entity(EntityType::ASSIGN, Synonym{ "a" }));
		q3.addRelation(RelRef(RelType::FOLLOWS_T, Entity(EntityType::STMT, Synonym{ "s" }), Entity(EntityType::ASSIGN, Synonym{ "a" })));
		q3.addPattern(Pattern(Entity(EntityType::ASSIGN, Synonym{ "a1" }), Entity(EntityType::VARIABLE, "i"), "10", false));

		EXPECT_EQ(test3, q3);
	}

	TEST(QueryPreprocessor, oneSuchThatAndOnePatternClauseParent) {
		QueryPreprocessor qp;
		Query test = qp.parse("stmt s; assign a, a1; variable v; Select a such that Parent(s, a) pattern a1(v, _)");

		Query q;
		q.addEntity(Entity(EntityType::STMT, Synonym{ "s" }));
		q.addEntity(Entity(EntityType::ASSIGN, Synonym{ "a" }));
		q.addEntity(Entity(EntityType::ASSIGN, Synonym{ "a1" }));
		q.addEntity(Entity(EntityType::VARIABLE, Synonym{ "v" }));
		q.addSelected(Entity(EntityType::ASSIGN, Synonym{ "a" }));
		q.addRelation(RelRef(RelType::PARENT, Entity(EntityType::STMT, Synonym{ "s" }), Entity(EntityType::ASSIGN, Synonym{ "a" })));
		q.addPattern(Pattern(Entity(EntityType::ASSIGN, Synonym{ "a1" }), Entity(EntityType::VARIABLE, Synonym{ "v" }), "", true));

		EXPECT_EQ(test, q);

		Query test2 = qp.parse("assign a; stmt s; Select a such that Parent(s, 9) pattern a(\"q\", _\"p\"_)");

		Query q2;
		q2.addEntity(Entity(EntityType::ASSIGN, Synonym{ "a" }));
		q2.addEntity(Entity(EntityType::STMT, Synonym{ "s" }));
		q2.addSelected(Entity(EntityType::ASSIGN, Synonym{ "a" }));
		q2.addRelation(RelRef(RelType::PARENT, Entity(EntityType::STMT, Synonym{ "s" }), Entity(EntityType::CONSTANT, "9")));
		q2.addPattern(Pattern(Entity(EntityType::ASSIGN, Synonym{ "a" }), Entity(EntityType::VARIABLE, "q"), "p", true));

		EXPECT_EQ(test2, q2);

		Query test3 = qp.parse("assign a, a1; stmt s; Select a such that Parent(s, a) pattern a1(\"i\", \"10\")");

		Query q3;
		q3.addEntity(Entity(EntityType::ASSIGN, Synonym{ "a" }));
		q3.addEntity(Entity(EntityType::ASSIGN, Synonym{ "a1" }));
		q3.addEntity(Entity(EntityType::STMT, Synonym{ "s" }));
		q3.addSelected(Entity(EntityType::ASSIGN, Synonym{ "a" }));
		q3.addRelation(RelRef(RelType::PARENT, Entity(EntityType::STMT, Synonym{ "s" }), Entity(EntityType::ASSIGN, Synonym{ "a" })));
		q3.addPattern(Pattern(Entity(EntityType::ASSIGN, Synonym{ "a1" }), Entity(EntityType::VARIABLE, "i"), "10", false));

		EXPECT_EQ(test3, q3);
	}

	TEST(QueryPreprocessor, oneSuchThatAndOnePatternClauseParentT) {
		QueryPreprocessor qp;
		Query test = qp.parse("stmt s; assign a, a1; variable v; Select a such that Parent*(s, a) pattern a1(v, _)");

		Query q;
		q.addEntity(Entity(EntityType::STMT, Synonym{ "s" }));
		q.addEntity(Entity(EntityType::ASSIGN, Synonym{ "a" }));
		q.addEntity(Entity(EntityType::ASSIGN, Synonym{ "a1" }));
		q.addEntity(Entity(EntityType::VARIABLE, Synonym{ "v" }));
		q.addSelected(Entity(EntityType::ASSIGN, Synonym{ "a" }));
		q.addRelation(RelRef(RelType::PARENT_T, Entity(EntityType::STMT, Synonym{ "s" }), Entity(EntityType::ASSIGN, Synonym{ "a" })));
		q.addPattern(Pattern(Entity(EntityType::ASSIGN, Synonym{ "a1" }), Entity(EntityType::VARIABLE, Synonym{ "v" }), "", true));

		EXPECT_EQ(test, q);

		Query test2 = qp.parse("assign a; stmt s; Select a such that Parent*(s, 9) pattern a(\"q\", _\"p\"_)");

		Query q2;
		q2.addEntity(Entity(EntityType::ASSIGN, Synonym{ "a" }));
		q2.addEntity(Entity(EntityType::STMT, Synonym{ "s" }));
		q2.addSelected(Entity(EntityType::ASSIGN, Synonym{ "a" }));
		q2.addRelation(RelRef(RelType::PARENT_T, Entity(EntityType::STMT, Synonym{ "s" }), Entity(EntityType::CONSTANT, "9")));
		q2.addPattern(Pattern(Entity(EntityType::ASSIGN, Synonym{ "a" }), Entity(EntityType::VARIABLE, "q"), "p", true));

		EXPECT_EQ(test2, q2);

		Query test3 = qp.parse("assign a, a1; stmt s; Select a such that Parent*(s, a) pattern a1(\"i\", \"10\")");

		Query q3;
		q3.addEntity(Entity(EntityType::ASSIGN, Synonym{ "a" }));
		q3.addEntity(Entity(EntityType::ASSIGN, Synonym{ "a1" }));
		q3.addEntity(Entity(EntityType::STMT, Synonym{ "s" }));
		q3.addSelected(Entity(EntityType::ASSIGN, Synonym{ "a" }));
		q3.addRelation(RelRef(RelType::PARENT_T, Entity(EntityType::STMT, Synonym{ "s" }), Entity(EntityType::ASSIGN, Synonym{ "a" })));
		q3.addPattern(Pattern(Entity(EntityType::ASSIGN, Synonym{ "a1" }), Entity(EntityType::VARIABLE, "i"), "10", false));

		EXPECT_EQ(test3, q3);
	}

	TEST(QueryPreprocessor, patternThenSuchThat) {
		QueryPreprocessor qp;
		Query test = qp.parse("assign a, a1; Select a pattern a1(\"y\", _\"x\"_) such that Modifies(a, \"x\") ");

		Query q;
		q.addEntity(Entity(EntityType::ASSIGN, Synonym{ "a" }));
		q.addEntity(Entity(EntityType::ASSIGN, Synonym{ "a1" }));
		q.addSelected(Entity(EntityType::ASSIGN, Synonym{ "a" }));
		q.addRelation(RelRef(RelType::MODIFIES_S, Entity(EntityType::ASSIGN, Synonym{ "a" }), Entity(EntityType::VARIABLE, "x")));
		q.addPattern(Pattern(Entity(EntityType::ASSIGN, Synonym{ "a1" }), Entity(EntityType::VARIABLE, "y"), "x", true));

		EXPECT_EQ(test, q);
	}

	TEST(QueryPreprocessor, invalidQueries) {
		QueryPreprocessor qp;
		Query q = Query();

		try {
			Query test1 = qp.parse("asg a;");
			Query test2 = qp.parse("la;skdjf lkasdfj laks;fdj");
		}
		catch (std::runtime_error const& err) {
			EXPECT_EQ(err.what(), std::string("Invalid syntax for declaration or select"));
		}
		catch (...) {
			FAIL();
		}

		try {
			Query test3 = qp.parse("assign a; Select s");
		}
		catch (std::runtime_error const& err) {
			EXPECT_EQ(err.what(), std::string("Select variable content has not been declared"));
		}
		catch (...) {
			FAIL();
		}

		try {
			Query test4 = qp.parse("assign a; stmt s; procedure p; Select a pattern p(s, _)");
			Query test5 = qp.parse("assign a; stmt s; call c; Select a pattern c(s, _)");
			Query test6 = qp.parse("assign a; stmt s; Select a pattern s(s, _)");
		}
		catch (std::runtime_error const& err) {
			EXPECT_EQ(err.what(), std::string("Invalid pattern type"));
		}
		catch (...) {
			FAIL();
		}
	}
}