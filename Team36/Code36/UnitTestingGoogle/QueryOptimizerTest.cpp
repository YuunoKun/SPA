#include "pch.h"

#include "QueryOptimizer.h"
#include <PKB.h>

namespace UnitTesting {
	TEST(QueryOptimizer, optimizeClausesOrder) {
		QueryOptimizer optimizer;
		RelRef rel_ref_no_synonym(FOLLOWS, { STMT, "1" }, { STMT, "2" });
		RelRef rel_ref_one_synonym(PARENT, { STMT, Synonym("a") }, { STMT, "2" });
		RelRef rel_ref_two_synonym(FOLLOWS_T, { STMT, Synonym("a") }, { STMT, Synonym("b") });
		Pattern pattern_no_synonym({ ASSIGN, "1" }, { WILD }, "", true);
		Pattern pattern_one_synonym({ ASSIGN, Synonym("a") }, { WILD }, "", true);
		Pattern patternTwoSynonym({ ASSIGN, Synonym("a") }, { VARIABLE, Synonym("b")}, "", true);

		std::vector<Clause> unordered_clauses, ordered_clauses;
		unordered_clauses.push_back({ rel_ref_one_synonym });
		unordered_clauses.push_back({ rel_ref_two_synonym });
		unordered_clauses.push_back({ rel_ref_no_synonym });

		ordered_clauses.push_back({ rel_ref_no_synonym });
		ordered_clauses.push_back({ rel_ref_one_synonym });
		ordered_clauses.push_back({ rel_ref_two_synonym });
		EXPECT_EQ(optimizer.optimizeClausesOrder(unordered_clauses), ordered_clauses);

		unordered_clauses.clear();
		ordered_clauses.clear();
		unordered_clauses.push_back({ patternTwoSynonym });
		unordered_clauses.push_back({ pattern_one_synonym });
		unordered_clauses.push_back({ pattern_no_synonym });

		ordered_clauses.push_back({ pattern_no_synonym });
		ordered_clauses.push_back({ pattern_one_synonym });
		ordered_clauses.push_back({ patternTwoSynonym });
		EXPECT_EQ(optimizer.optimizeClausesOrder(unordered_clauses), ordered_clauses);

		unordered_clauses.clear();
		ordered_clauses.clear();
		unordered_clauses.push_back({ rel_ref_one_synonym });
		unordered_clauses.push_back({ rel_ref_two_synonym });
		unordered_clauses.push_back({ rel_ref_no_synonym });
		unordered_clauses.push_back({ patternTwoSynonym });
		unordered_clauses.push_back({ pattern_one_synonym });
		unordered_clauses.push_back({ pattern_no_synonym });

		ordered_clauses.push_back({ rel_ref_no_synonym });
		ordered_clauses.push_back({ pattern_no_synonym });
		ordered_clauses.push_back({ rel_ref_one_synonym });
		ordered_clauses.push_back({ pattern_one_synonym });
		ordered_clauses.push_back({ rel_ref_two_synonym });
		ordered_clauses.push_back({ patternTwoSynonym });
		EXPECT_EQ(optimizer.optimizeClausesOrder(unordered_clauses), ordered_clauses);
	}

	TEST(QueryOptimizer, optimizeNextClausesOrder) {
		QueryOptimizer optimizer(false, true);
		RelRef no_synonym(NEXT_T, { STMT, "1" }, { WILD });
		RelRef one_synonym(NEXT_T, { STMT, Synonym("a") }, { WILD });
		RelRef compute_next_t_data(NEXT_T, { STMT, Synonym("a") }, { STMT, Synonym("b") });
		RelRef no_synonym_graph_search_before_precompute(NEXT_T, { STMT, "1" }, { STMT, "2" });
		RelRef one_synonym_graph_search_before_precompute1(NEXT_T, { STMT, Synonym("a") }, { STMT, "2" });
		RelRef one_synonym_graph_search_before_precompute2(NEXT_T, { STMT, "2" },  { STMT, Synonym("a") });
		RelRef two_synonym_next_after_compute(NEXT_T, { STMT, Synonym("a") }, { STMT, Synonym("b") });


		std::vector<Clause> unordered_clauses, ordered_clauses;
		unordered_clauses.push_back({ one_synonym_graph_search_before_precompute2 });
		unordered_clauses.push_back({ one_synonym_graph_search_before_precompute1 });
		unordered_clauses.push_back({ no_synonym_graph_search_before_precompute });
		unordered_clauses.push_back({ compute_next_t_data });
		unordered_clauses.push_back({ one_synonym });
		unordered_clauses.push_back({ two_synonym_next_after_compute });
		unordered_clauses.push_back({ no_synonym });

		ordered_clauses.push_back({ no_synonym });
		ordered_clauses.push_back({ one_synonym });
		ordered_clauses.push_back({ compute_next_t_data });
		ordered_clauses.push_back({ no_synonym_graph_search_before_precompute });
		ordered_clauses.push_back({ one_synonym_graph_search_before_precompute2 });
		ordered_clauses.push_back({ one_synonym_graph_search_before_precompute1 });
		ordered_clauses.push_back({ two_synonym_next_after_compute });
		EXPECT_EQ(optimizer.optimizeClausesOrder(unordered_clauses), ordered_clauses);
	}

