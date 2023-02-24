emcmake cmake . -B build -G "Unix Makefiles"

$copyExclude = Get-ChildItem -Recurse .\build
Copy-Item -Path .\roms -Destination .\build -Recurse -Exclude $copyExclude

cd build

make

cd ..