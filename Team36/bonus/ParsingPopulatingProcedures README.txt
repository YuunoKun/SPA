Bonus 2. Parsing and population of multiple procedures with:
   - detection of 
   - duplicate procedure names
   - recursive calls
   - cyclic calls
   - calls to non-existing procedure
- extraction of UsesP and ModifiesP and storing into PKB

----------------------------------------------------------------------
Visual Studio version used : Microsoft Visual Studio Community 2019
Configuration : Debug

Instructions on running integration test:
1. Open 'Team36\Code36\StartupSPASolution.sln' with Visual Studio
2. In Visual Studio toolbar, navigate to Build -> Rebuild Solution
3. In Solution Explorer, navigate to IntegrationTestingGoogle -> ParserPKBTest_bonus.cpp
4. Right click on it and select 'Run Tests'

----------------------------------------------------------------------
Explanation:

Duplicate procedure names

- Using unordered_map<procedure_name, procedure_id> and insert(). Based on 
the return_value.second, if it is false then there already exists a procedure 
with the same name.
- The data in the unordered map will be used for further validation and populating
 relations in the future (if validation passes)

Detect undefined/non-existing procedure

- For every call relation, if find(callee_name) on the unordered_map above
 does not return a pass-the-end iterator, then it is valid and will do 
push_back({caller_name, callee_name}) on the calls vector in the next part
 step 1. Or else the callee doe snot exist.

Cyclic calls and recursive calls

1. Create a vector that contains each procedure calls, i.e. vector<pair<caller_name, callee_name>> calls
2. Create a vector of size same as the number of procedures and initialized with zeros vector<int> caller_ctr . 
3. Iterate through calls and do increment for the caller for each call relation. i.e. caller_ctr[caller_id]++
4. While loop : Look through caller_ctr for values of zero, those that are zero does not call any procedure 
should be set to -1. If any pair in calls contains callee that does not call any procedure then that pair 
will be removed, the caller of the removed pair will do decrement in call_ctr. 
5. Termination : When all pairs are removed, there are no cyclic calls. If any an iteration of the while 
does not spot any counter that is of zero value and there are still pairs in calls, there must be cyclic calls.

This is similar to a call graph, but flattened. We built a tree then each time we pull only the leaves off
 until there is nothing left. If there are no leaves and the tree still exists, then there exists cyclic calls.