# gcc-concepts-bugs [![Build Status](https://travis-ci.org/Manu343726/gcc-concepts-bugs.svg?branch=master)](https://travis-ci.org/Manu343726/gcc-concepts-bugs)

A repository to host C++ Concepts Lite TS examples, to help with GCC bug reporting.

## Setup

This repository builds C++ Concepts Lite TS examples on the cloud using Travis CI service, checking if those are compiled succesfully on GCC. The travis setup uses GCC trunk pointing to the [Concepts Lite TS support commit](https://gcc.gnu.org/git/?p=gcc.git;a=commit;h=56c12fd4ba064759724236ad896232603b8797ed). That custom GCC is suppossed to be built once, then cached for next CI builds. Check the `.travis.yml` file for more details.

### Updating GCC

The [docker image](https://hub.docker.com/r/manu343726/gcc-concepts/) was built mounting both the GCC sources directory and the GCC buildtree out of the container, to not include build artifacts in the container snapshots. So updating GCC is not as simple as uncommenting the `Dockerfile` commands and build the image again. To update the image, run the image from with `docker run -i -t manu343726/gcc-concepts -v /PATH/TO/HOST/DIRECTORY:/gcc-concepts` (where `/PATH/TO/HOST/DIRECTORY` is a host folder with the gcc sources in a `gcc` subfolder and an empty `gcc-concepts-buildtree` folder for the buildtree), execute the `configure`,`build, and install commands; and then commit the image and push it to Docker Hub.

## Contributing

The idea behind this project is to host as many concepts lite examples as we can to help people to get into it. Since vendor support is rather experimental, this examples are designed to help with bug reporting as well: Whether you know that example should work, it can be checked here via continuous integration, serving as a sandbox to show when reporting compiler bugs.

### Submitting examples

The examples are complete C++ programs (i.e. `.cpp` files with a `main()` function) located in `src/` folder. The `src/` folder has a `concepts/` subfolder designed to host and share concept definitions between examples.

### Tagging examples

The `run.sh` script checks the filename of an example to know if the example was expected to be built successfully or not. There are four different tags:

 - **`STD_CORRECT`**: The example is correct by the Standard and should be compiled successfully in any standard conformant compiler.
 - **`STD_INCORRECT`**: The example is incorrect and should be discarded by any Standard conformant compiler.
 - **`EXPECTED_CONFORMANT`**: The compiler is expected to be Standard conformant when building the example.
 - **`EXPECTED_NON_CONFORMANT`**: The compiler is expected to be non Standard conformant when building the example. The example may use compiler extensions, etc.

As an use case, if we have an example file named `foo_STD_CORRECT_EXPECTED_CONFORMANT.cpp` that cannot be compiled successfully, the `foo` example may reflect a compiler bug that should be reported (See "*Reporting issues*" bellow). Also, an example tagged as Standard incorrect but non-conformant (Because you suspect of a bug or you are using an extension) that cannot be built may reflect the bug was fixed or a regresion was introduced.

### Reporting issues

As stated above, you can submit best practices examples, bad-code examples, or May Be A Bug examples. In the case you are submitting a bug example case (or something you think can be a bug) please open an issue here first to talk about it, let other people play with it, etc. Then open the correspondent bug tracking report and please link it in the issue.

Whenever one of the CI builds here fail, it may be treated as an issue in the example itself, not the compiler (Since the example should be tagged correctly).

## License

All C++ code examples are published under the MIT License, except explicitly stated otherwise with a license header at the beginning of the file. The same applies to any script (`run.sh`, `.travis.yml`, etc) within this repository. 
