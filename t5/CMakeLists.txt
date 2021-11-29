cmake_minimum_required(VERSION 3.5)

if(COMPILER_SUPPORTS_CXX11)#
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)#
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
    message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

project(third_party_libs)

# 使用库文件系统和系统查找 boost install
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
