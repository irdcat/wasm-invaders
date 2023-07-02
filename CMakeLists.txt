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
        src/main.cpp
    )
    set(CMAKE_EXECUTABLE_SUFFIX ".html")
    set(CXX_STANDARD 17)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s USE_SDL=2 -s ASYNCIFY -s ALLOW_MEMORY_GROWTH=1 -g")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --preload-file ${ROMS_DIR}")
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
        tests/CpuImplTest.cpp
        tests/RegisterPairTest.cpp
        tests/ShiftRegisterImplTest.cpp
    )
    set(CPU_DIAG_TEST_SOURCES
        tests/cpudiagtest/CpuDiagTest.cpp
    )
    set(CXX_STANDARD 17)

    include(FetchContent)
    FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/b796f7d44681514f58a683a3a71ff17c94edb0c1.zip
    )
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)

    add_library(wasm-invaders ${APP_LIB_SOURCES})
    add_executable(unit-tests ${TEST_SOURCES})
    target_link_libraries(unit-tests gtest_main gmock_main wasm-invaders)

    add_executable(cpu-diag-tests ${CPU_DIAG_TEST_SOURCES})
    target_link_libraries(cpu-diag-tests wasm-invaders)
endif()