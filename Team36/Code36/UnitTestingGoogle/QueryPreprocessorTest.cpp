#include "pch.h"

#include "QueryPreprocessor.h"
#include "Entity.h"
#include "RelRef.h"
#include "SyntacticErrorException.h"
#include "SemanticErrorException.h"

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
		Query test = qp.parse("procedure Select; Select Select");

		Query q;
		q.addEntity(Entity(EntityType::PROCEDURE, Synonym{ "Select" }));
		q.addSelected(Entity(EntityType::PROCEDURE, Synonym{ "Select" }));

		EXPECT_EQ(test, q);

		Query test2 = qp.parse("procedure Select; stmt s; Select Select such that Follows* (6, s)");

		Query q2;
		q2.addEntity(Entity(EntityType::PROCEDURE, Synonym{ "Select" }));
		q2.addEntity(Entity(EntityType::STMT, Synonym{ "s" }));
		q2.addSelected(Entity(EntityType::PROCEDURE, Synonym{ "Select" }));
		q2.addRelation(RelRef(RelType::FOLLOWS_T, Entity(EntityType::CONSTANT, "6"), Entity(EntityType::STMT, Synonym{ "s" })));

		EXPECT_EQ(test2, q2);

		Query test3 = qp.parse("procedure Select, procedure; stmt s; Select Select such that Follows* (6, s)");

		Query q3;
		q3.addEntity(Entity(EntityType::PROCEDURE, Synonym{ "Select" }));
		q3.addEntity(Entity(EntityType::PROCEDURE, Synonym{ "procedure" }));
		q3.addEntity(Entity(EntityType::STMT, Synonym{ "s" }));
		q3.addSelected(Entity(EntityType::PROCEDURE, Synonym{ "Select" }));
		q3.addRelation(RelRef(RelType::FOLLOWS_T, Entity(EntityType::CONSTANT, "6"), Entity(EntityType::STMT, Synonym{ "s" })));

		EXPECT_EQ(test3, q3);
	}

	TEST(QueryPreprocessor, selectBOOLEAN) {
		QueryPreprocessor qp;
		Query test = qp.parse("procedure p; Select BOOLEAN");

		EXPECT_EQ(test.getSelected()[0], Entity(EntityType::BOOLEAN));
	}

	TEST(QueryPrecessor, selectBOOLEANAsSynonym) {
		QueryPreprocessor qp;
		Query test = qp.parse("procedure BOOLEAN; Select BOOLEAN");

		Query q;
		q.addEntity(Entity(EntityType::PROCEDURE, Synonym{ "BOOLEAN" }));
		q.addSelected(Entity(EntityType::PROCEDURE, Synonym{ "BOOLEAN" }));

		EXPECT_EQ(test, q);

		Query test2 = qp.parse("stmt BOOLEAN; Select BOOLEAN");

		Query q2;
		q2.addEntity(Entity(EntityType::STMT, Synonym{ "BOOLEAN" }));
		q2.addSelected(Entity(EntityType::STMT, Synonym{ "BOOLEAN" }));

		EXPECT_EQ(test2, q2);

		Query test3 = qp.parse("constant BOOLEAN; Select BOOLEAN");

		Query q3;
		q3.addEntity(Entity(EntityType::CONSTANT, Synonym{ "BOOLEAN" }));
		q3.addSelected(Entity(EntityType::CONSTANT, Synonym{ "BOOLEAN" }));

		EXPECT_EQ(test3, q3);

		Query test4 = qp.parse("assign BOOLEAN; Select BOOLEAN");

		Query q4;
		q4.addEntity(Entity(EntityType::ASSIGN, Synonym{ "BOOLEAN" }));
		q4.addSelected(Entity(EntityType::ASSIGN, Synonym{ "BOOLEAN" }));

		EXPECT_EQ(test4, q4);
	}

	TEST(QueryPreprocessor, selectBOOLEANWithSuchThat) {
		QueryPreprocessor qp;
		Query test = qp.parse("procedure p; Select BOOLEAN such that Next*(2,9)");

		EXPECT_EQ(test.getSelected()[0], Entity(EntityType::BOOLEAN));

		Query test2 = qp.parse("procedure p; Select BOOLEAN such that Next*(2,8) and Next*(8,9)");

		EXPECT_EQ(test2.getSelected()[0], Entity(EntityType::BOOLEAN));
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
		q4.addRelation(RelRef(RelType::MODIFIES_P, Entity(EntityType::PROCEDURE, Synonym{ "p" }), Entity(EntityType::VARIABLE, "x")));

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
		q7.addRelation(RelRef(RelType::MODIFIES_S, Entity(EntityType::WHILE, Synonym{ "w" }), Entity(EntityType::WILD)));

		EXPECT_EQ(test7, q7);

		Query test8 = qp.parse("stmt s; Select s such that Parent(1, 2)");

		Query q8;
		q8.addEntity(Entity(EntityType::STMT, Synonym{ "s" }));
		q8.addSelected(Entity(EntityType::STMT, Synonym{ "s" }));
		q8.addRelation(RelRef(RelType::PARENT, Entity(EntityType::CONSTANT, "1"), Entity(EntityType::CONSTANT, "2")));

		EXPECT_EQ(test8, q8);

		Query test9 = qp.parse("stmt s; Select s such that Follows(1, _)");

		Query q9;
		q9.addEntity(Entity(EntityType::STMT, Synonym{ "s" }));
		q9.addSelected(Entity(EntityType::STMT, Synonym{ "s" }));
		q9.addRelation(RelRef(RelType::FOLLOWS, Entity(EntityType::CONSTANT, "1"), Entity(EntityType::WILD)));

		EXPECT_EQ(test9, q9);

		Query test10 = qp.parse("stmt s; Select s such that Follows(_, 1)");

		Query q10;
		q10.addEntity(Entity(EntityType::STMT, Synonym{ "s" }));
		q10.addSelected(Entity(EntityType::STMT, Synonym{ "s" }));
		q10.addRelation(RelRef(RelType::FOLLOWS, Entity(EntityType::WILD, ""), Entity(EntityType::CONSTANT, "1")));

		EXPECT_EQ(test10, q10);

		Query test11 = qp.parse("stmt s; Select s such that Follows(_, _)");

		Query q11;
		q11.addEntity(Entity(EntityType::STMT, Synonym{ "s" }));
		q11.addSelected(Entity(EntityType::STMT, Synonym{ "s" }));
		q11.addRelation(RelRef(RelType::FOLLOWS, Entity(EntityType::WILD, ""), Entity(EntityType::WILD)));

		EXPECT_EQ(test11, q11);
	}

	TEST(QueryPreprocessor, suchThatCalls) {
		QueryPreprocessor qp;
		Query test1 = qp.parse("procedure p; Select p such that Calls (p, _)");
		Query q1;
		q1.addEntity(Entity(EntityType::PROCEDURE, Synonym{ "p" }));
		q1.addSelected(Entity(EntityType::PROCEDURE, Synonym{ "p" }));
		q1.addRelation(RelRef(RelType::CALLS, Entity(EntityType::PROCEDURE, Synonym{ "p" }), Entity(EntityType::WILD)));

		EXPECT_EQ(test1, q1);

		Query test2 = qp.parse("procedure p; Select p such that Calls (p, \"Third\")");
		Query q2;
		q2.addEntity(Entity(EntityType::PROCEDURE, Synonym{ "p" }));
		q2.addSelected(Entity(EntityType::PROCEDURE, Synonym{ "p" }));
		q2.addRelation(RelRef(RelType::CALLS, Entity(EntityType::PROCEDURE, Synonym{ "p" }), Entity(EntityType::PROCEDURE, "Third")));

		EXPECT_EQ(test2, q2);

		Query test3 = qp.parse("procedure p; Select p such that Calls (\"Second\", p)");
		Query q3;
		q3.addEntity(Entity(EntityType::PROCEDURE, Synonym{ "p" }));
		q3.addSelected(Entity(EntityType::PROCEDURE, Synonym{ "p" }));
		q3.addRelation(RelRef(RelType::CALLS, Entity(EntityType::PROCEDURE, "Second"), Entity(EntityType::PROCEDURE, Synonym{ "p" })));

		EXPECT_EQ(test3, q3);
	}

	TEST(QueryPreprocessor, suchThatCallsT) {
		QueryPreprocessor qp;
		Query test1 = qp.parse("procedure p; Select p such that Calls* (p, _)");
		Query q1;
		q1.addEntity(Entity(EntityType::PROCEDURE, Synonym{ "p" }));
		q1.addSelected(Entity(EntityType::PROCEDURE, Synonym{ "p" }));
		q1.addRelation(RelRef(RelType::CALLS_T, Entity(EntityType::PROCEDURE, Synonym{ "p" }), Entity(EntityType::WILD)));

		EXPECT_EQ(test1, q1);

		Query test2 = qp.parse("procedure p; Select p such that Calls* (p, \"Third\")");
		Query q2;
		q2.addEntity(Entity(EntityType::PROCEDURE, Synonym{ "p" }));
		q2.addSelected(Entity(EntityType::PROCEDURE, Synonym{ "p" }));
		q2.addRelation(RelRef(RelType::CALLS_T, Entity(EntityType::PROCEDURE, Synonym{ "p" }), Entity(EntityType::PROCEDURE, "Third")));

		EXPECT_EQ(test2, q2);

		Query test3 = qp.parse("procedure p; Select p such that Calls* (\"Second\", p)");
		Query q3;
		q3.addEntity(Entity(EntityType::PROCEDURE, Synonym{ "p" }));
		q3.addSelected(Entity(EntityType::PROCEDURE, Synonym{ "p" }));
		q3.addRelation(RelRef(RelType::CALLS_T, Entity(EntityType::PROCEDURE, "Second"), Entity(EntityType::PROCEDURE, Synonym{ "p" })));

		EXPECT_EQ(test3, q3);
	}

	TEST(QueryPreprocessor, selectWithAttribute) {
		QueryPreprocessor qp;
		Query test1 = qp.parse("stmt s; Select s.stmt#");
		EXPECT_EQ(test1.getSelected()[0].getAttribute(), AttrRef{ STMT_INDEX });

		Query test2 = qp.parse("procedure p; Select p.procName");
		EXPECT_EQ(test2.getSelected()[0].getAttribute(), AttrRef{ PROC_NAME });

		Query test3 = qp.parse("variable v; Select v.varName");
		EXPECT_EQ(test3.getSelected()[0].getAttribute(), AttrRef{ VAR_NAME });

		Query test4 = qp.parse("read r; Select r.varName");
		EXPECT_EQ(test4.getSelected()[0].getAttribute(), AttrRef{ VAR_NAME });

		Query test5 = qp.parse("print pr; Select pr.varName");
		EXPECT_EQ(test5.getSelected()[0].getAttribute(), AttrRef{ VAR_NAME });

		Query test6 = qp.parse("constant c; Select c.value");
		EXPECT_EQ(test6.getSelected()[0].getAttribute(), AttrRef{ VALUE });

		Query test7 = qp.parse("read r; Select r.stmt#");
		EXPECT_EQ(test7.getSelected()[0].getAttribute(), AttrRef{ STMT_INDEX });

		Query test8 = qp.parse("print pr; Select pr.stmt#");
		EXPECT_EQ(test8.getSelected()[0].getAttribute(), AttrRef{ STMT_INDEX });

		Query test9 = qp.parse("call c; Select c.stmt#");
		EXPECT_EQ(test9.getSelected()[0].getAttribute(), AttrRef{ STMT_INDEX });

		Query test10 = qp.parse("while w; Select w.stmt#");
		EXPECT_EQ(test10.getSelected()[0].getAttribute(), AttrRef{ STMT_INDEX });

		Query test11 = qp.parse("if ifs; Select ifs.stmt#");
		EXPECT_EQ(test11.getSelected()[0].getAttribute(), AttrRef{ STMT_INDEX });

		Query test12 = qp.parse("assign a; Select a.stmt#");
		EXPECT_EQ(test12.getSelected()[0].getAttribute(), AttrRef{ STMT_INDEX });
	}

	TEST(QueryPreprocessor, selectWithInvalidEntityTypeForAttribute) {
		QueryPreprocessor qp;

		EXPECT_THROW(qp.parse("procedure p; call c; variable v; read r; print pr; constant co; stmt s; while w; if ifs; assign a; Select v.procName"), SemanticErrorException);
		qp.resetQuery();

		EXPECT_THROW(qp.parse("procedure p; call c; variable v; read r; print pr; constant co; stmt s; while w; if ifs; assign a; Select r.procName"), SemanticErrorException);
		qp.resetQuery();

		EXPECT_THROW(qp.parse("procedure p; call c; variable v; read r; print pr; constant co; stmt s; while w; if ifs; assign a; Select pr.procName"), SemanticErrorException);
		qp.resetQuery();

		EXPECT_THROW(qp.parse("procedure p; call c; variable v; read r; print pr; constant co; stmt s; while w; if ifs; assign a; Select co.procName"), SemanticErrorException);
		qp.resetQuery();

		EXPECT_THROW(qp.parse("procedure p; call c; variable v; read r; print pr; constant co; stmt s; while w; if ifs; assign a; Select w.procName"), SemanticErrorException);
		qp.resetQuery();

		EXPECT_THROW(qp.parse("procedure p; call c; variable v; read r; print pr; constant co; stmt s; while w; if ifs; assign a; Select ifs.procName"), SemanticErrorException);
		qp.resetQuery();

		EXPECT_THROW(qp.parse("procedure p; call c; variable v; read r; print pr; constant co; stmt s; while w; if ifs; assign a; Select a.procName"), SemanticErrorException);
		qp.resetQuery();

		EXPECT_THROW(qp.parse("procedure p; call c; variable v; read r; print pr; constant co; stmt s; while w; if ifs; assign a; Select p.varName"), SemanticErrorException);
		qp.resetQuery();

		EXPECT_THROW(qp.parse("procedure p; call c; variable v; read r; print pr; constant co; stmt s; while w; if ifs; assign a; Select c.varName"), SemanticErrorException);
		qp.resetQuery();

		EXPECT_THROW(qp.parse("procedure p; call c; variable v; read r; print pr; constant co; stmt s; while w; if ifs; assign a; Select co.varName"), SemanticErrorException);
		qp.resetQuery();

		EXPECT_THROW(qp.parse("procedure p; call c; variable v; read r; print pr; constant co; stmt s; while w; if ifs; assign a; Select s.varName"), SemanticErrorException);
		qp.resetQuery();

		EXPECT_THROW(qp.parse("procedure p; call c; variable v; read r; print pr; constant co; stmt s; while w; if ifs; assign a; Select w.varName"), SemanticErrorException);
		qp.resetQuery();

		EXPECT_THROW(qp.parse("procedure p; call c; variable v; read r; print pr; constant co; stmt s; while w; if ifs; assign a; Select ifs.varName"), SemanticErrorException);
		qp.resetQuery();

		EXPECT_THROW(qp.parse("procedure p; call c; variable v; read r; print pr; constant co; stmt s; while w; if ifs; assign a; Select a.varName"), SemanticErrorException);
		qp.resetQuery();

		EXPECT_THROW(qp.parse("procedure p; call c; variable v; read r; print pr; constant co; stmt s; while w; if ifs; assign a; Select v.value"), SemanticErrorException);
		qp.resetQuery();

		EXPECT_THROW(qp.parse("procedure p; call c; variable v; read r; print pr; constant co; stmt s; while w; if ifs; assign a; Select r.value"), SemanticErrorException);
		qp.resetQuery();

		EXPECT_THROW(qp.parse("procedure p; call c; variable v; read r; print pr; constant co; stmt s; while w; if ifs; assign a; Select pr.value"), SemanticErrorException);
		qp.resetQuery();

		EXPECT_THROW(qp.parse("procedure p; call c; variable v; read r; print pr; constant co; stmt s; while w; if ifs; assign a; Select p.value"), SemanticErrorException);
		qp.resetQuery();

		EXPECT_THROW(qp.parse("procedure p; call c; variable v; read r; print pr; constant co; stmt s; while w; if ifs; assign a; Select c.value"), SemanticErrorException);
		qp.resetQuery();

		EXPECT_THROW(qp.parse("procedure p; call c; variable v; read r; print pr; constant co; stmt s; while w; if ifs; assign a; Select w.value"), SemanticErrorException);
		qp.resetQuery();

		EXPECT_THROW(qp.parse("procedure p; call c; variable v; read r; print pr; constant co; stmt s; while w; if ifs; assign a; Select ifs.value"), SemanticErrorException);
		qp.resetQuery();

		EXPECT_THROW(qp.parse("procedure p; call c; variable v; read r; print pr; constant co; stmt s; while w; if ifs; assign a; Select a.value"), SemanticErrorException);
		qp.resetQuery();

		EXPECT_THROW(qp.parse("procedure p; call c; variable v; read r; print pr; constant co; stmt s; while w; if ifs; assign a; Select v.stmt#"), SemanticErrorException);
		qp.resetQuery();

		EXPECT_THROW(qp.parse("procedure p; call c; variable v; read r; print pr; constant co; stmt s; while w; if ifs; assign a; Select co.stmt#"), SemanticErrorException);
		qp.resetQuery();

		EXPECT_THROW(qp.parse("procedure p; call c; variable v; read r; print pr; constant co; stmt s; while w; if ifs; assign a; Select p.stmt#"), SemanticErrorException);
		qp.resetQuery();
	}

	TEST(QueryPreprocessor, selectMultipleClauses) {
		QueryPreprocessor qp;
		Query test1 = qp.parse("call c1, c2; Select <c1.procName, c2>");
		EXPECT_EQ(test1.getSelected()[0].getAttribute(), AttrRef{ PROC_NAME });
		EXPECT_EQ(test1.getSelected()[1].getAttribute(), AttrRef{ STMT_INDEX });

		Query test2 = qp.parse("call c1, c2; Select <c1.procName, c2.stmt#>");
		EXPECT_EQ(test2.getSelected()[0].getAttribute(), AttrRef{ PROC_NAME });
		EXPECT_EQ(test2.getSelected()[1].getAttribute(), AttrRef{ STMT_INDEX });

		Query test3 = qp.parse("call c1, c2; procedure p; constant co; Select <c1.procName, c2.stmt#, p.procName, co.value>");
		EXPECT_EQ(test3.getSelected()[0].getAttribute(), AttrRef{ PROC_NAME });
		EXPECT_EQ(test3.getSelected()[1].getAttribute(), AttrRef{ STMT_INDEX });
		EXPECT_EQ(test3.getSelected()[2].getAttribute(), AttrRef{ PROC_NAME });
		EXPECT_EQ(test3.getSelected()[3].getAttribute(), AttrRef{ VALUE });
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

		Query test8 = qp.parse("assign pattern; procedure p; Select pattern pattern pattern(_,_)");

		Query q8;
		q8.addEntity(Entity(EntityType::ASSIGN, Synonym{ "pattern" }));
		q8.addEntity(Entity(EntityType::PROCEDURE, Synonym{ "p" }));
		q8.addSelected(Entity(EntityType::ASSIGN, Synonym{ "pattern" }));
		q8.addPattern(Pattern(Entity(EntityType::ASSIGN, Synonym{ "pattern" }), Entity(EntityType::WILD, ""), "", true));

		EXPECT_EQ(test8, q8);
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

		Query test2 = qp.parse("assign a; variable v; Select a such that Modifies(6, \"z\") pattern a(v, _)");

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

		Query test2 = qp.parse("assign a; variable v; Select a such that Uses(6, \"z\") pattern a(v, _)");

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
		q.addPattern(Pattern(Entity(EntityType::ASSIGN, Synonym{ "a1" }), Entity(EntityType::VARIABLE, "y"), "x", true));
		q.addRelation(RelRef(RelType::MODIFIES_S, Entity(EntityType::ASSIGN, Synonym{ "a" }), Entity(EntityType::VARIABLE, "x")));

		EXPECT_EQ(test, q);
	}

	TEST(QueryPreprocessor, multipleSuchThat) {
		QueryPreprocessor qp;

		Query test1 = qp.parse("procedure p; call c; while w; Select p such that Calls (\"Second\", p) such that Parent (w, c)");
		Query q1;
		q1.addEntity(Entity(EntityType::PROCEDURE, Synonym{ "p" }));
		q1.addEntity(Entity(EntityType::CALL, Synonym{ "c" }));
		q1.addEntity(Entity(EntityType::WHILE, Synonym{ "w" }));
		q1.addSelected(Entity(EntityType::PROCEDURE, Synonym{ "p" }));
		q1.addRelation(RelRef(RelType::CALLS, Entity(EntityType::PROCEDURE, "Second"), Entity(EntityType::PROCEDURE, Synonym{ "p" })));
		q1.addRelation(RelRef(RelType::PARENT, Entity(EntityType::WHILE, Synonym{ "w" }), Entity(EntityType::CALL, Synonym{ "c" })));

		EXPECT_EQ(test1, q1);

		Query test2 = qp.parse("stmt s; assign a, a1; variable v; Select a such that Parent(s, a) such that Uses(a, v)");

		Query q2;
		q2.addEntity(Entity(EntityType::STMT, Synonym{ "s" }));
		q2.addEntity(Entity(EntityType::ASSIGN, Synonym{ "a" }));
		q2.addEntity(Entity(EntityType::ASSIGN, Synonym{ "a1" }));
		q2.addEntity(Entity(EntityType::VARIABLE, Synonym{ "v" }));
		q2.addSelected(Entity(EntityType::ASSIGN, Synonym{ "a" }));
		q2.addRelation(RelRef(RelType::PARENT, Entity(EntityType::STMT, Synonym{ "s" }), Entity(EntityType::ASSIGN, Synonym{ "a" })));
		q2.addRelation(RelRef(RelType::USES_S, Entity(EntityType::ASSIGN, Synonym{ "a" }), Entity(EntityType::VARIABLE, Synonym{ "v" })));

		EXPECT_EQ(test2, q2);
	}

	TEST(QueryPreprocessor, multiplePattern) {
		QueryPreprocessor qp;
		Query test = qp.parse("stmt s; assign a, a1; variable v; Select a pattern a(\"q\", _\"p\"_) pattern a1(v, _)");

		Query q;
		q.addEntity(Entity(EntityType::STMT, Synonym{ "s" }));
		q.addEntity(Entity(EntityType::ASSIGN, Synonym{ "a" }));
		q.addEntity(Entity(EntityType::ASSIGN, Synonym{ "a1" }));
		q.addEntity(Entity(EntityType::VARIABLE, Synonym{ "v" }));
		q.addSelected(Entity(EntityType::ASSIGN, Synonym{ "a" }));
		q.addPattern(Pattern(Entity(EntityType::ASSIGN, Synonym{ "a" }), Entity(EntityType::VARIABLE, "q"), "p", true));
		q.addPattern(Pattern(Entity(EntityType::ASSIGN, Synonym{ "a1" }), Entity(EntityType::VARIABLE, Synonym{ "v" }), "", true));

		EXPECT_EQ(test, q);
	}

	TEST(QueryPreprocessor, usingKeywordAnd) {
		QueryPreprocessor qp;

		Query test1 = qp.parse("procedure p; call c; while w; Select p such that Calls (\"Second\", p) and Parent (w, c)");
		Query q1;
		q1.addEntity(Entity(EntityType::PROCEDURE, Synonym{ "p" }));
		q1.addEntity(Entity(EntityType::CALL, Synonym{ "c" }));
		q1.addEntity(Entity(EntityType::WHILE, Synonym{ "w" }));
		q1.addSelected(Entity(EntityType::PROCEDURE, Synonym{ "p" }));
		q1.addRelation(RelRef(RelType::CALLS, Entity(EntityType::PROCEDURE, "Second"), Entity(EntityType::PROCEDURE, Synonym{ "p" })));
		q1.addRelation(RelRef(RelType::PARENT, Entity(EntityType::WHILE, Synonym{ "w" }), Entity(EntityType::CALL, Synonym{ "c" })));

		EXPECT_EQ(test1, q1);

		Query test2 = qp.parse("stmt s; assign a, a1; variable v; Select a such that Parent(s, a) and Uses(a, v)");

		Query q2;
		q2.addEntity(Entity(EntityType::STMT, Synonym{ "s" }));
		q2.addEntity(Entity(EntityType::ASSIGN, Synonym{ "a" }));
		q2.addEntity(Entity(EntityType::ASSIGN, Synonym{ "a1" }));
		q2.addEntity(Entity(EntityType::VARIABLE, Synonym{ "v" }));
		q2.addSelected(Entity(EntityType::ASSIGN, Synonym{ "a" }));
		q2.addRelation(RelRef(RelType::PARENT, Entity(EntityType::STMT, Synonym{ "s" }), Entity(EntityType::ASSIGN, Synonym{ "a" })));
		q2.addRelation(RelRef(RelType::USES_S, Entity(EntityType::ASSIGN, Synonym{ "a" }), Entity(EntityType::VARIABLE, Synonym{ "v" })));

		EXPECT_EQ(test2, q2);

		Query test3 = qp.parse("stmt s; assign a, a1; variable v; Select a pattern a(\"q\", _\"p\"_) and a1(v, _)");

		Query q3;
		q3.addEntity(Entity(EntityType::STMT, Synonym{ "s" }));
		q3.addEntity(Entity(EntityType::ASSIGN, Synonym{ "a" }));
		q3.addEntity(Entity(EntityType::ASSIGN, Synonym{ "a1" }));
		q3.addEntity(Entity(EntityType::VARIABLE, Synonym{ "v" }));
		q3.addSelected(Entity(EntityType::ASSIGN, Synonym{ "a" }));
		q3.addPattern(Pattern(Entity(EntityType::ASSIGN, Synonym{ "a" }), Entity(EntityType::VARIABLE, "q"), "p", true));
		q3.addPattern(Pattern(Entity(EntityType::ASSIGN, Synonym{ "a1" }), Entity(EntityType::VARIABLE, Synonym{ "v" }), "", true));

		EXPECT_EQ(test3, q3);

		Query test4 = qp.parse("stmt s; assign a, a1; variable v; Select a pattern a(\"q\", _\"p\"_) and a1(v, _) such that Parent(s, a) and Uses(a, v)");

		Query q4;
		q4.addEntity(Entity(EntityType::STMT, Synonym{ "s" }));
		q4.addEntity(Entity(EntityType::ASSIGN, Synonym{ "a" }));
		q4.addEntity(Entity(EntityType::ASSIGN, Synonym{ "a1" }));
		q4.addEntity(Entity(EntityType::VARIABLE, Synonym{ "v" }));
		q4.addSelected(Entity(EntityType::ASSIGN, Synonym{ "a" }));
		q4.addPattern(Pattern(Entity(EntityType::ASSIGN, Synonym{ "a" }), Entity(EntityType::VARIABLE, "q"), "p", true));
		q4.addPattern(Pattern(Entity(EntityType::ASSIGN, Synonym{ "a1" }), Entity(EntityType::VARIABLE, Synonym{ "v" }), "", true));
		q4.addRelation(RelRef(RelType::PARENT, Entity(EntityType::STMT, Synonym{ "s" }), Entity(EntityType::ASSIGN, Synonym{ "a" })));
		q4.addRelation(RelRef(RelType::USES_S, Entity(EntityType::ASSIGN, Synonym{ "a" }), Entity(EntityType::VARIABLE, Synonym{ "v" })));

		EXPECT_EQ(test4, q4);
	}

	TEST(QueryPreprocessor, tupleSelect) {
		QueryPreprocessor qp;

		Query test1 = qp.parse("procedure p, q; Select <p,q>");

		Query q1;
		q1.addEntity(Entity(EntityType::PROCEDURE, Synonym{ "p" }));
		q1.addEntity(Entity(EntityType::PROCEDURE, Synonym{ "q" }));
		q1.addSelected(Entity(EntityType::PROCEDURE, Synonym{ "p" }));
		q1.addSelected(Entity(EntityType::PROCEDURE, Synonym{ "q" }));

		EXPECT_EQ(test1, q1);
	}

	TEST(QueryPreprocessor, tupleSelectWithSuchThat) {
		QueryPreprocessor qp;

		Query test1 = qp.parse("procedure p, q; Select <p,q> such that Calls (p,q)");

		Query q1;
		q1.addEntity(Entity(EntityType::PROCEDURE, Synonym{ "p" }));
		q1.addEntity(Entity(EntityType::PROCEDURE, Synonym{ "q" }));
		q1.addSelected(Entity(EntityType::PROCEDURE, Synonym{ "p" }));
		q1.addSelected(Entity(EntityType::PROCEDURE, Synonym{ "q" }));
		q1.addRelation(RelRef(RelType::CALLS, Entity(EntityType::PROCEDURE, Synonym{ "p" }), Entity(EntityType::PROCEDURE, Synonym{ "q" })));

		EXPECT_EQ(test1, q1);
	}

	TEST(QueryPreprocessor, tupleSelectAnd) {
		QueryPreprocessor qp;
		Query test1 = qp.parse("while w1, w2, w3; Select <w1, w2, w3> such that Parent*(w1, w2) and Parent*(w2, w3)");

		Query q1;
		q1.addEntity(Entity(EntityType::WHILE, Synonym{ "w1" }));
		q1.addEntity(Entity(EntityType::WHILE, Synonym{ "w2" }));
		q1.addEntity(Entity(EntityType::WHILE, Synonym{ "w3" }));
		q1.addSelected(Entity(EntityType::WHILE, Synonym{ "w1" }));
		q1.addSelected(Entity(EntityType::WHILE, Synonym{ "w2" }));
		q1.addSelected(Entity(EntityType::WHILE, Synonym{ "w3" }));
		q1.addRelation(RelRef(RelType::PARENT_T, Entity(EntityType::WHILE, Synonym{ "w1" }), Entity(EntityType::WHILE, Synonym{ "w2" })));
		q1.addRelation(RelRef(RelType::PARENT_T, Entity(EntityType::WHILE, Synonym{ "w2" }), Entity(EntityType::WHILE, Synonym{ "w3" })));

		EXPECT_EQ(test1, q1);
	}

	TEST(QueryPreprocessor, tupleWithAttributes) {
		QueryPreprocessor qp;
		Query test1 = qp.parse("assign a1, a2; Select <a1.stmt#, a2>");

		Query q1;
		q1.addEntity(Entity(EntityType::ASSIGN, Synonym{ "a1" }));
		q1.addEntity(Entity(EntityType::ASSIGN, Synonym{ "a2" }));
		q1.addSelected(Entity(EntityType::ASSIGN, Synonym{ "a1" }, AttrRef::STMT_INDEX));
		q1.addSelected(Entity(EntityType::ASSIGN, Synonym{ "a2" }));

		EXPECT_EQ(test1, q1);
	}

	TEST(QueryPreprocessor, tupleWithAttributesAndRelations) {
		QueryPreprocessor qp;
		Query test1 = qp.parse("assign a1, a2; stmt s; Select <a1.stmt#, a2> such that Follows(s,a1)");

		Query q1;
		q1.addEntity(Entity(EntityType::ASSIGN, Synonym{ "a1" }));
		q1.addEntity(Entity(EntityType::ASSIGN, Synonym{ "a2" }));
		q1.addEntity(Entity(EntityType::STMT, Synonym{ "s" }));
		q1.addSelected(Entity(EntityType::ASSIGN, Synonym{ "a1" }, AttrRef::STMT_INDEX));
		q1.addSelected(Entity(EntityType::ASSIGN, Synonym{ "a2" }));
		q1.addRelation(RelRef(RelType::FOLLOWS, Entity(EntityType::STMT, Synonym{ "s" }), Entity(EntityType::ASSIGN, Synonym{ "a1" })));

		EXPECT_EQ(test1, q1);
	}

	TEST(QueryPreprocessor, tupleWithAttributesAndMultipleRelations) {
		QueryPreprocessor qp;
		Query test1 = qp.parse("assign a1, a2; stmt s; while w1, w2; Select <a1.stmt#, a2> such that Follows(s,a1) and Parent(w1, w2)");

		Query q1;
		q1.addEntity(Entity(EntityType::ASSIGN, Synonym{ "a1" }));
		q1.addEntity(Entity(EntityType::ASSIGN, Synonym{ "a2" }));
		q1.addEntity(Entity(EntityType::STMT, Synonym{ "s" }));
		q1.addEntity(Entity(EntityType::WHILE, Synonym{ "w1" }));
		q1.addEntity(Entity(EntityType::WHILE, Synonym{ "w2" }));
		q1.addSelected(Entity(EntityType::ASSIGN, Synonym{ "a1" }, AttrRef::STMT_INDEX));
		q1.addSelected(Entity(EntityType::ASSIGN, Synonym{ "a2" }));
		q1.addRelation(RelRef(RelType::FOLLOWS, Entity(EntityType::STMT, Synonym{ "s" }), Entity(EntityType::ASSIGN, Synonym{ "a1" })));
		q1.addRelation(RelRef(RelType::PARENT, Entity(EntityType::WHILE, Synonym{ "w1" }), Entity(EntityType::WHILE, Synonym{ "w2" })));

		EXPECT_EQ(test1, q1);
	}

	TEST(QueryPreprocessor, tupleWithAttributesAndPattern) {
		QueryPreprocessor qp;
		Query test1 = qp.parse("assign a1, a2; stmt s; Select <a1.stmt#, a2> pattern a1(\"q\", _\"p\"_)");

		Query q1;
		q1.addEntity(Entity(EntityType::ASSIGN, Synonym{ "a1" }));
		q1.addEntity(Entity(EntityType::ASSIGN, Synonym{ "a2" }));
		q1.addEntity(Entity(EntityType::STMT, Synonym{ "s" }));
		q1.addSelected(Entity(EntityType::ASSIGN, Synonym{ "a1" }, AttrRef::STMT_INDEX));
		q1.addSelected(Entity(EntityType::ASSIGN, Synonym{ "a2" }));
		q1.addPattern(Pattern(Entity(EntityType::ASSIGN, Synonym{ "a1" }), Entity(EntityType::VARIABLE, "q"), "p", true));

		EXPECT_EQ(test1, q1);
	}

	TEST(QueryPreprocessor, tupleWithAttributesAndMultiplePatterns) {
		QueryPreprocessor qp;
		Query test1 = qp.parse("assign a1, a2; stmt s; variable v; Select <a1.stmt#, a2> pattern a1(\"q\", _\"p\"_) and a2(v, _)");

		Query q1;
		q1.addEntity(Entity(EntityType::ASSIGN, Synonym{ "a1" }));
		q1.addEntity(Entity(EntityType::ASSIGN, Synonym{ "a2" }));
		q1.addEntity(Entity(EntityType::STMT, Synonym{ "s" }));
		q1.addEntity(Entity(EntityType::VARIABLE, Synonym{ "v" }));
		q1.addSelected(Entity(EntityType::ASSIGN, Synonym{ "a1" }, AttrRef::STMT_INDEX));
		q1.addSelected(Entity(EntityType::ASSIGN, Synonym{ "a2" }));
		q1.addPattern(Pattern(Entity(EntityType::ASSIGN, Synonym{ "a1" }), Entity(EntityType::VARIABLE, "q"), "p", true));
		q1.addPattern(Pattern(Entity(EntityType::ASSIGN, Synonym{ "a2" }), Entity(EntityType::VARIABLE, Synonym{ "v" }), "", true));

		EXPECT_EQ(test1, q1);
	}

	TEST(QueryPreprocessor, invalidSyntax) {
		QueryPreprocessor qp;

		EXPECT_THROW(qp.parse("Stmt s; read r; print p; while w; if ifs; assign a; variable v; constant co; procedure pr; call c; Select s"), SyntacticErrorException);
		qp.resetQuery();

		EXPECT_THROW(qp.parse("stmt s; read r; print p; while w; if ifs; assign a; variable v; constant co; procedure pr call c; Select s"), SyntacticErrorException);
		qp.resetQuery();

		EXPECT_THROW(qp.parse("assign a;; stmt s; Select s"), SyntacticErrorException);
		qp.resetQuery();

		EXPECT_THROW(qp.parse("stmt s; read r; print p; while w; if ifs; assign a; variable v; constant co; procedure pr; call c; Select s Follows(s,a)"), SyntacticErrorException);
		qp.resetQuery();

		EXPECT_THROW(qp.parse("asg a; Select a"), SyntacticErrorException);
		qp.resetQuery();

		EXPECT_THROW(qp.parse("assign a;; stmt s; Select s"), SyntacticErrorException);
		qp.resetQuery();

		EXPECT_THROW(qp.parse("assign a; stmt; s; Select s"), SyntacticErrorException);
		qp.resetQuery();

		EXPECT_THROW(qp.parse("assign a a1; stmt s; Select s"), SyntacticErrorException);
		qp.resetQuery();

		EXPECT_THROW(qp.parse("assign a; a1; stmt s; Select s"), SyntacticErrorException);
		qp.resetQuery();

		EXPECT_THROW(qp.parse("assign a; stmt s; Select s Such that Follows(s,a)"), SyntacticErrorException);
		qp.resetQuery();

		EXPECT_THROW(qp.parse("assign a; stmt s; Select s such That Follows(s,a)"), SyntacticErrorException);
		qp.resetQuery();

		EXPECT_THROW(qp.parse("assign a; stmt s; Select s suchthat Follows(s,a)"), SyntacticErrorException);
		qp.resetQuery();

		EXPECT_THROW(qp.parse("assign a; stmt s; Select s such   that Follows(s,a)"), SyntacticErrorException);
		qp.resetQuery();

		EXPECT_THROW(qp.parse("assign a; stmt s; Select s such that such that Follows(s,a)"), SyntacticErrorException);
		qp.resetQuery();

		EXPECT_THROW(qp.parse("assign a; stmt s; Select s such that Follows(s,a) such Follows(s,a)"), SyntacticErrorException);
		qp.resetQuery();

		EXPECT_THROW(qp.parse("assign a; stmt s; Select s such that Follows(s,a) that Follows(s,a)"), SyntacticErrorException);
		qp.resetQuery();

		EXPECT_THROW(qp.parse("assign a; stmt s; Select s such that Follows(s,a) such That Follows(s,a)"), SyntacticErrorException);
		qp.resetQuery();

		EXPECT_THROW(qp.parse("assign a; stmt s; Select s such that Follows(s,a) Such that Follows(s,a)"), SyntacticErrorException);
		qp.resetQuery();

		EXPECT_THROW(qp.parse("assign a; stmt s; Select s such that Follows(1,2) Follows(s,a)"), SyntacticErrorException);
		qp.resetQuery();

		EXPECT_THROW(qp.parse("assign a; stmt s; Select s such that Follows(1,2) such That Follows(s,a)"), SyntacticErrorException);
		qp.resetQuery();

		EXPECT_THROW(qp.parse("stmt s; read r; print p; while w; if ifs; assign a; variable v; constant co; procedure pr; call c; Select s such that Follows(1,\")"), SyntacticErrorException);
		qp.resetQuery();

		EXPECT_THROW(qp.parse("stmt s; read r; print p; while w; if ifs; assign a; variable v; constant co; procedure pr; call c; Select s such that Follows(,2)"), SyntacticErrorException);
		qp.resetQuery();

		EXPECT_THROW(qp.parse("stmt s; read r; print p; while w; if ifs; assign a; variable v; constant co; procedure pr; call c; Select s such that Follows***(1,2)"), SyntacticErrorException);
		qp.resetQuery();

		EXPECT_THROW(qp.parse("stmt s; read r; print p; while w; if ifs; assign a; variable v; constant co; procedure pr; call c; Select s such that Modifies()"), SyntacticErrorException);
		qp.resetQuery();

		EXPECT_THROW(qp.parse("assign a; variable v; Select v pattern a(v, \"x\" \"x\")"), SyntacticErrorException);
		qp.resetQuery();

		EXPECT_THROW(qp.parse("assign a; variable v; Select v pattern a(v , _\"x\"_ _\"x\"_)"), SyntacticErrorException);
		qp.resetQuery();

		EXPECT_THROW(qp.parse("assign a; variable v; Select v pattern a(v, _ _)"), SyntacticErrorException);
		qp.resetQuery();

		EXPECT_THROW(qp.parse("stmts s1, s2, s3; Select s1 Follows(s1 s2, s3)"), SyntacticErrorException);
		qp.resetQuery();

		EXPECT_THROW(qp.parse("stmts s1, s2, s3; Select s1 Follows(s1 _, s3)"), SyntacticErrorException);
		qp.resetQuery();

		EXPECT_THROW(qp.parse("stmts s1, s2, s3; Select s1 Follows(s1 1, s3)"), SyntacticErrorException);
		qp.resetQuery();

		EXPECT_THROW(qp.parse("stmts s1, s2, s3; Select s1 Follows(1 1, s3)"), SyntacticErrorException);
		qp.resetQuery();

		EXPECT_THROW(qp.parse("stmts s1, s2, s3; Select s1 Follows(_ s1, s3)"), SyntacticErrorException);
		qp.resetQuery();

		EXPECT_THROW(qp.parse("stmts s1, s2, s3; Select s1 Follows(s1, _ s3)"), SyntacticErrorException);
		qp.resetQuery();

		EXPECT_THROW(qp.parse("stmts s1, s2, s3; Select s1 Follows(s1, _ _)"), SyntacticErrorException);
		qp.resetQuery();

		EXPECT_THROW(qp.parse("stmts s1, s2, s3; Select s1 Follows("), SyntacticErrorException);
		qp.resetQuery();

		EXPECT_THROW(qp.parse("stmts s1, s2, s3; Select s1 Follows(s1"), SyntacticErrorException);
		qp.resetQuery();

		EXPECT_THROW(qp.parse("stmts s1, s2, s3; Select s1 Follows(s1,s2"), SyntacticErrorException);
		qp.resetQuery();

		EXPECT_THROW(qp.parse("assign a; variable v; Select a pattern a(v,_\"x\"_"), SyntacticErrorException);
		qp.resetQuery();

		EXPECT_THROW(qp.parse("assign a; variable v; Select a pattern a(v,_\"x\"_) s"), SyntacticErrorException);
		qp.resetQuery();

		EXPECT_THROW(qp.parse("assign 1; Select 1"), SyntacticErrorException);
		qp.resetQuery();

		EXPECT_THROW(qp.parse("assign a; stmt s; Select s such that such that Follows(s,a)"), SyntacticErrorException);
		qp.resetQuery();

		EXPECT_THROW(qp.parse("stmt s; assign a, a1; variable v; Select a pattern a(v, _) and pattern a1(v, _)"), SyntacticErrorException);
		qp.resetQuery();
	}

	TEST(QueryPreprocessor, invalidSemantic) {
		QueryPreprocessor qp;
		EXPECT_THROW(qp.parse("stmt s, s; read r; print p; while w; if ifs; assign a; variable v; constant co; procedure pr; call c; Select s"), SemanticErrorException);
		qp.resetQuery();

		EXPECT_THROW(qp.parse("assign a; Select s"), SemanticErrorException);
		qp.resetQuery();

		EXPECT_THROW(qp.parse("assign a; stmt s; procedure p; Select a pattern p(s, _)"), SemanticErrorException);
		qp.resetQuery();

		EXPECT_THROW(qp.parse("assign a; stmt s; call c; Select a pattern c(s, _)"), SemanticErrorException);
		qp.resetQuery();

		EXPECT_THROW(qp.parse("assign a; stmt s; Select a pattern s(s, _)"), SemanticErrorException);
		qp.resetQuery();

		EXPECT_THROW(qp.parse("assign a; stmt s; procedure p; Select a pattern p(s, _)"), SemanticErrorException);
		qp.resetQuery();

		EXPECT_THROW(qp.parse("assign a; variable v; Select a pattern pattern(v, _)"), SemanticErrorException);
		qp.resetQuery();

		EXPECT_THROW(qp.parse("assign a; variable v; Select a pattern pattern a(v, _)"), SemanticErrorException);
		qp.resetQuery();

		EXPECT_THROW(qp.parse("assign a; variable v; Select a pattern pattern a(v, _)"), SemanticErrorException);
		qp.resetQuery();
	}

	TEST(QueryPreprocessor, invalidSemanticFollows) {
		QueryPreprocessor qp;

		EXPECT_THROW(qp.parse("stmt s; read r; print p; while w; if ifs; assign a; variable v; constant co; procedure pr; call c; Select s such that Follows(s,v)"), SemanticErrorException);
		qp.resetQuery();

		EXPECT_THROW(qp.parse("stmt s; read r; print p; while w; if ifs; assign a; variable v; constant co; procedure pr; call c; Select s such that Follows(s,co)"), SemanticErrorException);
		qp.resetQuery();

		EXPECT_THROW(qp.parse("stmt s; read r; print p; while w; if ifs; assign a; variable v; constant co; procedure pr; call c; Select s such that Follows(s,pr)"), SemanticErrorException);
		qp.resetQuery();

		EXPECT_THROW(qp.parse("stmt s; read r; print p; while w; if ifs; assign a; variable v; constant co; procedure pr; call c; Select s such that Follows(v,s)"), SemanticErrorException);
		qp.resetQuery();

		EXPECT_THROW(qp.parse("stmt s; read r; print p; while w; if ifs; assign a; variable v; constant co; procedure pr; call c; Select s such that Follows(co, s)"), SemanticErrorException);
		qp.resetQuery();

		EXPECT_THROW(qp.parse("stmt s; read r; print p; while w; if ifs; assign a; variable v; constant co; procedure pr; call c; Select s such that Follows(pr, s)"), SemanticErrorException);
		qp.resetQuery();

		EXPECT_THROW(qp.parse("stmt s; read r; print p; while w; if ifs; assign a; variable v; constant co; procedure pr; call c; Select s such that Follows*(s,v)"), SemanticErrorException);
		qp.resetQuery();

		EXPECT_THROW(qp.parse("stmt s; read r; print p; while w; if ifs; assign a; variable v; constant co; procedure pr; call c; Select s such that Follows*(s,co)"), SemanticErrorException);
		qp.resetQuery();

		EXPECT_THROW(qp.parse("stmt s; read r; print p; while w; if ifs; assign a; variable v; constant co; procedure pr; call c; Select s such that Follows*(s,pr)"), SemanticErrorException);
		qp.resetQuery();

		EXPECT_THROW(qp.parse("stmt s; read r; print p; while w; if ifs; assign a; variable v; constant co; procedure pr; call c; Select s such that Follows*(v,s)"), SemanticErrorException);
		qp.resetQuery();

		EXPECT_THROW(qp.parse("stmt s; read r; print p; while w; if ifs; assign a; variable v; constant co; procedure pr; call c; Select s such that Follows*(co, s)"), SemanticErrorException);
		qp.resetQuery();

		EXPECT_THROW(qp.parse("stmt s; read r; print p; while w; if ifs; assign a; variable v; constant co; procedure pr; call c; Select s such that Follows*(pr, s)"), SemanticErrorException);
		qp.resetQuery();

		EXPECT_THROW(qp.parse("stmt s; read r; print p; while w; if ifs; assign a; variable v; constant co; procedure pr; call c; Select s such that Follows(x,y)"), SemanticErrorException);
		qp.resetQuery();

		EXPECT_THROW(qp.parse("stmt s; read r; print p; while w; if ifs; assign a; variable v; constant co; procedure pr; call c; Select s such that Follows(x,2)"), SemanticErrorException);
		qp.resetQuery();
	}

	TEST(QueryPreprocessor, invalidSemanticModifies) {
		QueryPreprocessor qp;

		EXPECT_THROW(qp.parse("stmt s; read r; print p; while w; if ifs; assign a; variable v; constant co; procedure pr; call c; Select s such that Modifies(v,v)"), SemanticErrorException);
		qp.resetQuery();

		EXPECT_THROW(qp.parse("stmt s; read r; print p; while w; if ifs; assign a; variable v; constant co; procedure pr; call c; Select s such that Modifies(co,v)"), SemanticErrorException);
		qp.resetQuery();

		EXPECT_THROW(qp.parse("stmt s; read r; print p; while w; if ifs; assign a; variable v; constant co; procedure pr; call c; Select s such that Modifies(s,s)"), SemanticErrorException);
		qp.resetQuery();

		EXPECT_THROW(qp.parse("stmt s; read r; print p; while w; if ifs; assign a; variable v; constant co; procedure pr; call c; Select s such that Modifies(s,r)"), SemanticErrorException);
		qp.resetQuery();

		EXPECT_THROW(qp.parse("stmt s; read r; print p; while w; if ifs; assign a; variable v; constant co; procedure pr; call c; Select s such that Modifies(s,p)"), SemanticErrorException);
		qp.resetQuery();

		EXPECT_THROW(qp.parse("stmt s; read r; print p; while w; if ifs; assign a; variable v; constant co; procedure pr; call c; Select s such that Modifies(s,w)"), SemanticErrorException);
		qp.resetQuery();

		EXPECT_THROW(qp.parse("stmt s; read r; print p; while w; if ifs; assign a; variable v; constant co; procedure pr; call c; Select s such that Modifies(s,ifs)"), SemanticErrorException);
		qp.resetQuery();

		EXPECT_THROW(qp.parse("stmt s; read r; print p; while w; if ifs; assign a; variable v; constant co; procedure pr; call c; Select s such that Modifies(s,a)"), SemanticErrorException);
		qp.resetQuery();

		EXPECT_THROW(qp.parse("stmt s; read r; print p; while w; if ifs; assign a; variable v; constant co; procedure pr; call c; Select s such that Modifies(s,co)"), SemanticErrorException);
		qp.resetQuery();

		EXPECT_THROW(qp.parse("stmt s; read r; print p; while w; if ifs; assign a; variable v; constant co; procedure pr; call c; Select s such that Modifies(s,pr)"), SemanticErrorException);
		qp.resetQuery();

		EXPECT_THROW(qp.parse("stmt s; read r; print p; while w; if ifs; assign a; variable v; constant co; procedure pr; call c; Select s such that Modifies(s,c)"), SemanticErrorException);
		qp.resetQuery();
	}

	TEST(QueryPreprocessor, invalidSemanticUses) {
		QueryPreprocessor qp;

		EXPECT_THROW(qp.parse("stmt s; read r; print p; while w; if ifs; assign a; variable v; constant co; procedure pr; call c; Select s such that Uses(v,v)"), SemanticErrorException);
		qp.resetQuery();

		EXPECT_THROW(qp.parse("stmt s; read r; print p; while w; if ifs; assign a; variable v; constant co; procedure pr; call c; Select s such that Uses(co,v)"), SemanticErrorException);
		qp.resetQuery();

		EXPECT_THROW(qp.parse("stmt s; read r; print p; while w; if ifs; assign a; variable v; constant co; procedure pr; call c; Select s such that Uses(s,s)"), SemanticErrorException);
		qp.resetQuery();

		EXPECT_THROW(qp.parse("stmt s; read r; print p; while w; if ifs; assign a; variable v; constant co; procedure pr; call c; Select s such that Uses(s,r)"), SemanticErrorException);
		qp.resetQuery();

		EXPECT_THROW(qp.parse("stmt s; read r; print p; while w; if ifs; assign a; variable v; constant co; procedure pr; call c; Select s such that Uses(s,p)"), SemanticErrorException);
		qp.resetQuery();

		EXPECT_THROW(qp.parse("stmt s; read r; print p; while w; if ifs; assign a; variable v; constant co; procedure pr; call c; Select s such that Uses(s,w)"), SemanticErrorException);
		qp.resetQuery();

		EXPECT_THROW(qp.parse("stmt s; read r; print p; while w; if ifs; assign a; variable v; constant co; procedure pr; call c; Select s such that Uses(s,ifs)"), SemanticErrorException);
		qp.resetQuery();

		EXPECT_THROW(qp.parse("stmt s; read r; print p; while w; if ifs; assign a; variable v; constant co; procedure pr; call c; Select s such that Uses(s,a)"), SemanticErrorException);
		qp.resetQuery();

		EXPECT_THROW(qp.parse("stmt s; read r; print p; while w; if ifs; assign a; variable v; constant co; procedure pr; call c; Select s such that Uses(s,co)"), SemanticErrorException);
		qp.resetQuery();

		EXPECT_THROW(qp.parse("stmt s; read r; print p; while w; if ifs; assign a; variable v; constant co; procedure pr; call c; Select s such that Uses(s,pr)"), SemanticErrorException);
		qp.resetQuery();

		EXPECT_THROW(qp.parse("stmt s; read r; print p; while w; if ifs; assign a; variable v; constant co; procedure pr; call c; Select s such that Uses(s,c)"), SemanticErrorException);
		qp.resetQuery();
	}
}

TEST(QueryPreprocessor, invalidBOOLEANSemanticError) {
	QueryPreprocessor qp;

	EXPECT_THROW(qp.parse("stmt s; read r; print p; while w; if ifs; assign a; variable v; constant co; procedure pr; call c; Select BOOLEAN pattern co(_,_)"), SemanticErrorException);
	qp.resetQuery();
}

TEST(QueryPreprocessor, invalidSyntaxPrecendenceSemanticError) {
	QueryPreprocessor qp;

	EXPECT_THROW(qp.parse("stmt s; read r; print p; while w; if ifs; assign a; variable v; constant co; procedure pr; call c; Select asdf such that Uses(s,,)"), SyntacticErrorException);
	qp.resetQuery();

	EXPECT_THROW(qp.parse("stmt s; read r; print p; while w; if ifs; assign a; variable v; constant co; procedure pr; call c; Select BOOLEAN pattern co(_,_,,,)"), SemanticErrorException);
	qp.resetQuery();
}