# wasm-invaders

## Description

Space invaders emulator compiled to WebAssembly.

![Emulator Screen](images/emulator.png)

## Controls

- **C** - Insert Coin
- **1** - Select 1 Player
- **2** - Select 2 Players

- **W** - Player 1 Shoot
- **A** - Player 1 Left
- **D** - Player 1 Right

- **↑** - Player 2 Shoot
- **←** - Player 2 Left
- **→** - Player 2 Right

## Local Development

### Prerequisites 

- [emscripten](https://emscripten.org/docs/getting_started/downloads.html)
- [node](https://nodejs.org/en/download)
- make
- [cmake](https://cmake.org/)

### Running locally

In order to run the application locally, application must be hosted on a server, because browsers are forbidding loading wasm files from the disk.

#### Install http-server from npm

In this document, tool used to host files on a local server is a **http-server** from NPM.

```
npm install -g http-server
```

#### Build the emulator

Scripts included in the project handle things like building Cpp sources into binaries and handling NPM dependencies (like TailwindCSS used in the project).
After building all of the required files are put in the `./build` directory.

```
.\build.sh
```

#### Serve built files on a local server

Command below serves directory with built files at port 8080.

```
http-server ./build -p8080 -c-1
```

#### Open application in web browser

Open *localhost:8080* in web browser
