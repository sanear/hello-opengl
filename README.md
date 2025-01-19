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
- Clone and [compile GLFW](https://www.glfw.org/docs/latest/compile.html) from [source](https://github.com/glfw/glfw)
- Run `make` _and_ `make install` in the `build/` directory! The readme &co do not specify the `make install` part, but there's no other way the build dll gets into place (`/usr/local/include`) on your system.
- Install `pkg-config` (`brew install pkg-config`)
- `make build` should work now!
- (I almost certainly missed a few arcane steps - going to replace this with CMake at some point, anyway)

### Build and run
- `make init all` should do it for the first time. After that, `make` is enough.
- `bin/hello`, baby! It makes a spinning triangle.
- (The odds of this working on anything other than my exact machine are slim to none)

### Interact
This project is brimming with features
- A triangle spins! Is it slightly off-center? I didn't write that part.
- Most importantly, SPACE pauses the spinning and resumes the spinning _from where it started_, using a _very stupid_ time-offset "algorithm"
- ESC closes the window (also cmd-q or cmd-w, but that's true out of the box)

### TODOS

#### OpenGL
- [x] Move (translate) the spinning triangle with WASD
- [x] Learn why there's those weird blocks of string-wrapped code for vertex & fragment shaders (also what those really are)
- [ ] Convert the triangle to a triforce
    - [ ] Stretch - make it a configurable Sierpinsky triangle
- [ ] Abstract some of the OpenGL boilerplate into a library
- [ ] Implement a button? Is that even an OpenGL thing? Or, I guess, learn what people make dummy user interfaces in
- [ ] Render a bitmap... if OpenGL is for that ([It appears so](https://users.polytech.unice.fr/~buffa/cours/synthese_image/DOCS/redbookSliced1.2ps+pdf/chapterVIII.pdf)).
- [ ] ~Find a way to get the `glad.c` file out of my source?~
- [ ] Learn linear algebra 😭

#### Build & infra
- [ ] Convert Frankenstein custom Makefile nightmare to CMake project. [This way](https://cliutils.gitlab.io/modern-cmake/chapters/basics/structure.html)?

#### Dev env
- [ ] Execute `make && bin/hello` via emacs command (ideally C-c C-c) (Probably easy but boring)
- [ ] Breakpoints & debugging (would be my first time in emacs, and, probably C++ programmers have that down?)

#### Pure C++ stuff
- [ ] Custom logging framework. [This one](https://www.geeksforgeeks.org/logging-system-in-cpp/) looks easy to work through.
- [ ] Probably make a Trello board to share with Ava...
