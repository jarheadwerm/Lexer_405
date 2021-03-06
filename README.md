# Ethereal

[![Build Status](https://api.cirrus-ci.com/github/Electrux/Ethereal.svg?branch=master)](https://cirrus-ci.com/github/Electrux/Ethereal)

A simple, dynamically typed, interpreted, generic programming language.

# Some Sample Programs

## Hello world using Function:
```go
fn hello( to ) {
	println( 'Hello ', to );
}

hello( 'world' );
```

## Taking input from the user - with a prompt:
```go
dat = scan( 'Please enter some data: ' );
println( 'The data you entered is: ', dat );
```

## Iterative Factorial of a number:
```python
import std.str; # for to_int()

num = scan( "Enter factorial of: " ).to_int();
fact = 1;

for x = num; x >= 2; x -= 1 {
        fact *= x;
}

println( "Factorial of ", num, ": ", fact );
```

# About

Visit the medium article [here](https://medium.com/p/so-i-created-a-programming-language-4d9c11038d22?source=email-852839018f8a--writer.postDistributed&sk=d09aaa9916783522215c1024f3ef86f2), for an overview of my journey with Ethereal :)

The language syntax is inspired from Python and C. It contains sufficient features to enjoy working with it, but avoids complex features like OOP.

Ethereal does use a concept of member functions which are actually nothing but plain functions bound to a particular struct/type. And they allow the use of `self` to use the calling variable inside the function itself.

One can easily make extensions for the language in the form of language modules or c++ dynamic libraries (if high performance is needed). See the existing modules for more information about it (language modules: `include/ethereal/`, C++ standard modules: `modules/std/`).

Documentation is under development and will take some time to build, but till then, feel free to go through the [Code Samples](https://github.com/Electrux/Ethereal/blob/master/Samples.md) and hack around with the programs in the `tests` directory.

# Prerequisites

*  GCC/Clang with full C++ 11 support, tested with:
    *  Ubuntu 14.04 (GCC 4.8.4, GMP 5.1.3, MPFR 3.1.2, MPC 1.0.1)
    *  Arch Linux (GCC 9.1.0, GMP 6.1.2, MPFR 3.1.2, MPC 1.0.1)
    *  macOS 10.13.6 (Homebrew LLVM 9.0.0, Homebrew GMP 6.1.2, MPFR 4.0.2, MPC 1.1.0)
    *  macOS 10.14.6 (Homebrew LLVM 8.0.1, Apple LLVM 10.0.1, Homebrew GMP 6.1.2, MPFR 4.0.2, MPC 1.1.0)
    *  FreeBSD 10.4 (clang 3.4.1, GMP 6.1.2, MPFR 3.1.2, MPC 1.0.1)
    *  Android 9 - Pie (Termux - clang 8.0.0, GMP 6.1.2 manually compiled - see the note below, MPFR 3.1.2, MPC 1.0.1)
*  GMP library with CXX support (will be almost always built with support for CXX in your official distribution package)

Note that GMP on Termux for android does not come with CXX support, hence it will have to be manually compiled from source with the `--enable-cxx` configure option. You may also need to specify the `PREFIX_DIR` directory using `--prefix` configure option which, for me, is `/data/data/com.termux/files/usr`

The entire command sequence for installing GMP on android (for me) is: `./configure --prefix /data/data/com.termux/files/usr --enable-cxx && make -j8 && make install`

Also, compiling with Link Time Optimization won't work on clang < 3.9, so disable LTO by setting `SKIP_LTO=true` before executing the `bootstrap.sh` script

# Installation

Once the prerequisites are met, just execute the cmake script by doing `mkdir build && cd build && cmake .. && make -j<cpu core count>` commands. They will build the language interpreter along with the stdlib modules. Installation (using `make install`) should be done **ONLY** if the `PREFIX_DIR` variable is set to a directory other than the cmake `build/` directory.

Note that if you use `PREFIX_DIR`, you may need root access depending on the directory you choose.

The following items will be installed:

*  `buildfiles/et` -> `$PREFIX_DIR/bin/`
*  `buildfiles/lib*.so` -> `$PREFIX_DIR/lib/ethereal/`
*  `include/ethereal/*` -> `$PREFIX_DIR/include/ethereal/`

Also, the interpreter code internally uses `PREFIX_DIR` to locate the `lib` and `include` directories, so you will have to rebuild the codebase if you change `PREFIX_DIR`.

Contributions are definitely accepted and greatly appreciated. ❤️
