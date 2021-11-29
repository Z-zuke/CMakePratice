cmake 入门攻略

> 如果你没有实际的项目需求，那么可以停下来了，因为 cmake 的学习过程就是实践过程，没有实践，读的再多几天后也会忘记。



## 构建基本例程

文件树

>├── CMakeLists.txt
>└── main.cpp

main.cpp

```cpp
#include <stdio.h>

int main() {
    printf("Hello Cmake!\n");

    return 0;
}
```

CMakeLists.txt

```cmake
cmake_minimum_required(VERSION 3.10)

project(hello_cmake)

SET(SRC_LIST ../main.cpp)

message(STATUS "This is BINARY dir " ${PROJECT_BINARY_DIR})	# 编译目录
message(STATUS "This is SOURCE dir " ${PROJECT_SOURCE_DIR})	# 工程顶层目录

add_executable(${PROJECT_NAME} ${SRC_LIST})
```

#### 解析

**命令作用解析**

CMake构建包含一个项目名称，上面的命令会自动生成一些变量，在使用多个项目时引用某些变量会更加容易。

PROJECT 指令的语法是： 

```cmake
PROJECT(projectname [CXX] [C] [Java])
```

这个指令隐式的定义了两个 cmake 变量: \<projectname>\_BINARY_DIR 以及\<projectname>\__SOURCE_DIR，这里就是 hello_cmake_BINARY_DIR 和 hello_cmake_SOURCE_DIR (所以 CMakeLists.txt 中两个 MESSAGE 指令可以直接使用了这两个变量)。

cmake 系统也预定义了 **PROJECT_BINARY_DIR** 和 **PROJECT_SOURCE_DIR** 变量，他们的值分别跟  hello_cmake_BINARY_DIR 和 hello_cmake_SOURCE_DIR  一致。

> 建议以后直接使用 PROJECT_BINARY_DIR，PROJECT_SOURCE_DIR，即使修改了工程名称，也不会影响这两个变量。如果使用了 \<projectname>\__SOURCE_DIR，修改工程名称后，需要同时修改这些变量。

CMake语法指定了许多变量，其中一些包括：

| Variable                 | Info                                                       |
| :----------------------- | :--------------------------------------------------------- |
| CMAKE_SOURCE_DIR         | 根源代码目录，工程顶层目录。暂认为就是PROJECT_SOURCE_DIR   |
| CMAKE_CURRENT_SOURCE_DIR | 当前处理的 CMakeLists.txt 所在的路径                       |
| PROJECT_SOURCE_DIR       | 工程顶层目录                                               |
| CMAKE_BINARY_DIR         | 运行cmake的目录。外部构建时就是build目录                   |
| CMAKE_CURRENT_BINARY_DIR | The build directory you are currently in.当前所在build目录 |
| PROJECT_BINARY_DIR       | 暂认为就是CMAKE_BINARY_DIR                                 |



SET 指令的语法是： 

```cmake
SET(VAR [VALUE] [CACHE TYPE DOCSTRING [FORCE]]) 
```

 比如我们用到的是 SET(SRC_LIST main.c)，如果有多个源文件，也可以定义成： SET(SRC_LIST main.c t1.c t2.c)。



MESSAGE 指令的语法是： 

```cmake
MESSAGE([SEND_ERROR | STATUS | FATAL_ERROR] "message to display" ...)
```

这个指令用于向终端输出用户定义的信息，包含了三种类型: 

- SEND_ERROR，产生错误，生成过程被跳过。 
- SATUS，输出前缀为—的信息。 
- FATAL_ERROR，立即终止所有 cmake 过程.



ADD_EXECUTABLE 指令：

```cmake
ADD_EXECUTABLE(${PROJECT_NAME} ${SRC_LIST}) # 生成可执行文件
```

add_executable()命令指定某些源文件生成可执行文件，本例中是main.cpp。 add_executable（）函数的第一个参数是可执行文件名，第二个参数是要编译的源文件列表。

\${PROJECT_NAME}表示PROJECT_NAME变量的值为hello_cmake，所以把\${PROJECT_NAME}用在add_executable()里可以生成可执行文件名字叫hello_cmake。



**基本语法规则**

1. 变量使用`${}`方式取值，但是在 IF 控制语句中是直接使用变量名 

2. `指令(参数 1 参数 2...)` 参数使用括弧括起，参数之间使用空格或分号分开。 
3. 指令是大小写无关的，参数和变量是大小写相关的。但，推荐你全部使用大写指令。
4. 假设一个源文件的文件名是 `fu nc.c`(文件名中间包含了空格)，必须使用双引号包含 `“fu nc.c”`。



