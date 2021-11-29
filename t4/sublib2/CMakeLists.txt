project(sublibrary2)

add_library(${PROJECT_NAME} SHARED src/sublib2.cpp)
add_library(sub::lib2 ALIAS ${PROJECT_NAME})

set_target_properties(${PROJECT_NAME} PROPERTIES VERSION 1.2 SOVERSION 1)

target_include_directories(${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/include
)