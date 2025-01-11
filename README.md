## Hello, C(++) 
Prove that  I can compile some C++ hopefully

### Project structure

- `src/`
    Source files. `main.cpp` is the entry point.
- `lib/`
    Library files. I'm putting models in there for now, which probably doesn't make sense
- `include/`
    External library files, like OpenGL stuff. This is just a shot in the dark tbh.

### OpenGL setup
Lazy option: brew
- `brew install pkg-config glfw`
- `make build`

This doesn't work, at all... tries to "install" glfw to `/usr/local/lib` (whether or not that whole directory exists) and then simply does nothing useful with the `pkg-config` component.
- Clone and [compile GLFW](https://www.glfw.org/docs/latest/compile.html) from [source](https://github.com/glfw/glfw)
- Install `pkg-config` (`brew install pkg-config`)
- Export the `PKG_CONFIG_PATH` env var to include the above glfw repo
- `make build` should work now!

### Build and stuff
`make init all` should do it for the first time. After that, `make` is enough.
