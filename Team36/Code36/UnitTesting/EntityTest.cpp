#include "stdafx.h"
#include "CppUnitTest.h"

#include "../source/Entity.h"
#include "../source/Common.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
	TEST_CLASS(EntityTest) {
public:

	TEST_METHOD(getType) {
		for (int i = EntityType::STMT; i != EntityType::WILD; ++i) {
			EntityType type = static_cast<EntityType>(i);
			Entity testEntity(type, "test");
			Assert::IsTrue(testEntity.getType() == type);
		}
	}

	TEST_METHOD(getName) {
	}

	TEST_METHOD(constantTest) {
		constant a = 5;
		constant b = 5;
		Assert::IsTrue(a == b);
	}
	TEST_METHOD(procedureName) {
		proc_name a = "55";
		proc_name b = "55";
		Assert::IsTrue(a == b);
	}
	};
}