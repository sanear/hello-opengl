C++ = llvm-g++
CFLAGS =

all: bin/ src/ build

init:
	mkdir -p bin/

# TODO: get a warning bout not finding a search path that doesn't make sense,
# has a space / option in it. pkg-config isn't doing what I want here,
# BUT IT WORKS
build: bin/ src/
	${C++} $(pkg-config --cflags glfw3) -o bin/hello src/*.cpp lib/*.cpp $(pkg-config --static --libs glfw3)
