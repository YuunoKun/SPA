@echo off
title Stress Test queries

Debug\AutoTester.exe ..\Tests36\stress_test_50_source.txt ..\Tests36\stress_test_queries.txt .\Tests\stress-test-1-query.xml
Debug\AutoTester.exe ..\Tests36\stress_test_250_source.txt ..\Tests36\stress_test_queries.txt .\Tests\stress-test-2-query.xml
Debug\AutoTester.exe ..\Tests36\stress_test_500_source.txt ..\Tests36\stress_test_queries.txt .\Tests\stress-test-3-query.xml

echo FINISHED
pause
exit

