# Advent Of Code 2022

## How to Build

### Nix development

`aoc2022` supports nix as a development tool. Simply install the [nix package manager](https://nixos.org/download.html) and use the standard development flow for nix flakes.

```sh
nix develop
mkdir build
cd build
cmake ..
make -j8
```

### Alternatives
`aoc2022` uses [CMake](http://www.cmake.org) to support cross-platform building. Install [CMake](http://www.cmake.org) _(Resources -> Download)_ before proceeding. The basic steps to build are:

**Note:** If you don't use the provided installer for your platform, make sure that you add `CMake`'s bin folder to your path.

#### 1. Navigate into the source directory, create build folder and run `CMake`:

```sh
mkdir build
cd build
cmake [-G generator] ..
```

  * The `generator` option is the build system you'd like to use. Run `cmake` without arguments to see a full list of available generators.
    * On Windows, you might use "Visual Studio 12 2013" (VS 2013 32-bits), or "Visual Studio 14 2015 Win64" (VS 2015 64-bits).
    * On OS X, you might use "Xcode".
    * On a UNIX-like system, omit the option and a Makefile will be generated.

#### 2. Build it!
  * The command you'll need to run depends on the generator you chose earlier.


