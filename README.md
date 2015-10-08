# gcc-concepts-bugs [![Build Status](https://travis-ci.org/Manu343726/gcc-concepts-bugs.svg?branch=master)](https://travis-ci.org/Manu343726/gcc-concepts-bugs)

A repository to host C++ Concepts Lite TS examples, to help with GCC bug reporting.

## Setup

This repository builds C++ Concepts Lite TS examples on the cloud using Travis CI service, checking if those are compiled succesfully on GCC. The travis setup uses GCC trunk pointing to the [latest `master` commit (2015-10-08)](https://gcc.gnu.org/git/?p=gcc.git;a=commit;h=8b4af95f5318007e76569679c8ca2b72cfc4687d). That custom GCC is built inside a docker image, and Travis is run under container infrastucture mode. Check the `.travis.yml` file for more details.

### Updating GCC

The [docker image](https://hub.docker.com/r/manu343726/gcc-concepts/) was built mounting both the GCC sources directory and the GCC buildtree out of the container, to not include build artifacts in the container snapshots. So updating GCC is not as simple as uncommenting the `Dockerfile` commands and build the image again. To update the image, run the image from with `docker run -i -t manu343726/gcc-concepts -v /PATH/TO/HOST/DIRECTORY:/gcc-concepts` (where `/PATH/TO/HOST/DIRECTORY` is a host folder with the gcc sources in a `gcc` subfolder and an empty `gcc-concepts-buildtree` folder for the buildtree), execute the `configure`,`build, and install commands; and then commit the image and push it to Docker Hub.

## Contributing

The idea behind this project is to host as many concepts lite examples as we can to help people to get into it. Since vendor support is rather experimental, this examples are designed to help with bug reporting as well: Whether you know that example should work, it can be checked here via continuous integration, serving as a sandbox to show when reporting compiler bugs.

### Submitting examples

The examples are complete C++ programs (i.e. `.cpp` files with a `main()` function) located in `src/` folder. The `src/` folder has a `concepts/` subfolder designed to host and share concept definitions between examples.

### Setting up tests

There's a `CMakeLists.txt` file under the `src/` directory where you can add tests via the `gcb_target()` function, which takes:

 - `TARGET`: Target name.
 - `SOURCES`: List of target sources (Relative to the `src/` directory).
 - `INCLUDE_DIRECTORIES`: List of include directories (Relative to the `src/` directory).
 - `COMPILE_OPTIONS`: List of compile options to build the test with.
 - `EXPECTED_SUCCESS`/`EXPECTED_FAIL`: Whether the given program is expected to be built successfully. Note the purpose of this tests is not to test program behavior (i.e. runtime), but to check if are compiled successfully or not.
 - 'VARIANTS`: List of variants (See *variants* bellow).

`gcb_target()` function instances a CTest test named `${TARGET}` which tries to build the test sources with the settings above and the current cmake C++ compiler. Test passes if compilation result matches `EXPECTED_XXX` as specified in the `gcb_target()` call.

By default the `CMakeLists.txt` file in `src/` scans for source files in that directory using the `add_directory_tests()` function, which GLOBs for `.cpp` files and for each one parses its contents searching for the variables above. So you can simply define a test as a unique `Foo.cpp` file like this:

``` cpp
/* DESCRIPTION: Tests the Foo concept against int type
 * COMPILE_OPTIONS: -std=c++1z -Wall -Werror
 * RESULT: EXPECTED_FAIL
 */

template<typename T>
concept Foo() = requires(T t)
{
	t.foo() -> int
};

int main()
{
	[](Foo foo){}(int{})
}
```

*Note `SOURCES` list is optional there (It's implicitly defined with the source file as only element). Anyway, you can place the name of this source file in the `SOURCES` list if you like.*

#### Variants

A test/example may define different *variants* to explicitly override setup variables in specific code blocks. This is specially useful to show examples of errors or bugs within compliant code:
For example:

``` cpp
/*
 * COMPILE_OPTIONS: -std=c++1z
 * RESULT: SUCCESS
 * VARIANTS: DEFAULT DEFAULT_CONSTRUCT_ERROR:FAIL
 */

struct non_default_constructible
{
    non_default_constructible() = delete;
};

int main()
{
#ifdef DEFAULT_CONSTRUCT_ERROR
    non_default_constructible ndc;
#endif
}
```

The code aboves states that the example is expected to be built successfully (`RESULT == SUCCESS`), but also uses the `VARIANTS` array to declare a `DEFAULT_CONSTRUCT_ERROR` variant in addition to the `DEFAULT` one to check if the compiler catches the deleted constructor error.

Variants can be declared in two different ways:

 - **`VARIANT_NAME`**: A variant that does not override any variable, but only does a `VARIANT_NAME` compile definition. Useful when writting multiple independent examples in the same file.
 - **`VARIANT_NAME:VARIANT_RESULT`**: Same as above, but also overrides `RESULT` variable for this variant.

Also you can override any variable in a variant setting `VARIANT_VARIABLE`:

``` cpp
/*
 * VARIANTS: CPP11 CPP14
 * CPP11_COMPILE_OPTIONS: -std=c++11
 * CPP14_COMPILE_OPTIONS: -std=c++14
 */

int main()
{
	...
}
```

*To avoid name conflicts with other symbols, you can set `VARIANT_DEFINITION_PREFIX` variable:

``` cpp
/*
 * VARIANTS: DEFAULT FOO:FAIL
 * VARIANT_DEFINITION_PREFIX: GCC_TESTS_VARIANT_
 */

// Suppose we have a FOO macro that may conflict with the variant name above
#define FOO std::cout << "foo!" << std::endl

// If we are building FOO variant:
#ifdef GCC_TESTS_VARIANT_FOO
...
#endif
```
### Reporting issues

As stated above, you can submit best practices examples, bad-code examples, or May Be A Bug examples. In the case you are submitting a bug example case (or something you think can be a bug) please open an issue here first to talk about it, let other people play with it, etc. Then open the correspondent bug tracking report and please link it in the issue.

Whenever one of the CI builds here fail, it may be treated as an issue in the example itself, not the compiler (Since the example should be tagged correctly).

## License

All C++ code examples are published under the MIT License, except explicitly stated otherwise with a license header at the beginning of the file. The same applies to any script (`run.sh`, `.travis.yml`, etc) within this repository. 