	TEST(QueryOptimizer, optimizeAffectClausesOrder_affect_t_only) {
		QueryOptimizer optimizer(false, true);
		RelRef no_synonym1(AFFECT_T, { STMT, "1" }, { WILD });
		RelRef no_synonym2(AFFECT_T, { WILD }, { STMT, "1" });
		RelRef no_synonym3(AFFECT_T, { WILD }, { WILD });
		RelRef one_synonym1(AFFECT_T, { STMT, Synonym("a") }, { WILD });
		RelRef one_synonym2(AFFECT_T, { WILD }, { STMT, Synonym("a") });
		RelRef compute_affect_t_data(AFFECT_T, { STMT, Synonym("a") }, { STMT, Synonym("b") });
		RelRef no_synonym_graph_search_before_precompute(AFFECT_T, { STMT, "1" }, { STMT, "2" });
		RelRef one_synonym_graph_search_before_precompute1(AFFECT_T, { STMT, Synonym("a") }, { STMT, "2" });
		RelRef one_synonym_graph_search_before_precompute2(AFFECT_T, { STMT, "2" }, { STMT, Synonym("a") });
		RelRef two_synonym_after_compute(AFFECT_T, { STMT, Synonym("a") }, { STMT, Synonym("b") });


		std::vector<Clause> unordered_clauses, ordered_clauses;
		unordered_clauses.push_back({ one_synonym_graph_search_before_precompute2 });
		unordered_clauses.push_back({ one_synonym_graph_search_before_precompute1 });
		unordered_clauses.push_back({ no_synonym_graph_search_before_precompute });
		unordered_clauses.push_back({ compute_affect_t_data });
		unordered_clauses.push_back({ one_synonym1 });
		unordered_clauses.push_back({ two_synonym_after_compute });
		unordered_clauses.push_back({ no_synonym1 });
		unordered_clauses.push_back({ no_synonym2 });
		unordered_clauses.push_back({ one_synonym2 });
		unordered_clauses.push_back({ no_synonym3 });

		ordered_clauses.push_back({ compute_affect_t_data });
		ordered_clauses.push_back({ no_synonym1 });
		ordered_clauses.push_back({ no_synonym2 });
		ordered_clauses.push_back({ no_synonym3 });
		ordered_clauses.push_back({ one_synonym1 });
		ordered_clauses.push_back({ one_synonym2 });
		ordered_clauses.push_back({ no_synonym_graph_search_before_precompute });
		ordered_clauses.push_back({ one_synonym_graph_search_before_precompute2 });
		ordered_clauses.push_back({ one_synonym_graph_search_before_precompute1 });
		ordered_clauses.push_back({ two_synonym_after_compute });
		EXPECT_EQ(optimizer.optimizeClausesOrder(unordered_clauses), ordered_clauses);
	}

	TEST(QueryOptimizer, optimizeAffectClausesOrder_affect_only) {

		QueryOptimizer optimizer(false, true);
		RelRef no_synonym1(AFFECT, { WILD }, { WILD });
		RelRef no_synonym2(AFFECT, { STMT, "1" }, { WILD });
		RelRef no_synonym3(AFFECT, { WILD } , { STMT, "1" });
		RelRef one_synonym1(AFFECT, { STMT, Synonym("a") }, { WILD });
		RelRef one_synonym2(AFFECT, { WILD }, { STMT, Synonym("a") });
		RelRef compute_affect_data(AFFECT, { STMT, Synonym("a") }, { STMT, Synonym("b") });
		RelRef no_synonym0(AFFECT, { STMT, "1" }, { STMT, "2" });
		RelRef one_synonym3(AFFECT, { STMT, Synonym("a") }, { STMT, "2" });
		RelRef one_synonym4(AFFECT, { STMT, "2" }, { STMT, Synonym("a") });
		RelRef two_synonym_after_compute(AFFECT, { STMT, Synonym("a") }, { STMT, Synonym("b") });


		std::vector<Clause> unordered_clauses, ordered_clauses;
		unordered_clauses.push_back({ one_synonym1 });
		unordered_clauses.push_back({ one_synonym2 });
		unordered_clauses.push_back({ compute_affect_data });
		unordered_clauses.push_back({ no_synonym0 });
		unordered_clauses.push_back({ one_synonym3 });
		unordered_clauses.push_back({ two_synonym_after_compute });
		unordered_clauses.push_back({ no_synonym1 });
		unordered_clauses.push_back({ no_synonym2 });
		unordered_clauses.push_back({ one_synonym4 });
		unordered_clauses.push_back({ no_synonym3 });

		ordered_clauses.push_back({ compute_affect_data });
		ordered_clauses.push_back({ no_synonym0 });
		ordered_clauses.push_back({ no_synonym1 });
		ordered_clauses.push_back({ no_synonym2 });
		ordered_clauses.push_back({ no_synonym3 });
		ordered_clauses.push_back({ one_synonym1 });
		ordered_clauses.push_back({ one_synonym2 });
		ordered_clauses.push_back({ one_synonym3 });
		ordered_clauses.push_back({ one_synonym4 });
		ordered_clauses.push_back({ two_synonym_after_compute });
		EXPECT_EQ(optimizer.optimizeClausesOrder(unordered_clauses), ordered_clauses);
	}

