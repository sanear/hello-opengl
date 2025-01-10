C++ = llvm-g++
CFLAGS =
BIN_DIR = bin/

init:
	mkdir -p ${BIN_DIR}

build: bin/ hello.cpp
	${C++} ${CFLAGS} hello.cpp -o bin/hello
