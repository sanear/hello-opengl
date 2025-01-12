C++ = llvm-g++
CFLAGS =

all: bin/ src/ build

init:
	mkdir -p bin/

# TODO: we get a warning about not finding a search path that doesn't make sense,
# has a space / option in it. pkg-config isn't doing what I want here,
# BUT IT WORKS
build: bin/ src/
	${C++} ${CFLAGS} `pkg-config --cflags glfw3` -o bin/hello src/*.cpp lib/*.cpp `pkg-config --libs glfw3`