	TEST(QueryOptimizer, optimizeAffectClausesOrder) {
		QueryOptimizer optimizer;
		RelRef affect_no_synonym1(AFFECT, { WILD }, { WILD });
		RelRef affect_no_synonym2(AFFECT, { STMT, "1" }, { WILD });
		RelRef affect_no_synonym3(AFFECT, { WILD }, { STMT, "1" });
		RelRef affect_one_synonym1(AFFECT, { STMT, Synonym("a") }, { WILD });
		RelRef affect_one_synonym2(AFFECT, { WILD }, { STMT, Synonym("a") });
		RelRef compute_affect_data(AFFECT, { STMT, Synonym("a") }, { STMT, Synonym("b") });
		RelRef affect_no_synonym0(AFFECT, { STMT, "1" }, { STMT, "2" });
		RelRef affect_one_synonym3(AFFECT, { STMT, Synonym("a") }, { STMT, "2" });
		RelRef affect_one_synonym4(AFFECT, { STMT, "2" }, { STMT, Synonym("a") });
		RelRef affect_two_synonym_after_compute(AFFECT, { STMT, Synonym("a") }, { STMT, Synonym("b") });

		RelRef affect_t_no_synonym1(AFFECT_T, { STMT, "1" }, { WILD });
		RelRef affect_t_no_synonym2(AFFECT_T, { WILD }, { STMT, "1" });
		RelRef affect_t_no_synonym3(AFFECT_T, { WILD }, { WILD });
		RelRef affect_t_one_synonym1(AFFECT_T, { STMT, Synonym("a") }, { WILD });
		RelRef affect_t_one_synonym2(AFFECT_T, { WILD }, { STMT, Synonym("a") });
		RelRef compute_affect_t_data(AFFECT_T, { STMT, Synonym("a") }, { STMT, Synonym("b") });
		RelRef affect_t_no_synonym_graph_search_before_precompute(AFFECT_T, { STMT, "1" }, { STMT, "2" });
		RelRef affect_t_one_synonym_graph_search_before_precompute1(AFFECT_T, { STMT, Synonym("a") }, { STMT, "2" });
		RelRef affect_t_one_synonym_graph_search_before_precompute2(AFFECT_T, { STMT, "2" }, { STMT, Synonym("a") });
		RelRef affect_t_two_synonym_after_compute(AFFECT_T, { STMT, Synonym("a") }, { STMT, Synonym("b") });


		std::vector<Clause> unordered_clauses, ordered_clauses;
		unordered_clauses.push_back({ affect_one_synonym1 });
		unordered_clauses.push_back({ affect_one_synonym2 });
		unordered_clauses.push_back({ compute_affect_data });
		unordered_clauses.push_back({ affect_t_no_synonym1 });
		unordered_clauses.push_back({ affect_t_one_synonym_graph_search_before_precompute2 });
		unordered_clauses.push_back({ affect_no_synonym0 });
		unordered_clauses.push_back({ affect_t_one_synonym1 });
		unordered_clauses.push_back({ affect_one_synonym3 });
		unordered_clauses.push_back({ affect_t_no_synonym_graph_search_before_precompute });
		unordered_clauses.push_back({ affect_two_synonym_after_compute });
		unordered_clauses.push_back({ compute_affect_t_data });
		unordered_clauses.push_back({ affect_t_one_synonym_graph_search_before_precompute1 });
		unordered_clauses.push_back({ affect_no_synonym1 });
		unordered_clauses.push_back({ affect_t_no_synonym2 });
		unordered_clauses.push_back({ affect_no_synonym2 });
		unordered_clauses.push_back({ affect_t_one_synonym2 });
		unordered_clauses.push_back({ affect_one_synonym4 });
		unordered_clauses.push_back({ affect_no_synonym3 });
		unordered_clauses.push_back({ affect_t_two_synonym_after_compute });
		unordered_clauses.push_back({ affect_t_no_synonym3 });


		ordered_clauses.push_back({ compute_affect_t_data });

		ordered_clauses.push_back({ affect_t_no_synonym1 });
		ordered_clauses.push_back({ affect_no_synonym0 });
		ordered_clauses.push_back({ affect_no_synonym1 });
		ordered_clauses.push_back({ affect_t_no_synonym2 });
		ordered_clauses.push_back({ affect_no_synonym2 });
		ordered_clauses.push_back({ affect_no_synonym3 });
		ordered_clauses.push_back({ affect_t_no_synonym3 });

		ordered_clauses.push_back({ affect_one_synonym1 });
		ordered_clauses.push_back({ affect_one_synonym2 });
		ordered_clauses.push_back({ affect_t_one_synonym1 });
		ordered_clauses.push_back({ affect_one_synonym3 });
		ordered_clauses.push_back({ affect_t_one_synonym2 });
		ordered_clauses.push_back({ affect_one_synonym4 });

		ordered_clauses.push_back({ affect_two_synonym_after_compute });
		ordered_clauses.push_back({ compute_affect_data });

		ordered_clauses.push_back({ affect_t_no_synonym_graph_search_before_precompute });
		ordered_clauses.push_back({ affect_t_one_synonym_graph_search_before_precompute2 });
		ordered_clauses.push_back({ affect_t_one_synonym_graph_search_before_precompute1 });
		ordered_clauses.push_back({ affect_t_two_synonym_after_compute });

		EXPECT_EQ(optimizer.optimizeClausesOrder(unordered_clauses), ordered_clauses);
	}


