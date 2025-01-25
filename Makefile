C++ = llvm-g++

all: bin/ src/ build rename

init:
	mkdir -p bin/

# TODO: we get a warning about not finding a search path that doesn't make sense,
# has a space / option in it. pkg-config isn't doing what I want here,
# BUT IT WORKS
build: bin/ src/
	${C++} ${CFLAGS} `pkg-config --cflags glfw3` -o bin/hello src/*.cpp lib/*.c `pkg-config --libs glfw3`

# TODO: this seems like it should be unnecessary, but maybe OSX defaults to /usr/lib?
# I think I could relatively easily reconfigure glfw to install there, but this seems
# easier for now...
# Eventually we're gonna use CMake anyway
rename: build
	install_name_tool -add_rpath /usr/local/lib bin/hello