#### 外部构建

使用外部构建，我们可以创建一个可以位于文件系统上任何位置的构建文件夹。 所有临时构建和目标文件都位于此目录中，以保持源代码树的整洁。

> 内部构建将所有临时文件和源文件生成到一起，没有build，临时文件会和源代码文件在一起。（不推荐）

运行下述代码，新建build构建文件夹，并运行cmake命令

```shell
mkdir build && cd build
cmake ..
make
```

可以看到，build文件夹下生成了许多二进制文件，如果要从头开始重新创建cmake环境，只需删除构建目录build，然后重新运行cmake。

make会调用生成的Makefile，构建出可运行程序。运行make命令时，输出仅显示构建状态。 要查看用于调试目的的完整输出，可以在运行 `make VERBOSE=1` 。`make clean` 即可对构建结果进行清理。



## 进阶

文件树

> ├── CMakeLists.txt
> ├── COPYRIGHT
> ├── doc							  // 工程文档
> │   └── hello.txt
> ├── README
> ├── runhello.sh				// 用来调用 hello 二进制 
> └── src								// 存储源代码
>     ├── CMakeLists.txt
>     └── main.cpp

目标如下：

将 hello 二进制与 runhello.sh 安装至/\<prefix>/bin，将 doc 目录中的 hello.txt 以及 COPYRIGHT/README 安装到 /\<prefix>/share/doc/cmake/t2



runhello.sh

```shell
#!/bin/bash

./hello_cmake_2
```

src/main.cpp文件内容同上节。

src/CMakeLists.txt

```cmake
add_executable(hello_cmake_2 main.cpp)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

INSTALL(TARGETS hello_cmake_2
        RUNTIME DESTINATION bin)
```

CMakeLists.txt

```cmake
cmake_minimum_required(VERSION 3.10)

PROJECT(hello_cmake_2)
add_subdirectory(src bin) 

INSTALL(FILES COPYRIGHT README DESTINATION share/doc/cmake/t2)

INSTALL(PROGRAMS runhello.sh DESTINATION bin)

INSTALL(DIRECTORY doc/ DESTINATION share/doc/cmake/t2)
```



#### 解析

指令 INSTALL 和变量 CMAKE_INSTALL_PREFIX

CMAKE_INSTALL_PREFIX 变量类似于 configure 脚本的 –prefix，常见的使用方法看起来是这个样子：

```shell
cmake -DCMAKE_INSTALL_PREFIX=../../tmp ..
```



INSTALL 指令用于定义安装规则，安装的内容可以包括目标二进制、动态库、静态库以及 文件、目录、脚本等。

1. 目标文件的安装

```cmake
INSTALL(TARGETS targets...
 [[ARCHIVE|LIBRARY|RUNTIME]
     [DESTINATION <dir>]
     [PERMISSIONS permissions...]
     [CONFIGURATIONS [Debug|Release|...]]
     [COMPONENT <component>]
     [OPTIONAL]
 ] [...])
```

参数中的 **TARGETS** 后面跟的就是我们通过 ADD_EXECUTABLE 或者 ADD_LIBRARY 定义的目标文件，可能是可执行二进制、动态库、静态库。

**目标类型**也就相对应的有三种，ARCHIVE 特指静态库，LIBRARY 特指动态库，RUNTIME 特指可执行目标二进制。

**DESTINATION** 定义了安装的路径，如果路径以/开头，那么指的是绝对路径，这时候 CMAKE_INSTALL_PREFIX 其实就无效了。如果你希望使用 CMAKE_INSTALL_PREFIX 来定义安装路径，就要写成相对路径，即不要以`/`开头，那么安装后的路径就是 ${CMAKE_INSTALL_PREFIX}/\<DESTINATION 定义的路径>。



2. 普通文件的安装

```cmake
INSTALL(FILES files... DESTINATION <dir>
     [PERMISSIONS permissions...]
     [CONFIGURATIONS [Debug|Release|...]]
     [COMPONENT <component>]
     [RENAME <name>] [OPTIONAL])
```

可用于安装一般文件，并可以指定访问权限，文件名是此指令所在路径下的相对路径。如果默认不定义权限 PERMISSIONS，安装后的权限为： OWNER_WRITE, OWNER_READ, GROUP_READ,和 WORLD_READ，即 644 权限。



3. 非目标文件的可执行程序安装(比如脚本之类)

```cmake
INSTALL(PROGRAMS files... DESTINATION <dir>
     [PERMISSIONS permissions...]
     [CONFIGURATIONS [Debug|Release|...]]
     [COMPONENT <component>]
     [RENAME <name>] [OPTIONAL])
```

