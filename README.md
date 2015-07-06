Open Conservation Law Solver Framework
======================================

Documentation
-------------
[Doxygen documentation](http://jenskre.at.ifi.uio.no/ocls/)

Build Project
-------------
### Note About C++11
At the moment the framework requires C++11 and the compiler argument 'std=c++11' is enabled by default. Fallback to support 
older compilers are planned. The C++11 featues currently required are:

*   [Thread support library] (http://en.cppreference.com/w/cpp/thread)
*   [Variadic Templates / Parameter Packs] (http://en.cppreference.com/w/cpp/language/parameter_pack)
*   [Functional Objects] (http://en.cppreference.com/w/cpp/utility/functional)

Please check if your compiler supports these features: [Compiler status](http://en.cppreference.com/w/cpp/compiler_support).

Tested:

*   GCC 4.8.2 (Linux)
*   Clang++ 3.3 (OSx Xcode 5.0)
*   Clang++ 3.2 (Linux)

Untested but should work:

*   GCC 4.3.*
*   Clang 2.9
*   Microsoft Visual Studio 2013 (MSVC12)

### Framework Dependencies:
1. OpenCL 
2. [GLEW (OpenGL Extension Wrangler Library)](http://glew.sourceforge.net/)
3. [GLFW3 (OpenGL Window Manager Library)](http://www.glfw.org/)
4. [GLM (OpenGL Mathematics)](http://glm.g-truc.net/0.9.5/index.html)
#### Unit-Testing Suit:
5. [Boost Test](http://www.boost.org/doc/libs/1_44_0/libs/test/doc/html/index.html)
    
#### Note:
We plan to make it possible to build the framework without visualization support which would remove the 
OpenGL related dependencies from the dependency list.

### Linux (Ubuntu)
#### Installing libraries:
GLEW, GLM and Boost Test can be installed from apt-get, simply copy this command into your terminal:

```
sudo apt-get install build-essential xorg-dev libboost-filesystem-dev libboost-system-dev libboost-test-dev libglew-dev libglewmx-dev libglm-dev
```

At the moment for writing, only GLFW2 is available through Ubuntu package distribution and we need
GLFW3. We must then instead download the latest version from GitHub. These commands will download the
latest version of GLFW, build it and install it.

```
git clone git@github.com:glfw/glfw.git &&
mkdir glfw/build &&
cd glfw/build &&
cmake .. &&
make install 
```

Installing OpenCL libraries depends on what OpenCL capable device you want to run the framework on.

- Intel CPUs, Integrated GPUs and Xeon Phi:
    * https://software.intel.com/en-us/articles/opencl-drivers
- AMD GPUs
    * OpenCL are distributed through the latest Catalyst Drivers http://support.amd.com/en-us/download
- AMD CPUs and APUs
    * http://support.amd.com/en-us/download
- Nvidia
    * OpenCL are distributed though the CUDA drivers https://developer.nvidia.com/cuda-downloads
    
### Building with CMake
1. Clone the project to a desired location
2. Create a build folder [Optional]
3. ```cmake [path to project folder]```
4. ```make all```
5. ```./bin/tests/``` to run the unit tests or ```./bin/demo``` to run the demo program
