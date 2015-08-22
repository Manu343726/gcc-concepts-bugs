# gcc-concepts-bugs [![Build Status](https://travis-ci.org/Manu343726/gcc-concepts-bugs.svg?branch=master)](https://travis-ci.org/Manu343726/gcc-concepts-bugs)

A repository to host C++ Concepts Lite TS examples, to help with GCC bug reporting.

## Setup

This repository builds C++ Concepts Lite TS examples on the cloud using Travis CI service, checking if those are compiled succesfully on GCC. The travis setup uses GCC trunk pointing to the [Concepts Lite TS support commit](https://gcc.gnu.org/git/?p=gcc.git;a=commit;h=56c12fd4ba064759724236ad896232603b8797ed). That custom GCC is suppossed to be built once, then cached for next CI builds. Check the `.travis.yml` file for more details.

## Tests

The `run.sh` scripts builds each C++ source file located at `tests/` directory. Those files should have `EXPECTED_SUCCESS` or `EXPECTED_FAILURE` strings as part of its filename, to indicate whether successful compilation is expected or not.

## License

All C++ code examples are published under the MIT License, except explicitly stated otherwise with a license header at the beginning of the file. The same applies to any script (`run.sh`, `.travis.yml`, etc) within this repository. 
