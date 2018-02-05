# video-game-design

This project uses CMake and a compiler that is at least compatible with C++14.

SDL2 is an external dependency and must be listed in your path for CMake to
locate it. Please see [SDL2 with CMake](https://trenki2.github.io/blog/2017/06/02/using-sdl2-with-cmake/)
for instructions on how to do this on your machine.

------

To build the project, navigate to the root and create a directory for an out of source build:

```
mkdir build
cd build
cmake ..
make
```

You only need to do this step for the initial build. Further builds can be done just with `make`
in the build folder.

The produced executable can be called by entering `./VGD` on a UNIX-like system.

## Project Structure

The project is set up in the following manner:

```
./
 |-src/    # All project source code
 |-inc/    # All project header files
 |-lib/
   |-inc/  # All external library header files
   |-src/  # All external library source files (only if being compiled from source)
```
