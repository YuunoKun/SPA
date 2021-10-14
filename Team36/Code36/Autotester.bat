@echo off
title Iteration 1 queries

Debug\AutoTester.exe ..\Tests36\Sample_source.txt ..\Tests36\Sample_queries.txt .\Tests\out-sample.xml
Debug\AutoTester.exe ..\Tests36\iter1_no_clause_source_1.txt ..\Tests36\iter1_no_clause_queries_1.txt .\Tests\out-1-iter1_no_clause_queries_1.xml
Debug\AutoTester.exe ..\Tests36\iter1_no_clause_source_2.txt ..\Tests36\iter1_no_clause_queries_2.txt .\Tests\out-2-iter1_no_clause_queries_2.xml
Debug\AutoTester.exe ..\Tests36\iter1_no_clause_source_3.txt ..\Tests36\iter1_no_clause_queries_3.txt .\Tests\out-3-iter1_no_clause_queries_3.xml
Debug\AutoTester.exe ..\Tests36\iter1_no_clause_source_4.txt ..\Tests36\iter1_no_clause_queries_4.txt .\Tests\out-4-iter1_no_clause_queries_4.xml
Debug\AutoTester.exe ..\Tests36\iter1_one_clause_source.txt ..\Tests36\iter1_one_clause_queries.txt .\Tests\out-5-iter1_one_clause_queries.xml
Debug\AutoTester.exe ..\Tests36\iter1_two_clauses_source.txt ..\Tests36\iter1_two_clauses_queries.txt .\Tests\out-6-iter1_two_clauses_queries.xml
Debug\AutoTester.exe ..\Tests36\iter1_nested_while_source.txt ..\Tests36\iter1_nested_while_queries.txt .\Tests\out-7-iter1_nested_while_queries.xml
Debug\AutoTester.exe ..\Tests36\iter1_single_procedure_source.txt ..\Tests36\iter1_single_procedure_queries.txt .\Tests\out-8-iter1_single_procedure_queries.xml
Debug\AutoTester.exe ..\Tests36\iter2_source.txt ..\Tests36\iter2_queries.txt .\Tests\out-9-iter2_queries.xml
Debug\AutoTester.exe ..\Tests36\iter2_calls_source.txt ..\Tests36\iter2_calls_queries.txt .\Tests\out-10-iter2_calls_queries.xml
Debug\AutoTester.exe ..\Tests36\iter2_no_call_source.txt ..\Tests36\iter2_no_call_queries.txt .\Tests\out-11-iter2_no_call_queries.xml
Debug\AutoTester.exe ..\Tests36\iter2_multiprocedure_source.txt ..\Tests36\iter2_multiprocedure_queries.txt .\Tests\out-12-iter2_multiprocedure_queries.xml
Debug\AutoTester.exe ..\Tests36\container_constants_source.txt ..\Tests36\container_constants_queries.txt .\Tests\out-container_constants.xml
Debug\AutoTester.exe ..\Tests36\multi_clauses_source.txt ..\Tests36\multi_clauses_queries.txt .\Tests\out-multi-clauses.xml
Debug\AutoTester.exe ..\Tests36\valid_query_source.txt ..\Tests36\valid_query_queries.txt .\Tests\out-valid-query.xml
Debug\AutoTester.exe ..\Tests36\invalid_query_source.txt ..\Tests36\invalid_query_queries.txt .\Tests\out-invalid-query.xml

echo FINISHED
pause
exit

