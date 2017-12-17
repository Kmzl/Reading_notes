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
- **project**
> Set a name, version, and enable languages for the entire project.
> 为项目设置名字、版本和允许使用的语言。
```
project(<PROJECT-NAME> [LANGUAGES] [<language-name>...])
project(<PROJECT-NAME>
        [VERSION <major>[.<minor>[.<patch>[.<tweak>]]]]
        [LANGUAGES <language-name>...])
```
- **add_executable**
> Add an executable to the project using the specified source files.
> 为项目添加一个由指定的源文件编译出来的可执行文件名字。
```
add_executable(<name> [WIN32] [MACOSX_BUNDLE]
              [EXCLUDE_FROM_ALL]
              source1 [source2 ...])
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
- **PROJECT_SOURCE_DIR**
> Top level source directory for the current project.
> 当前项目的根目录
- **PROJECT_BINARY_DIR**
> Full path to build directory for project.
> 当前项目的编译路径的全路径
- **include_directories**
> Add include directories to the build.
> 添加用来编译中进行查找头文件的文件夹路径
```
include_directories([AFTER|BEFORE] [SYSTEM] dir1 [dir2 ...])
```
由于配置生成的头文件将会写到编译文件生成的路径，我们必须把这个路径添加到查找头文件的路径列表。我们将要在源文件目录下创建**TutorialConfig.h.in**文件，该文件内容如下：
```
// the configured options and settings for Tutorial
#define Tutorial_VERSION_MAJOR @Tutorial_VERSION_MAJOR@
#define Tutorial_VERSION_MINOR @Tutorial_VERSION_MINOR@
```
当**CMake**进行配置头文件的时候，变量`@Tutorial_VERSION_MAJOR@`和`@Tutorial_VERSION_MINOR@`将会被**CMakeLists.txt**中的值替换。接下来我们将修改**tutorial.cxx**文件来包含配置生成的头文件和使用版本号。修改后的头文件如下:
```
// A simple program that computes the square root of a number
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "TutorialConfig.h"

int main (int argc, char *argv[])
{
    if (argc < 2)
    {
        fprintf(stdout,"%s Version %d.%d\n",
            argv[0],
            Tutorial_VERSION_MAJOR,
            Tutorial_VERSION_MINOR);
        fprintf(stdout,"Usage: %s number\n",argv[0]);
        return 1;
    }
    double inputValue = atof(argv[1]);
    double outputValue = sqrt(inputValue);
    fprintf(stdout,"The square root of %g is %g\n",
          inputValue, outputValue);
    return 0;
}
```
**tutorial.cxx**文件的主要修改是包含了头文件**TutorialConfig.h**和在使用信息中输出了版本号。
