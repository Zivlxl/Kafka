cmake_minimum_required(VERSION 2.8)
project(Kafka)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -rdynamic -O3 -g -std=c++11 -Wall -Wno-deprecated -Werror -Wno-unused-function")

set(LIB_SRC
    src/log/log.cpp
        )

add_library(Kafka SHARED ${LIB_SRC})
#add_library(Kafka_static STATIC ${LIB_SRC})
#SET_TARGET_PROPERTIES(Kafka_static PROPERTIES OUTPUT_NAME "Kafka")

add_executable(test tests/test.cpp)
add_dependencies(test Kafka)
target_link_libraries(test Kafka)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH  ${PROJECT_SOURCE_DIR}/lib)