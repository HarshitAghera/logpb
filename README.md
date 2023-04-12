# logpb
Desktop application to log and visualize data received from data acquisition systems in form of stream of Google Protocol Buffer Messages. 
The currently supported stream format is size-id-data. 
The project aims to provide logging and visualization solution for data acquisition systems with very high data rate.


## Supported Platforms
- Windows 64-bit
- GNU/Linux 64-bit


## License 
logpb can be distributed under LGPL v3.0 license.


## Dependencies
### Qt
The project uses Qt 6.3.2 framework. However, non-GUI part of the project does not depend on it.

### Google Protocol Buffers
The Protobuf 3.21.12 is built from source as part of the project.
The application is tightly coupled with the Protobuf.
For examples, everywhere in the project, for the file IOs Protobuf's FileInputStream, and FileOutputStream are used.

### Qwt
The project uses Qwt to generate plots.
The Qwt 6.2.0 is also built from source as part of the project.
It currently doesn't support CMake, and hence this [patch](https://github.com/HarshitAghera/qwt) of Qwt 6.2.0 is used instead.


## How to build
### Dependencies 
#### GNU/Linux
- Qt 6.3.2 for GCC-9 64-bit

#### Windows
- Qt 6.3.2 for MSVC-2019 64-bit

The Project uses GCC-9.x on GNU/Linux, and MSVC-2019 on Windows.
MinGW support for Windows is currently not considered.


The project uses CMake build system and Conan package manager.
Create a build directory, run Conan, generate build files using CMake, and build the project.
  
    mkdir build-dir
    cd build-dir
    conan install ..
    cmake -S .. . -DCMAKE_BUILD_TYPE=Debug -DPROJECT_BUILD_TESTS=True -G Ninja
    ninja
