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

## Building & Running tests

### Building

Building tests
```
.\build-tests.ps1
```

### Running

Running tests
```
.\run-tests.ps1
```