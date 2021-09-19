Bonus 3. Extraction and population of UsesP and ModifiesP relations in PKB
----------------------------------------------------------------------
Visual Studio version used : Microsoft Visual Studio Community 2019
Configuration : Debug

Instructions on running integration test:
1. Open 'Team36\Code36\StartupSPASolution.sln' with Visual Studio
2. In Visual Studio toolbar, navigate to Build -> Rebuild Solution
3. In Solution Explorer, navigate to IntegrationTestingGoogle -> ParserPKBTest_bonus.cpp
4. Right click on it and select 'Run Tests'

Expected behaviour: All 4 testcases pass

Test 1: Source with no procedure calls is parsed and UsesP/ModifiesP relations are successfully populated into PKB
Source file: Team36/Code36/IntegrationTestingGoogle/Tests/no_procedure_call_source.txt

Expected UsesP: [mySecondProcedure, x], [mySecondProcedure, y]
Expected ModifiesP: [myFirstProcedure, x], [myFirstProcedure, y],
			[mySecondProcedure, z], [mySecondProcedure, x], [mySecondProcedure, w]


Test 2: Source with procedure calls is parsed and UsesP/ModifiesP relations are successfully populated into PKB
Source file: Team36/Code36/IntegrationTestingGoogle/Tests/procedure_call_source.txt

Expected UsesP: [main, x], [main, y], [main, count], [main, cenX], [main, cenY], 
		[printResults, cenX], [printResults, cenY], 
		[computeCentroid, x], [computeCentroid, y], [computeCentroid, count], [computeCentroid, cenX], [computeCentroid, cenY]
Expected ModifiesP: [main, count], [main, x], [main, flag],[main, cenX],[main, normSq],
		    [readPoint, x], 
		    [computeCentroid, count], [computeCentroid, x], [computeCentroid, flag], [computeCentroid, cenX],[computeCentroid, normSq]