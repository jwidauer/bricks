# Changelog

## 0.1.0 (2023-09-13)


### ⚠ BREAKING CHANGES

* introduces new ranges header that contains all iterator adapters
* changes the charconv functions to use bricks::result as return type
* changes concurrent class name to rw_lock

### Features

* add a handle class to use when needing a custom deleter in a pointer ([060ea30](https://github.com/jwidauer/bricks/commit/060ea300f09186f9ec8aa66e54c6088a848a1e77))
* adds filter iterator adapter, for filtering ranges ([4ecce07](https://github.com/jwidauer/bricks/commit/4ecce0727e40c567123ea93002ff3b5792704e96))
* adds new mutex class ([3df782a](https://github.com/jwidauer/bricks/commit/3df782ac3fa57bb1f0b7fcc6e591979865993573))
* adds static functions to construct a result from a function call that might throw ([840fe41](https://github.com/jwidauer/bricks/commit/840fe4153945dcba7a1255dfe6bc189f93c8ed9b))
* initial migration of library ([87855f5](https://github.com/jwidauer/bricks/commit/87855f5c881c7516d25916307570e55fe4e0614a))


### Miscellaneous Chores

* release 0.1.0 ([2d5657c](https://github.com/jwidauer/bricks/commit/2d5657c295d457448cd871e186913dd6532fc654))


### Code Refactoring

* changes concurrent class name to rw_lock ([fe6f682](https://github.com/jwidauer/bricks/commit/fe6f682a893cb2899ae226f84a6d86def7a53e82))
* changes the charconv functions to use bricks::result as return type ([970c57f](https://github.com/jwidauer/bricks/commit/970c57fbfb67f31defaa212e19cee841fc52bded))
* introduces new ranges header that contains all iterator adapters ([dd14d17](https://github.com/jwidauer/bricks/commit/dd14d17cea9d45721113b22031f7214346fcad4b))
