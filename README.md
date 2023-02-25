## Instructions

### Dependencies

-   clang-format
-   Latest C++ compiler(Clang/GCC)
-   CMake
-   Ninja
-   CCache (optional but recommended)

### Setting-Up Instructions

-   `pip install -r requirements.txt` to download conan, cmake-format and compdb
-   For setting up the build folder, run `bash setup_build.sh`, this will fetch
    the dependencies and create a build folder.

### Build Instructions

-   `cd build`
-   `cmake -G Ninja ..` . We can provide extra build flags while running this.
    For example 
    - to generate release build, run
    `cmake -G Ninja -DCMAKE_BUILD_TYPE=Release ..`
    - to generate release build without tests and statically, run
    ` cmake -G Ninja -DCMAKE_BUILD_TYPE=Release -DBUILD_PROJECT=no_tests ..`
-   `ninja`

### Building inside Docker

-   `make gcc-docker`
-   Other steps are same as above
