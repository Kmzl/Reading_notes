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
## 添加一个库（步骤2）
我们将要添加一个库到我们的项目中。这个库将包含我们自己实现的计算平方根的函数。我们生成的可执行将使用这个库文件替代编译器生成的求标准平方根的函数。在这个教程中，我们将把这个库放在一个**MathFunctions**的子文件夹中，这个文件夹拥有一个只有一行的**CMakeLists.txt**文件：
```
add_library(MathFunctions mysqrt.cxx)
```
- add_library
> Add a library to the project using the specified source files.
```
add_library(<name> [STATIC | SHARED | MODULE]
            [EXCLUDE_FROM_ALL]
            source1 [source2 ...])
```
源文件**mysqrt.cxx**拥有一个名叫**mysqrt**的函数，功能和编译器的**sqrt**函数功能相似。为了编译生成新的库文件，我们将在项目根目录下的**CMakeLists.txt**文件中添加`add_subdirectory`命令加入库项目所在的文件夹，以使在**CMake**中生成的**Makefile**文件中生成库编译步骤。为了得到**MathFunctions/MathFunctions.h**中的函数原型，我们用把**MathFunctions**添加到头文件搜索路径列表中。最后为可执行程序添加新的库。修改后的文件最后几行如下：
```
include_directories ("${PROJECT_SOURCE_DIR}/MathFunctions")
add_subdirectory (MathFunctions)

# add the executable
add_executable (Tutorial tutorial.cxx)
target_link_libraries (Tutorial MathFunctions)
```
- add_subdirectory
> Add a subdirectory to the build.
```
add_subdirectory(source_dir [binary_dir]
                [EXCLUDE_FROM_ALL])
```
- target_link_libraries
> Link a target to given libraries.
```
target_link_libraries(<target> [item1 [item2 [...]]]
                      [[debug|optimized|general] <item>] ...)
```
现在让我们把**MathFunctions**库设为可选项。虽然在这个教程中，这是没有必要的，但是当项目拥有大量库或者库文件是由第三方的代码提供的时候，这个选项可能会用的上。把库设为可选项的第一个步骤是添加一个**option**到根目录的**CMakeLists.txt**文件中。
```
# should we use our own math functions?
option (USE_MYMATH
        "Use tutorial provided math implementation" ON)
```
- option
> Provides an option that the user can optionally select.
```
option(<option_variable> "help string describing option"
      [initial value])
```
这将导致在**CMake GUI**中默认值为**ON**，用户可以根据需要改变它。这个设置将会被保存，于是用户不需要每次在在这个项目中运行**CMake**时进行设置。接下来，把**MathFunctions**库的编译和链接跟改为有条件控制，为了达到这个目的，我们在根目录的**CMakeLists.txt**文件的末尾进行如下修改：
```
# add the MathFunctions library?
#
if (USE_MYMATH)
    include_directories ("${PROJECT_SOURCE_DIR}/MathFunctions")
    add_subdirectory (MathFunctions)
    set (EXTRA_LIBS ${EXTRA_LIBS} MathFunctions)
endif (USE_MYMATH)

# add the executable
add_executable (Tutorial tutorial.cxx)
target_link_libraries (Tutorial  ${EXTRA_LIBS})
```
- if
> Conditionally execute a group of commands.
```
if(expression)
  # then section.
  COMMAND1(ARGS ...)
  COMMAND2(ARGS ...)
  ...
elseif(expression2)
  # elseif section.
  COMMAND1(ARGS ...)
  COMMAND2(ARGS ...)
  ...
else(expression)
  # else section.
  COMMAND1(ARGS ...)
  COMMAND2(ARGS ...)
  ...
endif(expression)
```
- set
> Set a CMake, cache or environment variable to a given value.
```
set(<variable> <value>
    [[CACHE <type> <docstring> [FORCE]] | PARENT_SCOPE])
```
这通过使用设置`USE_MYMATH`变量来控制是否编译和使用`MathFunctions`函数。需要注意的是，我们在这个例子中使用了一个变量`EXTRA_LIBS`来收集可选的库，以在之后链接可执行程序时使用。这是一个使拥有大量可选组件的项目变得清晰的方法。可以相当直接的修改代码，得到如下内容：
```
// A simple program that computes the square root of a number
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "TutorialConfig.h"
#ifdef USE_MYMATH
#include "MathFunctions.h"
#endif

int main (int argc, char *argv[])
{
    if (argc < 2)
    {
        fprintf(stdout,"%s Version %d.%d\n", argv[0],
                Tutorial_VERSION_MAJOR,
                Tutorial_VERSION_MINOR);
        fprintf(stdout,"Usage: %s number\n",argv[0]);
        return 1;
    }

    double inputValue = atof(argv[1]);

#ifdef USE_MYMATH
    double outputValue = mysqrt(inputValue);
#else
    double outputValue = sqrt(inputValue);
#endif

    fprintf(stdout,"The square root of %g is %g\n",
              inputValue, outputValue);
    return 0;
}
```
在源代码中我们可以很好的使用宏`USE_MYMATH`，这通过我们在**TutorialConfig.h.in**文件中增加一行新的内容，然后通过**CMake**配置得到。
```
#cmakedefine USE_MYMATH
```
