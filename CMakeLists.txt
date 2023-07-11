cmake_minimum_required(VERSION 3.25)
project(wasm-invaders)

if(EMSCRIPTEN)
    set(ROMS_DIR "./roms/" CACHE STRING "Roms directory")
    set(APP_SOURCES
        src/ApuImpl.cpp
        src/BusImpl.cpp
        src/CpuImpl.cpp
        src/InputsImpl.cpp
        src/MemoryImpl.cpp
        src/ShiftRegisterImpl.cpp
        src/Registers.cpp
        src/Emulator.cpp
        src/api.cpp
    )
    set(CMAKE_EXECUTABLE_SUFFIX ".js")
    set(CXX_STANDARD 17)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -sWASM=1 -sUSE_SDL=2 -sASYNCIFY -sALLOW_MEMORY_GROWTH=1 -O3")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -sEXPORTED_FUNCTIONS=_run -sEXPORTED_RUNTIME_METHODS=ccall")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --preload-file ${ROMS_DIR} --use-preload-plugins")
    add_executable(wasm-invaders ${APP_SOURCES})
endif()

if(NOT EMSCRIPTEN)
    set(APP_LIB_SOURCES
        src/ApuImpl.cpp
        src/BusImpl.cpp
        src/CpuImpl.cpp
        src/InputsImpl.cpp
        src/MemoryImpl.cpp
        src/ShiftRegisterImpl.cpp
        src/Registers.cpp
    )
    set(TEST_SOURCES
        tests/CpuDiagTest.cpp
    )
    set(CXX_STANDARD 17)

    add_library(wasm-invaders ${APP_LIB_SOURCES})
    add_executable(cpu-diag-tests ${TEST_SOURCES})
    target_link_libraries(cpu-diag-tests wasm-invaders)
endif()