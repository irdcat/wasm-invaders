# wasm-invaders

## Local development

Prequisites:
- emscripten
- node
- make
- cmake

### Installing http-server

`npm install -g http-server`

## Compiling

`emcmake cmake . -B . -G "Unix Makefiles"`
`make`

## Serving generated page

`http-server ./`

## Run

Open localhost:8080/wasm-invaders.html in the browser