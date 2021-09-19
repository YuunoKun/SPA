Visual Studio version used : Microsoft Visual Studio Community 2019
Configuration : Debug

Instructions on compiling Autotester:
1. Open 'StartupSPASolution.sln' with Visual Studio
2. In Visual Studio toolbar, navigate to Build -> Rebuild Solution
3. Upon successful build, the Autotester executable is found in `Team36\Code36\Debug`

Instructions on running Autotester.exe
1. Open Powershell and navigate to `Team36\Code36\Debug`
2. Run the following command with your choice of sample and query files:
.\AutoTester.exe <SIMPLE-source> <SIMPLE-queries> ..\Tests\out.xml