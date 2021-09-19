Bonus 2. Source Processor Validation: detection of 
   - duplicate procedure names
   - recursive calls
   - cyclic calls
   - calls to non-existing procedure

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
.\AutoTester.exe ..\..\bonus\Tests\bonus2_duplicate_procedure_name_source.txt ..\..\bonus\Tests\bonus2_no_queries.txt ..\..\bonus\Tests\bonus2_no_query.xml
.\AutoTester.exe ..\..\bonus\Tests\bonus2_call_undefined_procedure_source.txt ..\..\bonus\Tests\bonus2_no_queries.txt ..\..\bonus\Tests\bonus2_no_query.xml
.\AutoTester.exe ..\..\bonus\Tests\bonus2_recursive_calls_source.txt ..\..\bonus\Tests\bonus2_no_queries.txt ..\..\bonus\Tests\bonus2_no_query.xml
.\AutoTester.exe ..\..\bonus\Tests\bonus2_cyclic_calls_source.txt ..\..\bonus\Tests\bonus2_no_queries.txt ..\..\bonus\Tests\bonus2_no_query.xml

Expected behaviour: The above tests aborts and the error message is shown in Powershell. For example, "Duplicate procedure names detected." will be shown if the first line of command is being executed.

3. And valid cases consisting multiple procedures without any violations.
.\AutoTester.exe ..\..\bonus\Tests\bonus2_valid_1_source.txt ..\..\bonus\Tests\bonus2_no_queries.txt ..\..\bonus\Tests\bonus2_no_query.xml
.\AutoTester.exe ..\..\bonus\Tests\bonus2_valid_2_source.txt ..\..\bonus\Tests\bonus2_no_queries.txt ..\..\bonus\Tests\bonus2_no_query.xml

Expected behaviour: Tests run successfully without any error.
The above 2 cases takes valid source as input and will successfully parse and validate. You can observe this from the outputs in the terminal.
We do not provide queries for the valid sources in bonus 2. For multiple procedures relations, please refer to bonus 3.

On a side note: Recursive calls and cyclic calls are pretty similar, so they will show the same error message.

----------------------------------------------------------------------
Explanation:


Detect duplicate procedure names

- Using unordered_map<procedure_name, procedure_id> and insert(). Based on 
the return_value.second, if it is false then there already exists a procedure 
with the same procedure name.
- The data in the unordered map will be used for further validation and populating
 relations in the future (if validation passes)


Detect undefined/non-existing procedure

- For every call relation, if find(callee_name) on the unordered_map above
 does not return a pass-the-end iterator, then it is valid and will do 
push_back({caller_name, callee_name}) on the calls vector in step 1 in the next part. Or else the callee does not exist.


Detect cyclic calls and recursive calls

1. Create a vector that contains all procedure calls, i.e. vector<pair<caller_name, callee_name>> calls
2. Create a vector of size same as the number of procedures and initialized with zeros :  vector<int> caller_ctr . 
3. Iterate through 'calls' and do increment for the caller for each call relation (basically counting the number of times a procedure calls another procedure). i.e. caller_ctr[caller_id]++
4. While loop : Look through 'caller_ctr' for values of zero, those that are zeros does not call any procedure and
should be set to -1. If any pair in 'calls' contains callee that does not call any procedure then that pair 
will be removed, the caller of the removed pair will do decrement in call_ctr. 
5. Termination : When all pairs are removed from 'calls', there are no cyclic calls. If any an iteration of the while 
does not spot any counter that is of zero value and there are still pairs in 'calls', there must be cyclic calls.

This is similar to a call graph/tree, but flattened. We built a tree then each time we pull only the leaves off
 until there is nothing left. If there are no leaves and the tree still exists, then there exists cyclic calls.

