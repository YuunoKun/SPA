#include "pch.h"

#include "QueryPreprocessor.h"
#include "QueryPreprocessor.cpp"
#include "Common.h"
#include <iostream>

namespace UnitTesting {
	class QueryPreprocessorTest : public ::testing::Test {
	protected:
		QueryPreprocessorTest() {
		}

		// If the constructor and destructor are not enough for setting up
		// and cleaning up each test, you can define the following methods:

		virtual void SetUp() override {
			// Code here will be called immediately after the constructor (right
			// before each test).
		}

		void TearDown() override {
			// Code here will be called immediately after each test (right
			// before the destructor).
		}

		// Class members declared here can be used by all tests in the test suite
		// for Foo.
	};

	// Single declaration of constant s
	// test to see if declaration and selected entity are passed correctly
	TEST(QueryPreprocessor, processConstant) {
		QueryPreprocessor preprocessor;
		Query query = preprocessor.process("constant s; Select s");
		
		// test if constant s is same
		Entity constantS = query.getEntities()["s"];
		std::string constantNameS = constantS.getName();
		EXPECT_EQ(constantNameS, "s");

		EntityType constantTypeS = constantS.getType();
		EXPECT_TRUE(constantTypeS == EntityType::CONSTANT);
		
		std::string out = query.getSelected().getName();
		EXPECT_EQ(out, "s");	
	}

	// Single declaration of procedure p
	// test to see if declaration and selected entity are passed correctly
	TEST(QueryPreprocessor, processProcedure) {
		QueryPreprocessor preprocessor;
		Query query = preprocessor.process("procedure p; Select p");

		Entity procedureP = query.getEntities()["p"];
		std::string procedureNameP = procedureP.getName();
		EXPECT_EQ(procedureNameP, "p") << "Wrong name for declaration procedure";

		EntityType procedureTypeP = procedureP.getType();
		EXPECT_TRUE(procedureTypeP == EntityType::PROCEDURE) << "Wrong procedure type";

		std::string out = query.getSelected().getName();
		EXPECT_EQ(out, "p") << "Wrong name for selected procedure";
	}

	// Multiple declaration, constant s and procedure p
	// test to see if declaration and selected entity are passed correctly
	TEST(QueryPreprocessor, processConstantAndProcedure) {
		QueryPreprocessor preprocessor;
		Query query = preprocessor.process("constant s; procedure p; Select s");

		// test if constant s is correct
		Entity constantS = query.getEntities()["s"];
		std::string constantNameS = constantS.getName();
		EXPECT_EQ(constantNameS, "s");

		EntityType constantTypeS = constantS.getType();
		EXPECT_TRUE(constantTypeS == EntityType::CONSTANT);

		//test if procedure p is correct
		Entity procedureP = query.getEntities()["p"];
		std::string procedureNameP = procedureP.getName();
		EXPECT_EQ(procedureNameP, "p") << "Wrong name for declaration procedure";

		EntityType procedureTypeP = procedureP.getType();
		EXPECT_TRUE(procedureTypeP == EntityType::PROCEDURE) << "Wrong procedure type";

		// test if selected is correct
		std::string out = query.getSelected().getName();
		EXPECT_EQ(out, "s");
	}




	// test to see if selected variable comes from entity's map






	//TEST(PKB, getConstants_second) {
	//	PKB::getInstance().resetCache();

	//	constant two = 2;
	//	std::vector<constant> v{ two };
	//	PKB::getInstance().setConstants(v);
	//	EXPECT_EQ(v, PKB::getInstance().getConstants());
	//}
	//TEST(PKB, getProcedures) {
	//	PKB::getInstance().resetCache();

	//	procedure_name first_procedure = "main";
	//	procedure_name second_procedure = "printY";
	//	std::vector<procedure_name> v{ first_procedure, second_procedure };
	//	PKB::getInstance().setProcedures(v);
	//	EXPECT_EQ(v, PKB::getInstance().getProcedures());
	//}
	//TEST(PKB, resetCache) {
	//	PKB::getInstance().resetCache();

	//	std::vector<constant> vc{ 3 };
	//	std::vector<procedure_name> vp{ "main" };
	//	PKB::getInstance().setConstants(vc);
	//	PKB::getInstance().setProcedures(vp);
	//	PKB::getInstance().resetCache();
	//	EXPECT_EQ(PKB::getInstance().getConstants().size(), 0);
	//	EXPECT_EQ(PKB::getInstance().getProcedures().size(), 0);
	//}
}