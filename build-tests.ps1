cmake . -B build/tests -G "Unix Makefiles"

cmake --build build/tests

$copyExclude = Get-ChildItem -Recurse .\build\tests
Copy-Item .\tests\TST8080.COM -Destination .\build\tests -Exclude $copyExclude