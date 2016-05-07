
# i6engine #

Welcome to **i6engine**!

i6engine is a collection of lightweight libraries to solve common tasks. We designed it to be used when you don't want to write the code by yourself and don't need some odd features.

## Features ##
 
**General**

 * Full source code access
 * Frequent minor updates and roadmap for next minor and major releases

**Platforms and API**

 * Runs on Windows and Linux
 * Builds with various C++11 compilers such as MSVC 13, gcc 4.9+ and clang 3
 * Easy to use and well documented API hiding the multithreaded subsystems
 * Tools also working on Linux and Windows

**Subsystems**

 * Audio (OpenAL Soft)
 * Graphic (Ogre3D)
 * GUI (CEGUI)
 * Input (OIS)
 * Physic (bullet)
 * Network (m2etis)
 * Scripting (lua or python)

**Objects**

 * Component based object system
 * Add a new Component to your GameObject to add functionality instead of inheritance

**Network**

 * Powerful network library using publish subscribe mechanics
 * Different configurations possible depending on requirements (speed, consistency, scalability)

**Scripting**

 * Complete API support for lua and python

**Samples**

 * Many samples showing different features implemented using i6engine
 * Example games for different genres showing how to create with the engine and can be used as base
 * Currently available sample games: RPG

## Platforms ##

i6engine has been tested on:

 * Linux
 * Windows

## Requirements ##

i6engine has nearly no requirements. We use CMake (2.8+) for the build system and a C++11 capable compiler such as g++ 4.9 or Visual Studio 13 (or higher).

## Documentation ##

The doxygen documentation can be found on our homepage: http://clockwork-origins.de/en/i6engine/docu

## Building from source ##

**LINUX**

 * First of all, build all the dependencies. This will take some time (~ 15 minutes)
 * There are several build-scripts, but build-dependencies.sh will run them all
* You can run the scripts individually, e.g. if you want to rebuild 1 library, but be careful
 * some libraries depends on others (read comments in build-dependencies)

cd dependencies
./build-dependencies.sh

 * Go back to the main directory, run cmake with the appropriate options (see infos below) and finally make

cd ..
cmake .
make

 * now, you'll find the executables in the bin subdirectory and can run it
 * That's all

**WINDOWS**

 * First of all, build all the dependencies. This will take some time
 * There are several build-scripts, but build-dependencies.bat will run them all
 * You can run the scripts individually, e.g. if you want to rebuild 1 library, but be careful
 * some libraries depends on others (read comments in build-dependencies)
 * build-dependencies.bat and the other build scripts require three tools:
	
 - WinRAR.exe (specify path in build-common.bat)
 - cmake (has to be in path)
 - Visual Studio
		
 * If all these programs are installed, you start the Visual Studio command line
 * (e.g. Start => All programs => Visual Studio => Visual Studio tools => VS2013 x86 Native Tools)
 * and move to i6engine directory. Then just follow the next steps:

cd dependencies
build-dependencies.bat

 * Go back to the main directory, run cmake with the appropriate options (see infos below) and you get your Visual Studio project

cd ..
cmake .

 * now you find a i6engine.sln file you can open in Visual Studio. You can compile it using F7. After this, all library files
 * and the executables are in the bin subdirectory and you can run it
 * That's all

## Contributing Code ##

We welcome patches. We will set up a Contributor Agreement soon to take care of the legal aspect of contributions.
