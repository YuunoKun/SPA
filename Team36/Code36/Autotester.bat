@echo off
title Iteration 1 queries

Debug\AutoTester.exe ..\Tests36\Sample_source.txt ..\Tests36\Sample_queries.txt .\Tests\out-sample.xml
Debug\AutoTester.exe ..\Tests36\iter1_no_clause_source_1.txt ..\Tests36\iter1_no_clause_queries_1.txt .\Tests\out-1-iter1_no_clause_queries_1.xml
Debug\AutoTester.exe ..\Tests36\iter1_no_clause_source_2.txt ..\Tests36\iter1_no_clause_queries_2.txt .\Tests\out-2-iter1_no_clause_queries_2.xml
Debug\AutoTester.exe ..\Tests36\iter1_no_clause_source_3.txt ..\Tests36\iter1_no_clause_queries_3.txt .\Tests\out-3-iter1_no_clause_queries_3.xml
Debug\AutoTester.exe ..\Tests36\iter1_no_clause_source_4.txt ..\Tests36\iter1_no_clause_queries_4.txt .\Tests\out-4-iter1_no_clause_queries_4.xml
Debug\AutoTester.exe ..\Tests36\iter1_ModifiesS_source.txt ..\Tests36\iter1_ModifiesS_queries.txt .\Tests\out-5-iter1_ModifiesS_queries.xml
Debug\AutoTester.exe ..\Tests36\iter1_UsesS_source.txt ..\Tests36\iter1_UsesS_queries.txt .\Tests\out-6-iter1_UsesS_queries.xml
Debug\AutoTester.exe ..\Tests36\iter1_parent_source.txt ..\Tests36\iter1_parent_queries.txt .\Tests\out-7-iter1_parent_queries.xml
Debug\AutoTester.exe ..\Tests36\iter1_parentT_source.txt ..\Tests36\iter1_parentT_queries.txt .\Tests\out-8-iter1_parentT_queries.xml
Debug\AutoTester.exe ..\Tests36\iter1_follows_source.txt ..\Tests36\iter1_follows_queries.txt .\Tests\out-9-iter1_follows_queries.xml
Debug\AutoTester.exe ..\Tests36\iter1_followsT_source.txt ..\Tests36\iter1_followsT_queries.txt .\Tests\out-10-iter1_followsT_queries.xml
Debug\AutoTester.exe ..\Tests36\iter1_patternA_source.txt ..\Tests36\iter1_patternA_queries.txt .\Tests\out-11-iter1_patternA_queries.xml
Debug\AutoTester.exe ..\Tests36\iter1_two_clauses_source.txt ..\Tests36\iter1_two_clauses_queries.txt .\Tests\out-12-iter1_two_clauses_queries.xml
Debug\AutoTester.exe ..\Tests36\iter1_nested_while_source.txt ..\Tests36\iter1_nested_while_queries.txt .\Tests\out-13-iter1_nested_while_queries.xml
Debug\AutoTester.exe ..\Tests36\iter1_single_procedure_source.txt ..\Tests36\iter1_single_procedure_queries.txt .\Tests\out-14-iter1_single_procedure_queries.xml
Debug\AutoTester.exe ..\Tests36\iter2_and_source.txt ..\Tests36\iter2_and_queries.txt .\Tests\out-15-iter2_and_queries.xml
Debug\AutoTester.exe ..\Tests36\iter2_calls_source.txt ..\Tests36\iter2_calls_queries.txt .\Tests\out-16-iter2_calls_queries.xml
Debug\AutoTester.exe ..\Tests36\iter2_callsT_source.txt ..\Tests36\iter2_callsT_queries.txt .\Tests\out-17-iter2_callsT_queries.xml
Debug\AutoTester.exe ..\Tests36\iter2_no_call_source.txt ..\Tests36\iter2_no_call_queries.txt .\Tests\out-18-iter2_no_call_queries.xml
Debug\AutoTester.exe ..\Tests36\iter2_ModifiesP_source.txt ..\Tests36\iter2_ModifiesP_queries.txt .\Tests\out-19-iter2_ModifiesP_queries.xml
Debug\AutoTester.exe ..\Tests36\iter2_UsesP_source.txt ..\Tests36\iter2_UsesP_queries.txt .\Tests\out-20-iter2_UsesP_queries.xml
Debug\AutoTester.exe ..\Tests36\iter2_next_source.txt ..\Tests36\iter2_next_queries.txt .\Tests\out-21-iter2_next_queries.xml
Debug\AutoTester.exe ..\Tests36\iter2_patternA_source.txt ..\Tests36\iter2_patternA_queries.txt .\Tests\out-22-iter2_patternA_queries.xml
Debug\AutoTester.exe ..\Tests36\iter2_patternIF_source.txt ..\Tests36\iter2_patternIF_queries.txt .\Tests\out-23-iter2_patternIF_queries.xml
Debug\AutoTester.exe ..\Tests36\iter2_patternWhile_source.txt ..\Tests36\iter2_patternWhile_queries.txt .\Tests\out-24-iter2_patternWhile_queries.xml
Debug\AutoTester.exe ..\Tests36\iter2_multi_clauses_source.txt ..\Tests36\iter2_multi_clauses_queries.txt .\Tests\out-25-iter2_multi_clauses_queries.xml
Debug\AutoTester.exe ..\Tests36\iter3_nextT_source.txt ..\Tests36\iter3_nextT_queries.txt .\Tests\out-26-iter3_nextT_queries.xml
Debug\AutoTester.exe ..\Tests36\iter3_boolean_source.txt ..\Tests36\iter3_boolean_queries.txt .\Tests\out-27-iter3_boolean_queries.xml
Debug\AutoTester.exe ..\Tests36\iter3_tuple_source.txt ..\Tests36\iter3_tuple_queries.txt .\Tests\out-28-iter3_tuple_queries.xml
Debug\AutoTester.exe ..\Tests36\iter3_with_source.txt ..\Tests36\iter3_with_queries.txt .\Tests\out-29-iter3_with_queries.xml
Debug\AutoTester.exe ..\Tests36\iter3_Affects_source.txt ..\Tests36\iter3_Affects_queries.txt .\Tests\out-30-iter3_Affects_queries.xml
Debug\AutoTester.exe ..\Tests36\iter3_AffectsT_source.txt ..\Tests36\iter3_AffectsT_queries.txt .\Tests\out-31-iter3_AffectsT_queries.xml
Debug\AutoTester.exe ..\Tests36\iter3_nextbip_source.txt ..\Tests36\iter3_nextbip_queries.txt .\Tests\out-32-iter3_nextbip_queries.xml
Debug\AutoTester.exe ..\Tests36\iter3_nextbipT_source.txt ..\Tests36\iter3_nextbipT_queries.txt .\Tests\out-33-iter3_nextbipT_queries.xml
Debug\AutoTester.exe ..\Tests36\container_constants_source.txt ..\Tests36\container_constants_queries.txt .\Tests\out-container_constants.xml
Debug\AutoTester.exe ..\Tests36\multi_clauses_source.txt ..\Tests36\multi_clauses_queries.txt .\Tests\out-multi-clauses.xml
Debug\AutoTester.exe ..\Tests36\valid_query_source.txt ..\Tests36\valid_query_queries.txt .\Tests\out-valid-query.xml
Debug\AutoTester.exe ..\Tests36\invalid_query_source.txt ..\Tests36\invalid_query_queries.txt .\Tests\out-invalid-query.xml

echo FINISHED
pause
exit

