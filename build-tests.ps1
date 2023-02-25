cmake . -B build/tests -G "Unix Makefiles"

cmake --build build/tests

$copyExclude = Get-ChildItem -Recurse .\build\tests
Copy-Item .\tests\cpudiagtest\8080EX1.COM -Destination .\build\tests -Exclude $copyExclude
Copy-Item .\tests\cpudiagtest\8080PRE.COM -Destination .\build\tests -Exclude $copyExclude
Copy-Item .\tests\cpudiagtest\CPUTEST.COM -Destination .\build\tests -Exclude $copyExclude
Copy-Item .\tests\cpudiagtest\TEST.COM -Destination .\build\tests -Exclude $copyExclude