cmake_minimum_required(VERSION 3.10)

PROJECT(hello_cmake)
ADD_SUBDIRECTORY(src bin) 



INSTALL(FILES COPYRIGHT README DESTINATION share/doc/cmake/t2)
INSTALL(DIRECTORY doc/ DESTINATION share/doc/cmake/t2)

INSTALL(PROGRAMS runhello.sh DESTINATION bin)
