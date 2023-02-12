# wasm-invaders

## Local development

Prequisites:
- emscripten
- node

### Installing http-server

`npm install -g http-server`

## Compiling

`emcc -c src/main.c -o obj/main.o -s USE_SDL=2`
`emcc obj/main.o -o index.html -s USE_SDL=2`

## Serving generated page

`http-server ./`

## Run

Open localhost:8080/index.html in the browser