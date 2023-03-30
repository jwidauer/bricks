# BRICKS

Small, useful blocks of code, to build bigger things.

Documentation can be found [here](https://jwidauer.github.io/bricks).

## Installation

We use [Conan](https://conan.io/), [Meson](https://mesonbuild.com/), [Ninja](https://ninja-build.org/), [pre-commit](https://pre-commit.com/), and [commitlint](https://commitlint.js.org/#/) for tooling.
To install them, run:

```bash
pip install conan meson ninja pre-commit
npm install --save-dev @commitlint/{config-conventional,cli}
conan profile detect
```

## Building

Then, to install the dependencies and compile the project, run:

```bash
conan build .
```