跟上面的 FILES 指令使用方法一样，唯一的不同是安装后权限为: OWNER_EXECUTE, GROUP_EXECUTE, 和 WORLD_EXECUTE，即 755 权限。



4. 目录的安装

```cmake
INSTALL(DIRECTORY dirs... DESTINATION <dir>
     [FILE_PERMISSIONS permissions...]
     [DIRECTORY_PERMISSIONS permissions...]
     [USE_SOURCE_PERMISSIONS]
     [CONFIGURATIONS [Debug|Release|...]]
     [COMPONENT <component>]
     [[PATTERN <pattern> | REGEX <regex>]
     [EXCLUDE] [PERMISSIONS permissions...]
 ] [...])
```

DIRECTORY 后面连接的是所在 Source 目录的相对路径。

注意： abc 和 abc/有很大的区别。 如果目录名不以/结尾，那么这个目录将被安装为目标路径下的 abc，如果目录名以/结尾， 代表将这个目录中的内容安装到目标路径，但不包括这个目录本身。

PATTERN 用于使用正则表达式进行过滤，PERMISSIONS 用于指定 PATTERN 过滤后的文件权限。



5. 安装 CMAKE 脚本

```cmake
INSTALL([[SCRIPT <file>] [CODE <code>]] [...])
```

SCRIPT 参数用于在安装时调用 cmake 脚本文件（也就是.cmake 文件）



6. 安装 CODE

```cmake
INSTALL(CODE "MESSAGE(\"Sample install message.\")")
```

CODE 参数用于执行 CMAKE 指令，必须以双引号括起来。



#### 构建

运行下述代码，新建build构建文件夹，并运行cmake命令。

CMAKE_INSTALL_PREFIX 的默认定义是 /usr/local

```shell
mkdir build && cd build
cmake -DCMAKE_INSTALL_PREFIX=../../tmp ..
make
make install
```

进入/tmp 目录看一下安装结果：

> ├── bin
> │   ├── hello_cmake_2
> │   └── runhello.sh
> └── share
>     └── doc
>         └── cmake
>             └── t2
>                 ├── COPYRIGHT
>                 ├── hello.txt
>                 └── README



## 进阶二

文件树

> ├── CMakeLists.txt
> └── lib
>     ├── CMakeLists.txt
>     ├── hello.cpp
>     └── hello.h

本节目标：建立一个静态库和动态库，安装头文件和共享库。



hello.h

```cpp
#ifndef HELLO_H
#define HELLO_H

#include <stdio.h>

void HelloFunc();

#endif  // HELLO_H
```

hello.cpp

```cpp
#include "hello.h"

void HelloFunc() {
    printf("Hello Cmake Lib.\n");
}
```

lib/CMakeLists.txt

```cmake
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

SET(LIBHELLO_SRC hello.cpp)

add_library(hello SHARED ${LIBHELLO_SRC})
add_library(hello_static STATIC ${LIBHELLO_SRC})

set_target_properties(hello PROPERTIES VERSION 1.2 SOVERSION 1)

set_target_properties(hello_static PROPERTIES OUTPUT_NAME "hello")
# set_target_properties(hello PROPERTIES OUTPUT_NAME "hello")

get_target_property(OUTPUT_VALUE hello_static OUTPUT_NAME)
message(STATUS "This is the hello_static_lib OUTPUT_NAME: " ${OUTPUT_VALUE})


INSTALL(TARGETS hello hello_static
        LIBRARY DESTINATION lib 
        ARCHIVE DESTINATION lib)

INSTALL(FILES hello.h DESTINATION include/hello)
```

CMakeLists.txt

```cmake
cmake_minimum_required(VERSION 3.10)

project(hello_cmake_lib)

add_subdirectory(lib)
```



#### 解析

指令 ADD_LIBRARY

```cmake
ADD_LIBRARY(libname 
	[SHARED|STATIC|MODULE]
    [EXCLUDE_FROM_ALL]
    source1 source2 ... sourceN)
```

不需要写全 libhello.so，只需要填写 hello 即可，cmake 系统会自动为你生成 libhello.X。

类型有三种: 

- SHARED，动态库 
- STATIC，静态库 
- MODULE，在使用 dyld 的系统有效，如果不支持 dyld，则被当作 SHARED 对待。 

EXCLUDE_FROM_ALL 参数的意思是这个库不会被默认构建，除非有其他的组件依赖或者手工构建。



指令 SET_TARGET_PROPERTIES

```cmake
SET_TARGET_PROPERTIES(target1 target2 ...
    PROPERTIES prop1 value1
    prop2 value2 ...)
```

这条指令可以用来设置输出的名称，对于动态库，还可以用来指定动态库版本和 API 版本。

