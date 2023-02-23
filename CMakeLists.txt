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
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s USE_SDL=2 -s ASYNCIFY")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --preload-file ${ROMS_DIR}")
    add_executable(wasm-invaders ${APP_SOURCES})
elseif(TESTS)
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
    
    )
    set(CXX_STANDARD 17)
    
    include(FetchContent)
    FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/b796f7d44681514f58a683a3a71ff17c94edb0c1.zip
    )
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)

    add_library(wasm-invaders-lib ${APP_LIB_SOURCES})
    add_executable(runTests ${TEST_SOURCES})
    target_link_libraries(runTests googletest gtest_main wasm-invaders-lib)
endif()