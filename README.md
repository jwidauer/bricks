# BRICKS

Small, useful blocks of code, to build bigger things.

Documentation can be found [here](https://jwidauer.github.io/bricks).

## Installation

We use [Meson](https://mesonbuild.com/), [Ninja](https://ninja-build.org/), and [pre-commit](https://pre-commit.com/) for tooling.
To install them, run:

```bash
pip install meson ninja pre-commit
```

## Building

To build the project, run:

```bash
meson setup build
meson compile -C build
```
