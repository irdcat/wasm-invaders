#!/bin/sh
set -ex

emcmake cmake . -B build -G "Unix Makefiles"

cp -R ./roms ./build
cp ./src/web/index.html ./build
cp ./src/web/index.js ./build
npx tailwindcss -i ./src/web/index.css -o ./build/index.css --minify

cd build

make

cd ..