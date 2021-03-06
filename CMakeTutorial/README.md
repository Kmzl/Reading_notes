[cmake-tutorial 官方地址](https://cmake.org/cmake-tutorial/)
# CMake Tutorial
## 一个简单的开始(步骤1)
大多数简单项目是有源文件所编译。对于例子，**CMakeLists.txt** 文件仅仅需要两行，这将开始我们的教程。**CMakeList.txt** 文件如下：

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
需要注意的是，这个样例文件 **CMakeLists.txt** 使用小写形式的命令，但是 **CMake** 的命令可以是大写，小写和大小写混合使用的。文件 **tutorial.cxx** 的第一个版本十分简单，将要计算一个数的平方根，代码如下：
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
我们将要添加的第一个特性是使我们的项目和可执行程序拥有一个版本号。尽管你可以通过修改代码来达到这个目标，但是让它在 **CMakeLists.txt** 文件中完成可以更加灵活。为了增加版本号，我们将要修改 **CMakeLists.txt** 文件，修改后的文件如下：
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
由于配置生成的头文件将会写到编译文件生成的路径，我们必须把这个路径添加到查找头文件的路径列表。我们将要在源文件目录下创建 **TutorialConfig.h.in** 文件，该文件内容如下：
```
// the configured options and settings for Tutorial
#define Tutorial_VERSION_MAJOR @Tutorial_VERSION_MAJOR@
#define Tutorial_VERSION_MINOR @Tutorial_VERSION_MINOR@
```
当 **CMake** 进行配置头文件的时候，变量`@Tutorial_VERSION_MAJOR@`和`@Tutorial_VERSION_MINOR@`将会被 **CMakeLists.txt** 中的值替换。接下来我们将修改 **tutorial.cxx** 文件来包含配置生成的头文件和使用版本号。修改后的头文件如下:
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
**tutorial.cxx** 文件的主要修改是包含了头文件 **TutorialConfig.h** 和在使用信息中输出了版本号。
## 添加一个库（步骤2）
我们将要添加一个库到我们的项目中。这个库将包含我们自己实现的计算平方根的函数。我们生成的可执行将使用这个库文件替代编译器生成的求标准平方根的函数。在这个教程中，我们将把这个库放在一个 **MathFunctions** 的子文件夹中，这个文件夹拥有一个只有一行的 **CMakeLists.txt** 文件：
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
源文件 **mysqrt.cxx** 拥有一个名叫 **mysqrt** 的函数，功能和编译器的 **sqrt** 函数功能相似。为了编译生成新的库文件，我们将在项目根目录下的 **CMakeLists.txt** 文件中添加`add_subdirectory`命令加入库项目所在的文件夹，以使在 **CMake** 中生成的 **Makefile** 文件中生成库编译步骤。为了得到 **MathFunctions/MathFunctions.h** 中的函数原型，我们用把 **MathFunctions** 添加到头文件搜索路径列表中。最后为可执行程序添加新的库。修改后的文件最后几行如下：
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
现在让我们把 **MathFunctions** 库设为可选项。虽然在这个教程中，这是没有必要的，但是当项目拥有大量库或者库文件是由第三方的代码提供的时候，这个选项可能会用的上。把库设为可选项的第一个步骤是添加一个 **option** 到根目录的 **CMakeLists.txt** 文件中。
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
这将导致在 **CMake GUI** 中默认值为 **ON**，用户可以根据需要改变它。这个设置将会被保存，于是用户不需要每次在在这个项目中运行 **CMake** 时进行设置。接下来，把 **MathFunctions** 库的编译和链接跟改为有条件控制，为了达到这个目的，我们在根目录的 **CMakeLists.txt** 文件的末尾进行如下修改：
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
这通过使用设置 `USE_MYMATH` 变量来控制是否编译和使用 `MathFunctions` 函数。需要注意的是，我们在这个例子中使用了一个变量 `EXTRA_LIBS` 来收集可选的库，以在之后链接可执行程序时使用。这是一个使拥有大量可选组件的项目变得清晰的方法。可以相当直接的修改代码，得到如下内容：
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
在源代码中我们可以很好的使用宏 `USE_MYMATH`，这通过我们在 **TutorialConfig.h.in** 文件中增加一行新的内容，然后通过 **CMake** 配置得到。
```
#cmakedefine USE_MYMATH
```

## 安装和测试（步骤3）
接下来，我们将在项目中添加安装规则和测试支持。安装规则非常简单，对于 **MathFunctions** 库，我们将在 **MathFunctions** 目录下的 **CMakeLists.txt** 文件中添加两行代码进行库文件和头文件的安装。
```
install (TARGETS MathFunctions DESTINATION bin)
install (FILES MathFunctions.h DESTINATION include)
```
接下来，我们将在根目录的 **CMakeLists.txt** 文件中增加代码以使可执行程序和配置生成的头文件完成安装。
```
# add the install targets
install (TARGETS Tutorial DESTINATION bin)
install (FILES "${PROJECT_BINARY_DIR}/TutorialConfig.h" DESTINATION include)
```
这就是全部，至此，你可以键入 `make install` 或者从你的 **IDE** 进行构建，这将安装相应的头文件、库和可执行程序。**CMake** 的变量 **CMAKE_INSTALL_PREFIX** 用来决定安装路径的根目录。添加测试也是一个相当直接的过程。在根目录的 **CMakeLists.txt** 文件的末尾我们可以添加一些基本测试来验证程序是否正常工作。
```
include(CTest)

# does the application run
add_test (TutorialRuns Tutorial 25)
# does it sqrt of 25
add_test (TutorialComp25 Tutorial 25)
set_tests_properties (TutorialComp25 PROPERTIES PASS_REGULAR_EXPRESSION "25 is 5")
# does it handle negative numbers
add_test (TutorialNegative Tutorial -25)
set_tests_properties (TutorialNegative PROPERTIES PASS_REGULAR_EXPRESSION "-25 is 0")
# does it handle small numbers
add_test (TutorialSmall Tutorial 0.0001)
set_tests_properties (TutorialSmall PROPERTIES PASS_REGULAR_EXPRESSION "0.0001 is 0.01")
# does the usage message work?
add_test (TutorialUsage Tutorial)
set_tests_properties (TutorialUsage PROPERTIES PASS_REGULAR_EXPRESSION "Usage:.*number")
```
构建程序后，我们可以执行 **ctest** 命令来执行测试。第一个测试用来简单判断程序运行过程中是否存在 **segfault** 或者其他 **crash** 情况，程序是否正常返回0。这是 **CTest** 的基本样子。接下来几个测试全使用 **PASS_REGULAR_EXPRESSION** 来判断测试中的程序输出内容中是否包含执行的字符串。这种情况下，需要验证计算平方根的结构是否正确，和当提供无效的数字时是否输出 **usage** 信息。如果你想要测试不同的输入参数，你需要像下方的代码中创建宏。
```
#define a macro to simplify adding tests, then use it
macro (do_test arg result)
  add_test (TutorialComp${arg} Tutorial ${arg})
  set_tests_properties (TutorialComp${arg} PROPERTIES PASS_REGULAR_EXPRESSION ${result})
endmacro (do_test)

# do a bunch of result based tests
do_test (25 "25 is 5")
do_test (-25 "-25 is 0")
```
对于每次调用`do_test`，项目通过传入的名字、输入、和输出参数添加一个测试。

## 思考平台差异性（步骤4）
接下来我们在我们项目中添加一些依赖平台特性的代码。在这个列子中，我们将要目标平台是否拥有 **log** 和 **exp** 函数来决定添加代码。当然，每个平台都拥有这些函数，但是在这个教程中假定这些函数不通用。如果平台拥有 **log** 函数，我们将使用 **mysqrt** 函数来计算平方根。我们首先在根目录的 **CMakeLists.txt** 文件钟使用 **CheckFunctionExists.cmake** 宏来判断函数是否可用：
```
# does this system provide the log and exp functions?
include (CheckFunctionExists)
check_function_exists (log HAVE_LOG)
check_function_exists (exp HAVE_EXP)
```
接下来我们修改 **TutorialConfig.h.in** 文件，当 **CMake** 发现那些函数时，则定义相关的宏：
```
// does the platform provide exp and log functions?
#cmakedefine HAVE_LOG
#cmakedefine HAVE_EXP
```
重要的是，我们必须在使用 **configure_file** 命令生成 **TutorialConfig.h** 文件前判断 **log** 和 **exp** 函数是否可以使用。**configure_file** 命令直接使用 **CMake** 的设置来生成配置后的文件。最后，**mysqrt** 函数在系统支持的情况下，可以使用另一种由使用 **log** 和 **exp** 函数的方式实现。
```
// if we have both log and exp then use them
#if defined (HAVE_LOG) && defined (HAVE_EXP)
  result = exp(log(x)*0.5);
#else // otherwise use an iterative approach
  . . .
```
## 添加生成器和生成的文件（步骤5）
在这个段落，我们将要展示如何添加一个生成的源文件到一个应用的构建过程中。在这个例子中，我们将创建一张预先计算了平方根的表到构建过程中，然后在编译我们的应用的过程中使用这张表。为了达到这个目的，我们搜先需要一个程序来生成这张表。在 **MathFunctions** 文件夹，我们创建一个新的文件 **MakeTable.cxx** 来达到这个目的。
```
// A simple program that builds a sqrt table
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main (int argc, char *argv[])
{
    int i;
    double result;

    // make sure we have enough arguments
    if (argc < 2)
    {
        return 1;
    }

    // open the output file
    FILE *fout = fopen(argv[1], "w");
    if (!fout)
    {
        return 1;
    }

    // create a source file with a table of square roots
    fprintf(fout,"double sqrtTable[] = {\n");
    for (i = 0; i < 10; ++i)
    {
        result = sqrt(static_cast<double>(i));
        fprintf(fout,"%g,\n",result);
    }

    // close the table with a zero
    fprintf(fout,"0};\n");
    fclose(fout);
    return 0;
}
```
注意，这个表通过有效的代码生成，写到文件中的名字将作为参数传递。下一个步骤，添加相应的命令到 **MathFuncitions** 的 **CMakeLists.txt** 文件中，以生成 **MakeTable** 程序，并在构建过程中运行程序当作构建的一部分。接下来的一些代码就是用来达到这个目的：
```
# first we add the executable that generates the table
add_executable (MakeTable MakeTable.cxx)
# add the command to generate the source code
add_custom_command (
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/Table.h
    DEPENDS MakeTable
    COMMAND MakeTable ${CMAKE_CURRENT_BINARY_DIR}/Table.h
    )
# add the binary tree directory to the search path
# for include files
include_directories( ${CMAKE_CURRENT_BINARY_DIR} )

# add the main library
add_library(MathFunctions mysqrt.cxx ${CMAKE_CURRENT_BINARY_DIR}/Table.h)
```
第一个 **exectuable** 指令，就像其他可执行程序的 **executable** 指令一样被添加到文件中。接下来我们添加一个自定义指令，用来通过 **MakeTable** 来产生 **Table.h** 文件。为了使 **CMake** 知道 **mysqrt.cxx** 依赖 **Table.h** 文件，我们需要把 **Table.h** 文件添加到库 **MathFunctions** 的源文件列表中，同时添加当前程序文件夹到头文件查找列表中，这样 **Table.h** 才能在 **mysqrt.cxx** 中发现和使用。当项目进行构建时，它会首先构建 **MakeTable** 程序，然后在运行 **MakeTable** 过程中将生产 **Table.h**。最后，构建的过程将使用 **Table.h** 编译 **mysqrt.cxx** 生成库 **MathFunctions**。添加了所有特性后，根目录的 **CMakeLists.txt** 文件如下。
```
cmake_minimum_required (VERSION 2.6)
project (Tutorial)
include (CTest)

# The version number.
set (Tutorial_VERSION_MAJOR 1)
set (Tutorial_VERSION_MINOR 0)

# does this system provide the log and functons?
include (${CMAKE_ROOT}/Modules/CheckFunctionExists.cmake)

check_function_exists (log HAVE_LOG)
check_function_exists (exp HAVE_EXP)

# should we use our own math functions
option (USE_MYMATH
    "Use tutotial  provided math implementation" ON)

# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
    "${PROJECT_SOURCE_DIR}/TutorialConfig.h.in"
    "${PROJECT_BINARY_DIR}/TutorialConfig.h"
    )

# add the binary tree to the search path for include files
# so that we will find TutorialConfig.h
include_directories ("${PROJECT_BINARY_DIR}")

# add the MathFunctions library?
if (USE_MYMATH)
    include_directories ("${PROJECT_SOURCE_DIR}/MathFunctions")
    add_subdirectory (MathFunctions)
    set (EXTRA_LIBS ${EXTRA_LIBS} MathFunctions)
endif (USE_MYMATH)


# add the executable
add_executable (Tutorial tutorial.cxx)
target_link_libraries (Tutorial ${EXTRA_LIBS})

# add the install targets
install (TARGETS Tutorial DESTINATION bin)
install (FILES "${PROJECT_BINARY_DIR}/TutorialConfig.h"
            DESTINATION include)

# does the application run
add_test (TutorialRuns Tutorial 25)

# does the usage mesage work?
add_test (TutorialUsage Tutorial)
set_tests_properties (TutorialUsage
    PROPERTIES
    PASS_REGULAR_EXPRESSION "Usage:.*number"
    )

#define a macro to simplify adding tests
macro (do_test arg result)
    add_test (TutorialComp${arg} Tutorial ${arg})
    set_tests_properties (TutorialComp${arg}
        PROPERTIES PASS_REGULAR_EXPERSSION ${result}
        )
endmacro (do_test)

do_test (4 "4 is 2")
do_test (9 "9 is 3")
do_test (5 "5 is 2.236")
do_test (7  "7 is 2.645")
do_test (25 "25 is 5")
do_test (-25 "-25 is 0")
do_test (0.0001 "0.0001 is 0.01")
```
**TutorialConfig.h.in** 如下
```
// the configured options and settings for Tutorial
#define Tutorial_VERSION_MAJOR @Tutorial_VERSION_MAJOR@
#define Tutorial_VERSION_MINOR @Tutorial_VERSION_MINOR@
#cmakedefine USE_MYMATH

// does the platform provide exp and log functions?
#cmakedefine HAVE_LOG
#cmakedefine HAVE_EXP
```
**MathFuncitons** 里的 **CMakeLists.txt** 如下
```
# first we add the executable that generates the table
add_executable (MakeTable MakeTable.cxx)
# add the command to generate the source code
add_custom_command (
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/Table.h
    DEPENDS MakeTable
    COMMAND MakeTable ${CMAKE_CURRENT_BINARY_DIR}/Table.h
    )
# add the binary tree directory to the search path
# for include files
include_directories( ${CMAKE_CURRENT_BINARY_DIR} )

# add the main library
add_library(MathFunctions mysqrt.cxx ${CMAKE_CURRENT_BINARY_DIR}/Table.h)

install (TARGETS MathFunctions DESTINATION bin)
install (FILES MathFunctions.h DESTINATION include)
```

## 构建一个安装程序（步骤6）
接下来我们打算发布我们的程序给别人使用。我们希望完成程序和源代码在不同平台的分发。这和我们在步骤3使用 **install** 指令进行的安装和测试有一些不同，步骤3我们必须编译我们需要安装的程序。在这个例子中，我们将要根据 **cygwin**, **debian**, **RPMs** 等不同的包管理特性来构建我们的安装程序来提供可执行程序。我们将在这个正解中描述如何使用 **CPack** 来创建分别对于个平台的安装程序。我了达到这目的，我们将在根目录的 **CMakeLists.txt** 文件的底下增加几行。
```
# build a CPack driven installer package
include (InstallRequiredSystemLibraries)
set (CPACK_RESOURCE_FILE_LICENSE
        "${CMAKE_CURRENT_SOURCE_DIR}/License.txt")
set (CPACK_PACKAGE_VERSION_MAJOR "${Tutorial_VERSION_MAJOR}")
set (CPACK_PACKAGE_VERSION_MINOR "${Tutorial_VERSION_MINOR}")
include (CPack)
```
这就是所有需要的代码。我们首先包含 **InstallRequiresSystemLibraries**。这个模块将会包含所有我们的项目在各个平台下需要的动态库。接下来我们设置一些 **CPack** 变量用来加载我们项目的证书和版本号。最后，我们包含 **CPack** 模块，这个模块在运行安装程序时使用变量和其他需要的系统资源。接下来的步骤，我们呢以通常的方式构建项目，并执行 **CPack**。为了构建一个二进制发布版本，你需要执行：
```
cpack --config CPackConfig.cmake
```
为了发布一个源代码版本，你需要执行：
```
cpack --config CPackSourceConfig.cmake
```

## 添加仪表盘支持（步骤7）
支持提交我们的测试结果到仪表盘非常容易。我们已经在我们的项目中定义了一系列测试样例在之前的步骤。我们只需要执行这些测试然后提交这些到仪表盘。我们需要在根目录下的 **CMakeLists.txt** 里包含 **CTest** 模块来增加仪表盘支持。
```
# enable dashboard scripting
include (CTest)
```
我们还通过创建一个 **CTestConfig.cmake** 文件来在仪表盘中指定项目名字。
```
set (CTEST_PROJECT_NAME "Tutorial")
```
**CTest** 将在执行的时候加载这个文件。你可以通过切换目录到可执行程序目录，运行 `ctest –D Experimental` 来创建一个简单的仪表盘。仪表盘的结果将被上传到 **Kitware**的公开仪表盘，[网址](https://open.cdash.org/index.php?project=PublicDashboard)。
