# BRICKS

Small, useful blocks of code, to build bigger things.

Documentation can be found [here](https://jwidauer.github.io/bricks).

## Installation

We use [Conan](https://conan.io/), [Meson](https://mesonbuild.com/), [Ninja](https://ninja-build.org/), [pre-commit](https://pre-commit.com/), and [commitlint](https://commitlint.js.org/#/) for tooling.
To install them, run:

```bash
pip install conan==1.59.0 meson ninja pre-commit
npm install --save-dev @commitlint/{config-conventional,cli}
conan profile new default --detect
conan profile update settings.compiler.libcxx=libstdc++11 default
```

## Building

Then, to install the dependencies and compile the project, run:

```bash
conan install . -if build --build=missing
conan build . -bf build
```
