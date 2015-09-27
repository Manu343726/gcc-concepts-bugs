# gcc-concepts-bugs [![Build Status](https://travis-ci.org/Manu343726/gcc-concepts-bugs.svg?branch=master)](https://travis-ci.org/Manu343726/gcc-concepts-bugs)

A repository to host C++ Concepts Lite TS examples, to help with GCC bug reporting.

## Setup

This repository builds C++ Concepts Lite TS examples on the cloud using Travis CI service, checking if those are compiled succesfully on GCC. The travis setup uses GCC trunk pointing to the [latest `master` commit (2015-09-14)](https://gcc.gnu.org/git/?p=gcc.git;a=commit;h=dd8691e6dce3ff5b64011a5df090a324f6bba45b). That custom GCC is built inside a docker image, and Travis is run under container infrastucture mode. Check the `.travis.yml` file for more details.

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

### Reporting issues

As stated above, you can submit best practices examples, bad-code examples, or May Be A Bug examples. In the case you are submitting a bug example case (or something you think can be a bug) please open an issue here first to talk about it, let other people play with it, etc. Then open the correspondent bug tracking report and please link it in the issue.

Whenever one of the CI builds here fail, it may be treated as an issue in the example itself, not the compiler (Since the example should be tagged correctly).

## License

All C++ code examples are published under the MIT License, except explicitly stated otherwise with a license header at the beginning of the file. The same applies to any script (`run.sh`, `.travis.yml`, etc) within this repository. 
