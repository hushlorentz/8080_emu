An emulator for the 8080 processor

The Space Invaders front-end uses SDL. You can download the library from here https://www.libsdl.org/

The Space Invaders binary is not included in this repo, but is easily found on the internet. It's usually distributed in four parts: invaders.e, invaders.f, invaders.g, invaders.h. The loader expects the files to be concatenated into an invaders.bin file starting with invaders.h and working back to invaders.e.

This project has a badly written Makefile that builds two projects.

'make' will produce the 'emu' binary.
'make build_tests' will produce the 'run_tests' binary.
'make run_tests' will produce the 'run_tests' binary and run it.

There are a number of errors in the dependencies in the Makefile, such that editing cpu.cpp, and running make may lead to seg faults. Sorry, I'm terrible at make! Doing 'make clean && make' will always produce a correct binary.

The test framework I used is Catch. It is included in the project and documentation can be found here https://github.com/catchorg/Catch2

I followed this guide http://www.emulator101.com/ and the 8080 programming manual.