对应指令 GET_TARGET_PROPERTY

```cmake
GET_TARGET_PROPERTY(VAR target property)
```

这条指令用来获取属性的值，可以通过 message 来输出。



**动态库版本号**

按照规则，动态库是应该包含一个版本号的。一般情况是 ：

> libhello.so.1.2 
>
> libhello.so ->libhello.so.1 
>
> libhello.so.1->libhello.so.1.2

为了实现动态库版本号，仍然需要使用 SET_TARGET_PROPERTIES 指令。

```cmake
SET_TARGET_PROPERTIES(hello PROPERTIES VERSION 1.2 SOVERSION 1) 
```

VERSION 指代动态库版本，SOVERSION 指代 API 版本。



**安装共享库和头文件**

需要将 libhello.a, libhello.so.x 以及 hello.h 安装到系统目录，才能真正让其他人开发使用。

```cmake
INSTALL(TARGETS hello hello_static
        LIBRARY DESTINATION lib 
        ARCHIVE DESTINATION lib)

INSTALL(FILES hello.h DESTINATION include/hello)
```



#### 构建

运行下述代码，新建build构建文件夹，并运行cmake命令。

```shell
mkdir build && cd build
cmake -DCMAKE_INSTALL_PREFIX=../../tmp ..
make
make install
```

进入/tmp 目录看一下安装结果：

> ├── include
> │   └── hello
> │       └── hello.h
> └── lib
>     ├── libhello.a
>     ├── libhello.so -> libhello.so.1
>     ├── libhello.so.1 -> libhello.so.1.2
>     └── libhello.so.1.2



## 进阶三

文件树

> ├── CMakeLists.txt
> ├── subbinary							// 可执行文件，主程序
> │   ├── CMakeLists.txt
> │   └── main.cpp
> ├── sublib1								// 静态库
> │   ├── CMakeLists.txt
> │   ├── include
> │   │   └── sublib1.h
> │   └── src
> │       └── sublib1.cpp
> ├── sublib2								// 动态库
> │   ├── CMakeLists.txt
> │   ├── include
> │   │   └── sublib2.h
> │   └── src
> │       └── sublib2.cpp
> └── sublib3								// 只有头文件的库
>     ├── CMakeLists.txt
>     └── include
>         └── sublib3.h



sublib1/include/sublib1.h

```cpp
#ifndef __SUBLIB_1_H_
#define __SUBLIB_1_H_

class sublib1 {
 public:
  void print();
};

#endif
```

sublib1/src/sublib1.cpp

```cpp
#include <iostream>
#include "sublib1.h"

void sublib1::print() {
    std::cout << "Hello sub-library 1..." << std::endl;
}
```

sublib1/CMakeLists.txt

```cmake
project(sublibrary1)

add_library(${PROJECT_NAME} STATIC src/sublib1.cpp)
add_library(sub::lib1 ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/include
)
```



sublib2/include/sublib2.h 内容同sublib1.h

sublib2/src/sublib2.cpp 内容同sublib1.cpp

sublib2/CMakeLists.txt

```cmake
project(sublibrary2)

add_library(${PROJECT_NAME} SHARED src/sublib2.cpp)
add_library(sub::lib2 ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/include
)
```



sublib3/include/sublib3.h

```cpp
#ifndef __SUBLIB_3_H_
#define __SUBLIB_3_H_

#include <iostream>

class sublib3 {
 public:
  void print() {
    std::cout << "Hello header only sub-library 3..." << std::endl;
  }
};

#endif
```

sublib3/CMakeLists.txt

```cmake
project(sublibrary3)

add_library(${PROJECT_NAME} INTERFACE)
add_library(sub::lib3 ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME}
    INTERFACE ${PROJECT_SOURCE_DIR}/include 
)
```



subbinary/main.cpp

```cpp
#include "sublib1.h"
#include "sublib2.h"
#include "sublib3.h"

int main() {
    sublib1 h1;
    h1.print();

    sublib2 h2;
    h2.print();

    sublib3 h3;
    h3.print();

    return 0;
}
```

subbinary/CMakeLists.txt

```cmake
project(subbinary)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME}
    sub::lib1
    sub::lib2 
    sub::lib3
)
```



CMakeLists.txt

```cmake
cmake_minimum_required(VERSION 3.10)

project(hello_cmake_lib)

add_subdirectory(sublib1)
add_subdirectory(sublib2)
add_subdirectory(sublib3)
add_subdirectory(subbinary)
```



#### 解析

指令 TARGET_LINK_LIBRARIES

```cmake
TARGET_LINK_LIBRARIES(target library1
    <debug | optimized> library2
    ...)
```

