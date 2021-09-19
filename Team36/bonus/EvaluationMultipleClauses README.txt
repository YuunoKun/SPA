Bonus 1. Evaluation of multiple clauses (refer to EvaluatingMultipleClauses.docx for visual representation)

----------------------------------------------------------------------

Visual Studio version used : Microsoft Visual Studio Community 2019
Configuration : Debug

Instructions on compiling Autotester:
1. Open 'Team36\Code36\StartupSPASolution.sln' with Visual Studio
2. In Visual Studio toolbar, navigate to Build -> Rebuild Solution
3. Upon successful build, the Autotester executable is found in `Team36\Code36\Debug`

Instructions on testing extension
1. Open Powershell and navigate to `Team36\Code36\Debug`
2. Run the following commands:
.\AutoTester.exe ..\..\bonus\Tests\valid_query_source.txt ..\..\bonus\Tests\valid_query_queries.txt ..\..\bonus\Tests\out_valid_query.xml
.\AutoTester.exe ..\..\bonus\Tests\multi_clauses_source.txt ..\..\bonus\Tests\multi_clauses_queries.txt ..\..\bonus\Tests\out_multi_clauses.xml
3. Verify the results of the out.xml files in `Team36\bonus\Tests`.
----------------------------------------------------------------------
Explanation:
Managing multiple clauses is split into 2 parts, the table merging and the retrieving of the result.

The table merging algorithm takes care of four different scenarios. 
1.	Table a and b where there is one common column header and one of the tables
		a or b is a one-column table, filter the result.
2.	Table a and b where there are two common column headers and one of the a or
		b is a two-column table, filter the result.
3.	Table a and b where there is one common column header and both tables a and
		b have more than one column, join the table result according to the common header.
4.	Table a and b where there are two common column headers and both tables a 
		and b have more than two-column, join the table result according to the common header.

The tables are managed by a tables manager, where all tables added will be 
checked for if it is mergeable before adding to the table list. Since all 
relations and patterns will produce a table size of at most 2 columns. 
There is at most 2 common headers for any new table result. Since the 
algorithm handles the merging of the table for two common headers and below,
there can be an unlimited number of tables added as long as all tables were 
merged before they were added to the table list.

When receiving the result of e.g synonym x, the system will first check if 
there is a result (non-empty table for all filled table). If any of the tables
in the tables manager have an empty table, the system will return an empty list.
Else it will check with the table manager if there is a table header with the 
synonym x. If there is, retrieve the result from the tables manager, else 
return the unfiltered result of synonym x types.