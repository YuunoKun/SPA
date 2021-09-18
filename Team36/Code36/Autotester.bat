@echo off
title Iteration 1 queries

Debug\AutoTester.exe ..\Tests36\invalid_query_source.txt ..\Tests36\invalid_query_queries.txt .\Tests\out-1.xml


echo FINISHED
pause
exit