	TEST(QueryOptimizer, sortClausesOrderByRelationType) {
		QueryOptimizer optimizer(false, true);

		RelRef rel_ref_no_synonym(FOLLOWS, { STMT, "1" }, { STMT, "2" });
		RelRef rel_ref_one_synonym(PARENT, { STMT, Synonym("a") }, { STMT, "2" });
		RelRef rel_ref_two_synonym(FOLLOWS_T, { STMT, Synonym("a") }, { STMT, Synonym("b") });
		Pattern pattern_one_synonym({ ASSIGN, Synonym("a") }, { WILD }, "", true);
		Pattern pattern_two_synonym({ ASSIGN, Synonym("a") }, { VARIABLE, Synonym("b") }, "", true);

		RelRef next_t_no_synonym(NEXT_T, { STMT, "1" }, { WILD });
		RelRef next_t_one_synonym(NEXT_T, { STMT, Synonym("a") }, { WILD });
		RelRef compute_next_t_data(NEXT_T, { STMT, Synonym("a") }, { STMT, Synonym("b") });
		RelRef next_t_no_synonym_graph_search_before_precompute(NEXT_T, { STMT, "1" }, { STMT, "2" });
		RelRef next_t_one_synonym_graph_search_before_precompute1(NEXT_T, { STMT, Synonym("a") }, { STMT, "2" });
		RelRef next_t_one_synonym_graph_search_before_precompute2(NEXT_T, { STMT, "2" }, { STMT, Synonym("a") });
		RelRef next_t_two_synonym_next_after_compute(NEXT_T, { STMT, Synonym("a") }, { STMT, Synonym("b") });

		RelRef affect_no_synonym1(AFFECT, { WILD }, { WILD });
		RelRef affect_no_synonym2(AFFECT, { STMT, "1" }, { WILD });
		RelRef affect_no_synonym3(AFFECT, { WILD }, { STMT, "1" });
		RelRef affect_one_synonym1(AFFECT, { STMT, Synonym("a") }, { WILD });
		RelRef affect_one_synonym2(AFFECT, { WILD }, { STMT, Synonym("a") });
		RelRef compute_affect_data(AFFECT, { STMT, Synonym("a") }, { STMT, Synonym("b") });
		RelRef affect_no_synonym0(AFFECT, { STMT, "1" }, { STMT, "2" });
		RelRef affect_one_synonym3(AFFECT, { STMT, Synonym("a") }, { STMT, "2" });
		RelRef affect_one_synonym4(AFFECT, { STMT, "2" }, { STMT, Synonym("a") });
		RelRef affect_two_synonym_after_compute(AFFECT, { STMT, Synonym("a") }, { STMT, Synonym("b") });

		RelRef affect_t_no_synonym1(AFFECT_T, { STMT, "1" }, { WILD });
		RelRef affect_t_no_synonym2(AFFECT_T, { WILD }, { STMT, "1" });
		RelRef affect_t_no_synonym3(AFFECT_T, { WILD }, { WILD });
		RelRef affect_t_one_synonym1(AFFECT_T, { STMT, Synonym("a") }, { WILD });
		RelRef affect_t_one_synonym2(AFFECT_T, { WILD }, { STMT, Synonym("a") });
		RelRef compute_affect_t_data(AFFECT_T, { STMT, Synonym("a") }, { STMT, Synonym("b") });
		RelRef affect_t_no_synonym_graph_search_before_precompute(AFFECT_T, { STMT, "1" }, { STMT, "2" });
		RelRef affect_t_one_synonym_graph_search_before_precompute1(AFFECT_T, { STMT, Synonym("a") }, { STMT, "2" });
		RelRef affect_t_one_synonym_graph_search_before_precompute2(AFFECT_T, { STMT, "2" }, { STMT, Synonym("a") });
		RelRef affect_t_two_synonym_after_compute(AFFECT_T, { STMT, Synonym("a") }, { STMT, Synonym("b") });


		std::vector<Clause> unordered_clauses, ordered_clauses;

		unordered_clauses.push_back({ affect_one_synonym1 });
		unordered_clauses.push_back({ affect_one_synonym2 });
		unordered_clauses.push_back({ next_t_one_synonym_graph_search_before_precompute2 });
		unordered_clauses.push_back({ compute_affect_data });
		unordered_clauses.push_back({ affect_t_no_synonym1 });
		unordered_clauses.push_back({ affect_t_one_synonym_graph_search_before_precompute2 });
		unordered_clauses.push_back({ affect_no_synonym0 });
		unordered_clauses.push_back({ affect_t_one_synonym1 });
		unordered_clauses.push_back({ rel_ref_one_synonym });
		unordered_clauses.push_back({ next_t_one_synonym_graph_search_before_precompute1 });
		unordered_clauses.push_back({ rel_ref_no_synonym });
		unordered_clauses.push_back({ next_t_no_synonym_graph_search_before_precompute });
		unordered_clauses.push_back({ affect_one_synonym3 });
		unordered_clauses.push_back({ rel_ref_two_synonym });
		unordered_clauses.push_back({ affect_t_no_synonym_graph_search_before_precompute });
		unordered_clauses.push_back({ compute_next_t_data });
		unordered_clauses.push_back({ affect_two_synonym_after_compute });
		unordered_clauses.push_back({ compute_affect_t_data });
		unordered_clauses.push_back({ next_t_one_synonym });
		unordered_clauses.push_back({ affect_t_one_synonym_graph_search_before_precompute1 });
		unordered_clauses.push_back({ next_t_two_synonym_next_after_compute });
		unordered_clauses.push_back({ affect_no_synonym1 });
		unordered_clauses.push_back({ affect_t_no_synonym2 });
		unordered_clauses.push_back({ affect_no_synonym2 });
		unordered_clauses.push_back({ affect_t_one_synonym2 });
		unordered_clauses.push_back({ affect_one_synonym4 });
		unordered_clauses.push_back({ pattern_two_synonym });
		unordered_clauses.push_back({ next_t_no_synonym });
		unordered_clauses.push_back({ affect_no_synonym3 });
		unordered_clauses.push_back({ affect_t_two_synonym_after_compute });
		unordered_clauses.push_back({ affect_t_no_synonym3 });
		unordered_clauses.push_back({ pattern_one_synonym });


		ordered_clauses.push_back({ rel_ref_no_synonym });
		ordered_clauses.push_back({ next_t_no_synonym });
		ordered_clauses.push_back({ rel_ref_one_synonym });
		ordered_clauses.push_back({ next_t_one_synonym });
		ordered_clauses.push_back({ pattern_one_synonym });
		ordered_clauses.push_back({ rel_ref_two_synonym });
		ordered_clauses.push_back({ pattern_two_synonym });
		ordered_clauses.push_back({ compute_next_t_data });
		ordered_clauses.push_back({ next_t_no_synonym_graph_search_before_precompute });
		ordered_clauses.push_back({ next_t_one_synonym_graph_search_before_precompute2 });
		ordered_clauses.push_back({ next_t_one_synonym_graph_search_before_precompute1 });
		ordered_clauses.push_back({ next_t_two_synonym_next_after_compute });

		ordered_clauses.push_back({ compute_affect_t_data });

		ordered_clauses.push_back({ affect_t_no_synonym1 });
		ordered_clauses.push_back({ affect_no_synonym0 });
		ordered_clauses.push_back({ affect_no_synonym1 });
		ordered_clauses.push_back({ affect_t_no_synonym2 });
		ordered_clauses.push_back({ affect_no_synonym2 });
		ordered_clauses.push_back({ affect_no_synonym3 });
		ordered_clauses.push_back({ affect_t_no_synonym3 });

		ordered_clauses.push_back({ affect_one_synonym1 });
		ordered_clauses.push_back({ affect_one_synonym2 });
		ordered_clauses.push_back({ affect_t_one_synonym1 });
		ordered_clauses.push_back({ affect_one_synonym3 });
		ordered_clauses.push_back({ affect_t_one_synonym2 });
		ordered_clauses.push_back({ affect_one_synonym4 });

		ordered_clauses.push_back({ affect_two_synonym_after_compute });
		ordered_clauses.push_back({ compute_affect_data });

		ordered_clauses.push_back({ affect_t_no_synonym_graph_search_before_precompute });
		ordered_clauses.push_back({ affect_t_one_synonym_graph_search_before_precompute2 });
		ordered_clauses.push_back({ affect_t_one_synonym_graph_search_before_precompute1 });
		ordered_clauses.push_back({ affect_t_two_synonym_after_compute });
		EXPECT_EQ(optimizer.optimizeClausesOrder(unordered_clauses), ordered_clauses);
	}



