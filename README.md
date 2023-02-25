# wasm-invaders

## Local development

Prequisites:
- emscripten
- node
- make
- cmake

### Installing http-server

```
npm install -g http-server
```

## Emulator

### Building

```
.\build.ps1
```

### Serving generated page

```
http-server ./
```

### Running

Open localhost:8080/wasm-invaders.html in the browser

## Building & Running tests

### Building

Building tests
```
.\build-tests.ps1
```

### Running

Running unit-tests
```
.\unit-tests.ps1
```

Running cpu diagnostics tests
```
.\cpu-diag-tests.ps1
```