emcmake cmake . -B build -G "Unix Makefiles"

Copy-Item -Path .\roms -Destination .\build -Recurse -Force
Copy-Item .\src\web\index.html .\build -Force
Copy-Item .\src\web\index.js .\build -Force

cd build

make

cd ..