	TEST(QueryOptimizer, optmizeClausesWithAffectThreshold) {
		QueryOptimizer optimizer(false, false, true, 3);

		RelRef affect_one_synonym1(AFFECT, { STMT, Synonym("a") }, { WILD });
		RelRef affect_one_synonym2(AFFECT, { WILD }, { STMT, Synonym("a") });
		RelRef affect_one_synonym3(AFFECT, { STMT, Synonym("a") }, { STMT, "2" });
		RelRef affect_two_synonym(AFFECT, { STMT, Synonym("a") }, { STMT, Synonym("b") });
		RelRef affect_two_synonym_dummy(AFFECT, { ASSIGN, Synonym() }, { ASSIGN, Synonym() });

		RelRef affect_t_no_synonym1(AFFECT_T, { STMT, "1" }, { WILD });
		RelRef affect_t_no_synonym_graph_search_before_precompute(AFFECT_T, { STMT, "1" }, { STMT, "2" });
		RelRef affect_t_one_synonym_graph_search_before_precompute1(AFFECT_T, { STMT, Synonym("a") }, { STMT, "2" });
		RelRef affect_t_one_synonym_graph_search_before_precompute2(AFFECT_T, { STMT, "2" }, { STMT, Synonym("a") });
		RelRef affect_t_two_synonym(AFFECT_T, { STMT, Synonym("a") }, { STMT, Synonym("b") });
		RelRef affect_t_two_synonym_dummy(AFFECT_T, { ASSIGN, Synonym() }, { ASSIGN, Synonym() });


		std::vector<Clause> unordered_clauses, ordered_clauses;

		unordered_clauses.push_back({ affect_one_synonym1 });
		unordered_clauses.push_back({ affect_one_synonym2 });
		unordered_clauses.push_back({ affect_one_synonym3 });

		ordered_clauses.push_back({ affect_one_synonym1 });
		ordered_clauses.push_back({ affect_one_synonym2 });
		ordered_clauses.push_back({ affect_one_synonym3 });

		EXPECT_EQ(optimizer.optimizeClausesOrder(unordered_clauses), ordered_clauses);

		unordered_clauses.clear();
		ordered_clauses.clear();
		unordered_clauses.push_back({ affect_one_synonym1 });
		unordered_clauses.push_back({ affect_one_synonym2 });
		unordered_clauses.push_back({ affect_one_synonym3 });
		unordered_clauses.push_back({ affect_t_no_synonym1 });
		unordered_clauses.push_back({ affect_two_synonym });

		ordered_clauses.push_back({ affect_one_synonym1 });
		ordered_clauses.push_back({ affect_one_synonym2 });
		ordered_clauses.push_back({ affect_one_synonym3 });
		ordered_clauses.push_back({ affect_t_no_synonym1 });
		ordered_clauses.push_back({ affect_two_synonym });

		EXPECT_EQ(optimizer.optimizeClausesOrder(unordered_clauses), ordered_clauses);

		unordered_clauses.clear();
		ordered_clauses.clear();
		unordered_clauses.push_back({ affect_one_synonym1 });
		unordered_clauses.push_back({ affect_one_synonym2 });
		unordered_clauses.push_back({ affect_one_synonym3 });
		unordered_clauses.push_back({ affect_t_no_synonym1 });

		ordered_clauses.push_back({ affect_one_synonym1 });
		ordered_clauses.push_back({ affect_one_synonym2 });
		ordered_clauses.push_back({ affect_one_synonym3 });
		ordered_clauses.push_back({ affect_t_no_synonym1 });
		ordered_clauses.push_back({ affect_two_synonym_dummy });

		EXPECT_EQ(optimizer.optimizeClausesOrder(unordered_clauses), ordered_clauses);

		unordered_clauses.clear();
		ordered_clauses.clear();
		unordered_clauses.push_back({ affect_one_synonym1 });
		unordered_clauses.push_back({ affect_one_synonym2 });
		unordered_clauses.push_back({ affect_one_synonym3 });
		unordered_clauses.push_back({ affect_t_no_synonym_graph_search_before_precompute });

		ordered_clauses.push_back({ affect_one_synonym1 });
		ordered_clauses.push_back({ affect_one_synonym2 });
		ordered_clauses.push_back({ affect_one_synonym3 });
		ordered_clauses.push_back({ affect_t_no_synonym_graph_search_before_precompute });

		EXPECT_EQ(optimizer.optimizeClausesOrder(unordered_clauses), ordered_clauses);


		unordered_clauses.clear();
		ordered_clauses.clear();
		unordered_clauses.push_back({ affect_one_synonym1 });
		unordered_clauses.push_back({ affect_one_synonym2 });
		unordered_clauses.push_back({ affect_one_synonym3 });
		unordered_clauses.push_back({ affect_t_no_synonym_graph_search_before_precompute });
		unordered_clauses.push_back({ affect_t_one_synonym_graph_search_before_precompute1 });
		unordered_clauses.push_back({ affect_t_one_synonym_graph_search_before_precompute2 });
		unordered_clauses.push_back({ affect_t_no_synonym_graph_search_before_precompute });

		ordered_clauses.push_back({ affect_one_synonym1 });
		ordered_clauses.push_back({ affect_one_synonym2 });
		ordered_clauses.push_back({ affect_one_synonym3 });
		ordered_clauses.push_back({ affect_t_no_synonym_graph_search_before_precompute });
		ordered_clauses.push_back({ affect_t_one_synonym_graph_search_before_precompute1 });
		ordered_clauses.push_back({ affect_t_one_synonym_graph_search_before_precompute2 });
		ordered_clauses.push_back({ affect_t_no_synonym_graph_search_before_precompute });
		ordered_clauses.push_back({ affect_t_two_synonym_dummy });

		EXPECT_EQ(optimizer.optimizeClausesOrder(unordered_clauses), ordered_clauses);

		unordered_clauses.clear();
		ordered_clauses.clear();
		unordered_clauses.push_back({ affect_one_synonym1 });
		unordered_clauses.push_back({ affect_one_synonym2 });
		unordered_clauses.push_back({ affect_one_synonym3 });
		unordered_clauses.push_back({ affect_t_no_synonym_graph_search_before_precompute });
		unordered_clauses.push_back({ affect_t_one_synonym_graph_search_before_precompute1 });
		unordered_clauses.push_back({ affect_t_one_synonym_graph_search_before_precompute2 });
		unordered_clauses.push_back({ affect_t_no_synonym_graph_search_before_precompute });
		unordered_clauses.push_back({ affect_t_two_synonym });

		ordered_clauses.push_back({ affect_one_synonym1 });
		ordered_clauses.push_back({ affect_one_synonym2 });
		ordered_clauses.push_back({ affect_one_synonym3 });
		ordered_clauses.push_back({ affect_t_no_synonym_graph_search_before_precompute });
		ordered_clauses.push_back({ affect_t_one_synonym_graph_search_before_precompute1 });
		ordered_clauses.push_back({ affect_t_one_synonym_graph_search_before_precompute2 });
		ordered_clauses.push_back({ affect_t_no_synonym_graph_search_before_precompute });
		ordered_clauses.push_back({ affect_t_two_synonym });

		EXPECT_EQ(optimizer.optimizeClausesOrder(unordered_clauses), ordered_clauses);

	}


