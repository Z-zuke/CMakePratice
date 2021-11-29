SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

SET(LIBHELLO_SRC hello.cpp)

add_library(hello SHARED ${LIBHELLO_SRC})
add_library(hello_static STATIC ${LIBHELLO_SRC})

set_target_properties(hello PROPERTIES VERSION 1.2 SOVERSION 1)

set_target_properties(hello_static PROPERTIES OUTPUT_NAME "hello")
# set_target_properties(hello_shared PROPERTIES OUTPUT_NAME "hello")

get_target_property(OUTPUT_VALUE hello_static OUTPUT_NAME)
message(STATUS "This is the hello_static_lib OUTPUT_NAME: " ${OUTPUT_VALUE})


INSTALL(TARGETS hello hello_static
        LIBRARY DESTINATION lib 
        ARCHIVE DESTINATION lib)

INSTALL(FILES hello.h DESTINATION include/hello)