cmake . -B build/tests -G "Unix Makefiles"

cmake --build build/tests

cp ./tests/TST8080.COM ./build/tests/TST8080.COM