## Hello, OpenGL in C++
Proof that I can render things at all

### Project structure

- `src/`
    - Source files. `main.cpp` is the entry point.
- `lib/`
    - Library files. I'm putting models in there for now, which probably doesn't make sense
- `bin/`
    - Target directory for `hello` executable
    - Empty dir, so can't include it in the repo. Created with `make init`
### OpenGL setup
Lazy option: brew
- `brew install pkg-config glfw`
- `make build`

This doesn't work, at all... tries to "install" glfw to `/usr/local/lib` (whether or not that whole directory exists) and then simply does nothing useful with the `pkg-config` component.
- Clone and [compile GLFW](https://www.glfw.org/docs/latest/compile.html) from [source](https://github.com/glfw/glfw)
- Run `make` _and_ `make install` in the `build/` directory! The readme &co do not specify the `make install` part, but there's no other way the build dll gets into place (`/usr/local/include) on your system.
- Install `pkg-config` (`brew install pkg-config`)
- `make build` should work now!

### Build and run
- `make init all` should do it for the first time. After that, `make` is enough.
- `bin/hello`, baby! It makes a spinning triangle.
- (The odds of this working on anything other than my exact machine are slim to none)

### TODOS
- [ ] Convert Frankenstein custom Makefile nightmare to CMake project
- [ ] Find a way to get the `glad.c` file out of my source?
- [ ] Learn why there's those weird blocks of string-wrapped code for vertex & fragment shaders (also what those really are)
    - [ ] Consider removing the shading entirely for simplicity
- [ ] Move (translate) the spinning triangle with WASD
- [ ] Abstract some of the OpenGL boilerplate into a library
- [ ] Implement a button? Is that even an OpenGL thing? Or, I guess, learn what people make dummy user interfaces in
- [ ] Render a bitmap... if OpenGL is for that. If not, pivot to another "hello world" framework
- [ ] Learn linear algebra 😭
- [ ] Probably make a Trello board to share with Ava...
