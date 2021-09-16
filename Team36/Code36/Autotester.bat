@echo off
title Iteration 1 queries

Debug\AutoTester.exe ..\Tests36\Sample_source.txt ..\Tests36\Sample_queries.txt .\Tests\out-sample.xml
Debug\AutoTester.exe ..\Tests36\Source_test_1.txt ..\Tests36\Query_test_1.txt .\Tests\out-1.xml
Debug\AutoTester.exe ..\Tests36\Source_test_2.txt ..\Tests36\Query_test_2.txt .\Tests\out-2.xml
Debug\AutoTester.exe ..\Tests36\Source_test_3.txt ..\Tests36\Query_test_3.txt .\Tests\out-3.xml
Debug\AutoTester.exe ..\Tests36\Source_test_4.txt ..\Tests36\Query_test_4.txt .\Tests\out-4.xml
Debug\AutoTester.exe ..\Tests36\Source_test_5.txt ..\Tests36\Query_test_5.txt .\Tests\out-5.xml
Debug\AutoTester.exe ..\Tests36\Source_test_6.txt ..\Tests36\Query_test_6.txt .\Tests\out-6.xml
Debug\AutoTester.exe ..\Tests36\Source_test_7.txt ..\Tests36\Query_test_7.txt .\Tests\out-7.xml
Debug\AutoTester.exe ..\Tests36\container_constants_source.txt ..\Tests36\container_constants_queries.txt .\Tests\out-container_constants.xml
Debug\AutoTester.exe ..\Tests36\multi_clauses_source.txt ..\Tests36\multi_clauses_queries.txt .\Tests\out-multi-clauses.xml

echo FINISHED
pause
exit