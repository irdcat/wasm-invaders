EMCXX = em++
EMCXXFLAGS = -s USE_SDL=2 -s NO_DISABLE_EXCEPTION_CATCHING -std=c++17
EMOBJFLAGS = -s USE_SDL=2 -s ASYNCIFY -s NO_DISABLE_EXCEPTION_CATCHING --preload-file roms

SOURCES := $(wildcard src/*.cpp)
OBJECTS := $(patsubst src/%.cpp, obj/%.o, $(SOURCES))

all: wasm-invaders

wasm-invaders: $(OBJECTS)
	${EMCXX} ${EMOBJFLAGS} -o $@.html $^

obj/%.o: src/%.cpp
	${EMCXX} -c ${EMCXXFLAGS} -o $@ $<

clean:
	rm -rf obj