这个指令可以用来为 target 添加需要链接的共享库，但是同样可以用于为自己编写的共享库添加共享库链接。



#### 构建

运行下述代码，新建build构建文件夹，并运行cmake命令。

```shell
mkdir build && cd build
cmake ..
make
```

进入/build目录（部分展示）看一下生成的共享库：

> ├── subbinary
> │   ├── CMakeFiles
> │   ├── cmake_install.cmake
> │   ├── Makefile
> │   └── subbinary
> ├── sublib1
> │   ├── CMakeFiles
> │   ├── cmake_install.cmake
> │   ├── libsublibrary1.a
> │   └── Makefile
> ├── sublib2
> │   ├── CMakeFiles
> │   ├── cmake_install.cmake
> │   ├── libsublibrary2.so
> │   └── Makefile
> └── sublib3
>     ├── CMakeFiles
>     ├── cmake_install.cmake
>     └── Makefile



## 进阶四

文件树

> ├── CMakeLists.txt
> └── main.cpp

main.cpp

```cpp
#include <iostream>
#include <boost/shared_ptr.hpp>
#include <boost/filesystem.hpp>
using namespace std;

int main() {
    cout << "Third Party Libs Include!" << endl;

    boost::shared_ptr<int> isp(new int(4));

    boost::filesystem::path path = "usr/share/cmake/modules";
    if (path.is_relative()) {
        cout << "Path is relative." << endl;
    } else {
        cout << "Path is not relative." << endl;
    }
    
    return 0;
}
```

CMakeLists.txt

```cmake
cmake_minimum_required(VERSION 3.5)

project(third_party_libs)

# 使用库文件系统和系统查找 boost install
# find a boost install with the libraries filesystem and system
find_package(Boost 1.46.1 REQUIRED COMPONENTS filesystem system)

if (Boost_FOUND)
    message("boost find")
    include_directories(${Boost_INCLUDE_DIRS})
else ()
    message(FATAL_ERROR "Cannot find Boost")
endif()

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME}
    PRIVATE Boost::filesystem
)
```



#### 解析

指令 FIND_PACKAGE

```cmake
FIND_PACKAGE(<name> [major.minor] [QUIET] [NO_MODULE]
	[[REQUIRED|COMPONENTS] [componets...]])
```

用来调用预定义在 CMAKE_MODULE_PATH 下的 Find.cmake 模块，你也可以自己定义 Find模块，通过 SET(CMAKE_MODULE_PATH dir)将其放入工程的某个目录中供工程使用。

对于系统预定义的 Find.cmake 模块，使用方法一般如上例所示。每一个模块都会定义以下几个变量 

- `<name>_FOUND`
- `<name>_ INCLUDE_DIR` or `<name>_INCLUDES  `
- `<name>_LIBRARY` or `<name>_LIBRARIES`  

可以通过 `<name>_FOUND` 来判断模块是否被找到，如果没有找到，按照工程的需要关闭某些特性、给出提醒或者中止编译，上面的例子就是报出致命错误并终止构建。

如果  `<name>_FOUND` 为真，则将 `<name>_ INCLUDE_DIR`  加入 INCLUDE_DIRECTORIES， 将 `<name>_LIBRARY` 加入 TARGET_LINK_LIBRARIES 中。

REQUIRED 参数，其含义是指这个共享库是否是工程必须的，如果使用了这个参数，说明这 个链接库是必备库，如果找不到这个链接库，则工程不能编译。

COMPONENTS - 要查找的库列表。从后面的参数代表的库里找boost。



#### 构建

运行下述代码，新建build构建文件夹，并运行cmake命令。

```shell
mkdir build && cd build
cmake ..
make
```

检查一下 third_party_libs 的链接情况：

> ldd third_party_libs
>
> ​		linux-vdso.so.1 (0x00007ffd8edc4000)
> ​        libboost_filesystem.so.1.65.1 => /usr/lib/x86_64-linux-gnu/libboost_filesystem.so.1.65.1 (0x00007f9ccec10000)
> ​        libboost_system.so.1.65.1 => /usr/lib/x86_64-linux-gnu/libboost_system.so.1.65.1 (0x00007f9ccea0b000)





## 进阶五

#### 常用变量和环境变量

见《Cmake Pratice》p31-33



#### 常用指令

见《Cmake Pratice》p34-42





参考：

1. 《Cmake Pratice》
2. [ttroy50/cmake-exxample](https://github.com/ttroy50/cmake-examples/) 
3. [SFUMECJF/cmake-examples-Chinese](https://github.com/SFUMECJF/cmake-examples-Chinese) 

