#!/bin/sh

emcmake cmake . -B build -G "Unix Makefiles"

cp -R ./roms ./build
cp ./src/web/index.html ./build
cp ./src/web/index.js ./build

cd build

make

cd ..