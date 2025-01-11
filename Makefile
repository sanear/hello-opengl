C++ = llvm-g++
CFLAGS =

all: bin/ src/ build

init:
	mkdir -p bin/

build: bin/ src/
	${C++} ${CFLAGS} src/*.cpp lib/*.cpp -o bin/hello
