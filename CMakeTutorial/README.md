[TOC]
[cmake-tutorial](https://cmake.org/cmake-tutorial/)
# CMake Tutorial
## 一个简单的开始(步骤1)
大多数简单项目是有源文件所编译。对于例子，CMakeLists.txt 文件仅仅需要两行，这将开始我们的教程。CMakeList.txt 文件如下：
```
cmake_minimum_required (VERSION 2.6)
project (Tutorial)
add_executable(Tutorial tutorial.cxx)
```
需要注意的是，这个样例文件 CMakeLists.txt 使用小写形式的命令，但是 CMake 的命令可以是大写，小写和大小写混合使用的。文件 tutorial.cxx 的第一个版本十分简单，将要计算一个数的平方根，代码如下：
```
// A simple program that computes the square root of a number
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main (int argc, char *argv[])
{
    if (argc < 2)
    {
        fprintf(stdout, "Usage: %s number\n", argv[0]);
        return 1;
    }
    double inputValue = atof(argv[1]);
    double outputValue = sqrt(inputValue);
    fprintf(stdout, "The square root of %g is %g\n",
                inputValue, outputValue);
    return 0;
}
```
### 添加版本号和配置头文件
我们将要添加的第一个特性是使我们的项目和可执行程序拥有一个版本号。尽管你可以通过修改代码来达到这个目标，但是让它在 CMakeLists.txt 文件中完成可以更加灵活。为了增加版本号，我们将要修改 CMakeLists.txt 文件，修改后的文件如下：
```
cmake_minimum_required (VERSION 2.6)
project (Tutorial)
# The version number.
set (Tutorial_VERSION_MAJOR 1)
set (Tutorial_VERSION_MINOR 0)

# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
  "${PROJECT_SOURCE_DIR}/TutorialConfig.h.in"
  "${PROJECT_BINARY_DIR}/TutorialConfig.h"
  )

# add the binary tree to the search path for include files
# so that we will find TutorialConfig.h
include_directories("${PROJECT_BINARY_DIR}")

# add the executable
add_executable(Tutorial tutorial.cxx)
```
