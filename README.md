# Makefile Template v0.6

A reusable Makefile template for C projects.

This template separates project configuration from build logic. Most projects only require editing `config.mk`, while the build rules remain in the `Makefile` and the modules under `makefiles/`.

The template supports multiple build targets, multiple compiler toolchains, automatic dependency generation, recursive source discovery, and automatic test discovery.

## Project Layout

```
.
├── bin/
│   └── tests/
├── debug/
├── include/
├── lib/
├── makefiles/
│   ├── architecture.mk
│   ├── target-generator.mk
│   ├── test-generator.mk
│   ├── gcc.mk
│   ├── clang.mk
│   ├── clang-msvc.mk
│   └── msvc.mk
├── obj/
├── src/
├── tests/
├── config.mk
└── Makefile
```

Generated directories:

* `bin/` — Executable targets.
* `bin/tests/` — Test executables.
* `lib/` — Static libraries.
* `obj/` — Object files and dependency files.
* `debug/` — Debug equivalents of the previous directories.

---

# Build Targets

Targets are declared in `config.mk`.

Each target requires:

* `TARGETS` — Space-separated list of target names.
* `[target]_DIR` — Directory containing the source files.
* `[target]_TYPE` — `executable`, `static` or `shared`.

Optional target settings:

* `[target]_LIBS`
* `[target]_LIBS_PATH`

Source files are discovered recursively.

---

# Tests

Every `.c` file inside the `tests/` directory is treated as an independent executable.

Tests are discovered automatically. No registration is required.

Global test configuration is available through:

* `TESTS_LIBS`
* `TESTS_LIBS_PATH`

---

# Global Configuration

The following variables are available in `config.mk`.

| Variable                    | Description                                             |
| --------------------------- | ------------------------------------------------------- |
| `CVERSION`                  | C language version passed to the selected toolchain.    |
| `EXTRA_CFLAGS`              | Additional compiler flags.                              |
| `EXTRA_LDFLAGS`             | Additional linker flags.                                |
| `EXTRA_INCLUDES`            | Additional include directories.                         |
| `PRE_BUILD`                 | Command executed before each target is built.           |
| `POST_BUILD`                | Command executed after each target is built.            |
| `ENABLE_LTO`                | Enables Link Time Optimization when supported.          |
| `ENABLE_SANITIZERS`         | Enables sanitizers supported by the selected toolchain. |
| `ENABLE_WARNINGS_AS_ERRORS` | Treats compiler warnings as errors.                     |

---

# Available Commands

```
make
```

Builds every target listed in `TARGETS`.

```
make clean
```

Removes all generated files.

```
make debug
```

Builds every target in debug mode.

```
make tests
```

Builds every test executable.

```
make tests-debug
```

Builds every test in debug mode.

---

# Toolchains

The toolchain is selected when invoking Make.

```
make TOOLCHAIN=gcc
make TOOLCHAIN=clang
make TOOLCHAIN=clang-msvc
make TOOLCHAIN=msvc
```

---

### gcc

Uses the GNU toolchain (`gcc`, `gcc-ar`).

Produces `.o` objects and `.a` static libraries.

---

### clang

Uses Clang with the GNU driver (`clang`, `llvm-ar`).

Compatible with GCC-style command-line options and intended for Linux or Windows through MinGW/MSYS2.

---

### clang-msvc

Uses the official LLVM compiler targeting the Microsoft ABI.

Produces MSVC-compatible `.lib` libraries while preserving the Clang/GNU command-line interface.

---

### msvc

Uses Microsoft's native compiler and linker (`cl`, `link`, `lib`).

Produces `.obj` object files and `.lib` static libraries.

---

# Hints

---

If your tests use a library built by the same project, configure:

```
TESTS_LIBS := <target>
TESTS_LIBS_PATH := lib
```

where `<target>` is the name of the static library target.

---

Additional include directories should be specified through `EXTRA_INCLUDES` instead of adding `-I` or `/I` manually.

---

Additional compiler and linker options should be placed in `EXTRA_CFLAGS` and `EXTRA_LDFLAGS` instead of modifying the toolchain files.

---

Library names should be specified without prefixes or extensions.

For example:

```
m
pthread
SDL2
MyLibrary
```

instead of

```
libm.a
libSDL2.a
SDL2.lib
```

The appropriate naming convention is applied automatically by each toolchain.

---

Target library paths should be specified without `-L` or `/LIBPATH:`.

---

Changing the toolchain does not require modifying the project configuration. Only the command used to invoke `make` changes.

---

When possible, prefer configuring projects through `config.mk` rather than editing the Makefile itself.

---

# Troubleshooting

---

### Clang fails on Windows

`TOOLCHAIN=clang` expects a MinGW/MSYS2 installation.

If you are using the official LLVM distribution together with Visual Studio Build Tools, use

```
make TOOLCHAIN=clang-msvc
```

instead.

---

### Sanitizers do not work

Sanitizer support depends on the compiler.

* GCC (MinGW): depends on the distribution; many builds provide limited or no support.
* Clang: supports AddressSanitizer and UndefinedBehaviorSanitizer.
* MSVC: supports AddressSanitizer only.

---

### MSVC cannot find standard libraries

Open a Developer Command Prompt or Developer PowerShell before running `make`.

These environments configure the required `PATH`, `INCLUDE` and `LIB` variables.

---

### Link errors when using LTO

LTO requires the corresponding archive tool.

* GCC should use `gcc-ar`.
* Clang (GNU) should use `llvm-ar`.
* MSVC should use `lib`.
* Clang targeting the MSVC ABI should use `llvm-lib`.

---

### Clang reports architecture conflicts

Ensure that the Visual Studio environment matches the compiler target.

For example, use the x64 Developer Command Prompt when compiling x64 binaries.

---

### Undefined references when linking tests

Verify that the required libraries are listed in `TESTS_LIBS` and that `TESTS_LIBS_PATH` points to the directory containing them.

---

### Dependency files are not generated

Dependency generation is available only on GNU-style toolchains (`gcc` and `clang`). MSVC does not generate `.d` files.

---

### Build artifacts appear outdated

Run

```
make clean
```

before rebuilding if the build directory has been modified manually or the selected toolchain has changed.