	TEST(QueryOptimizer, checkConstantExist) {
		PKB::getInstance().resetCache();
		QueryOptimizer optimizer;

		RelRef stmt_constant_synonym1(FOLLOWS, { STMT, Synonym("a") }, { STMT, "1" });
		RelRef stmt_constant_synonym2(FOLLOWS, { IF, "1"}, {STMT, Synonym("a") });
		RelRef if_constant_synonym(FOLLOWS, { STMT, Synonym("a") }, { STMT, "1" });
		Pattern stmt_constant_synonym3({ ASSIGN, "1" }, { WILD }, "", true);

		std::vector<Clause> clauses;
		clauses.push_back({ { FOLLOWS, { STMT, Synonym("a") }, { STMT, "1" } } });
		clauses.push_back({ { FOLLOWS, { STMT, Synonym("a") }, { IF, "1" } } });
		EXPECT_FALSE(optimizer.checkAllConstantExist(clauses));
		PKB::getInstance().addStmt(STMT_IF);
		EXPECT_TRUE(optimizer.checkAllConstantExist(clauses));

		clauses.push_back({ { FOLLOWS, { STMT, Synonym("a") }, { IF, "2" } } });
		EXPECT_FALSE(optimizer.checkAllConstantExist(clauses));
		PKB::getInstance().addStmt(STMT_IF);
		EXPECT_TRUE(optimizer.checkAllConstantExist(clauses));


		clauses.push_back({ { FOLLOWS, { STMT, Synonym("a") }, { READ, "3" } } });
		EXPECT_FALSE(optimizer.checkAllConstantExist(clauses));
		PKB::getInstance().addStmt(STMT_READ);
		EXPECT_TRUE(optimizer.checkAllConstantExist(clauses));
		clauses.push_back({ { FOLLOWS, { STMT, Synonym("a") }, { READ, "4" } } });
		EXPECT_FALSE(optimizer.checkAllConstantExist(clauses));
		PKB::getInstance().addStmt(STMT_READ);
		EXPECT_TRUE(optimizer.checkAllConstantExist(clauses));

		clauses.push_back({ { FOLLOWS, { STMT, Synonym("a") }, { PRINT, "5" } } });
		EXPECT_FALSE(optimizer.checkAllConstantExist(clauses));
		PKB::getInstance().addStmt(STMT_PRINT);
		EXPECT_TRUE(optimizer.checkAllConstantExist(clauses));
		clauses.push_back({ { FOLLOWS, { STMT, Synonym("a") }, { PRINT, "6" } } });
		EXPECT_FALSE(optimizer.checkAllConstantExist(clauses));
		PKB::getInstance().addStmt(STMT_PRINT);
		EXPECT_TRUE(optimizer.checkAllConstantExist(clauses));

		clauses.push_back({ { FOLLOWS, { STMT, Synonym("a") }, { CALL, "7" } } });
		EXPECT_FALSE(optimizer.checkAllConstantExist(clauses));
		PKB::getInstance().addStmt(STMT_CALL);
		EXPECT_TRUE(optimizer.checkAllConstantExist(clauses));
		clauses.push_back({ { FOLLOWS, { STMT, Synonym("a") }, { CALL, "8" } } });
		EXPECT_FALSE(optimizer.checkAllConstantExist(clauses));
		PKB::getInstance().addStmt(STMT_CALL);
		EXPECT_TRUE(optimizer.checkAllConstantExist(clauses));

		clauses.push_back({ { FOLLOWS, { STMT, Synonym("a") }, { WHILE, "9" } } });
		EXPECT_FALSE(optimizer.checkAllConstantExist(clauses));
		PKB::getInstance().addStmt(STMT_WHILE);
		EXPECT_TRUE(optimizer.checkAllConstantExist(clauses));
		clauses.push_back({ { FOLLOWS, { STMT, Synonym("a") }, { WHILE, "10" } } });
		EXPECT_FALSE(optimizer.checkAllConstantExist(clauses));
		PKB::getInstance().addStmt(STMT_WHILE);
		EXPECT_TRUE(optimizer.checkAllConstantExist(clauses));

		clauses.push_back({ { FOLLOWS, { STMT, Synonym("a") }, { IF, "11" } } });
		EXPECT_FALSE(optimizer.checkAllConstantExist(clauses));
		PKB::getInstance().addStmt(STMT_IF);
		EXPECT_TRUE(optimizer.checkAllConstantExist(clauses));
		clauses.push_back({ { FOLLOWS, { STMT, Synonym("a") }, { IF, "12" } } });
		EXPECT_FALSE(optimizer.checkAllConstantExist(clauses));
		PKB::getInstance().addStmt(STMT_IF);
		EXPECT_TRUE(optimizer.checkAllConstantExist(clauses));

		clauses.push_back({ { FOLLOWS, { STMT, Synonym("a") }, { ASSIGN, "13" } } });
		EXPECT_FALSE(optimizer.checkAllConstantExist(clauses));
		PKB::getInstance().addStmt(STMT_ASSIGN);
		EXPECT_TRUE(optimizer.checkAllConstantExist(clauses));
		clauses.push_back({ { FOLLOWS, { STMT, Synonym("a") }, { ASSIGN, "14" } } });
		EXPECT_FALSE(optimizer.checkAllConstantExist(clauses));
		PKB::getInstance().addStmt(STMT_ASSIGN);
		EXPECT_TRUE(optimizer.checkAllConstantExist(clauses));

		clauses.push_back({ {{ ASSIGN, Synonym("a") }, { VARIABLE ,"x"}, "", true} });
		EXPECT_FALSE(optimizer.checkAllConstantExist(clauses));
		PKB::getInstance().addVariable("x");
		EXPECT_TRUE(optimizer.checkAllConstantExist(clauses));

		clauses.push_back({ { CALLS, { PROCEDURE, "x" }, { PROCEDURE, Synonym("a") } } });
		EXPECT_FALSE(optimizer.checkAllConstantExist(clauses));
		PKB::getInstance().addProcedure("x");
		EXPECT_TRUE(optimizer.checkAllConstantExist(clauses));


	}

}