cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project (Renascence)
MESSAGE(STATUS "operation system is ${CMAKE_SYSTEM}")

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g -O3")
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 ")

include_directories(include ./third/tinyxml/ ./third/pystring/ ./third/ ./third/protobuf2json-c/include/ ./)
file(GLOB SRC_LIBS "src/*/*.*" "src/*/*.c" "src/*.cpp" "./third/tinyxml/tiny*.cpp" "./third/pystring/pystring.cpp" "./third/protobuf-c/protobuf-c.c" "./proto/*.c" "./third/protobuf2json-c/src/*.c" "include/*/*" "include/*")
file(GLOB SRC_TEST "test/*.cpp")

add_library(Renascence SHARED ${SRC_LIBS})
add_library(Renascence_Static STATIC ${SRC_LIBS})
add_executable(test.out ${SRC_TEST})
add_executable(example.out ./simple_main.cpp)
if (CMAKE_SYSTEM_NAME MATCHES "Linux")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")
    target_link_libraries(Renascence dl)
endif()
target_link_libraries(test.out Renascence)
target_link_libraries(example.out Renascence)

install(TARGETS Renascence LIBRARY DESTINATION /usr/local/lib)

include_directories(example/graphic/include example/graphic/third_party/libjpeg/ include/)
file(GLOB SRC_LIBS_PICS "example/graphic/src/*/*.cpp" "example/graphic/src/*/*.c" "example/graphic/third_party/*/*.c")

add_library(pics SHARED ${SRC_LIBS_PICS})

find_package (Threads)
include_directories(include example/MultiThreadParallelMachine/include/)
file(GLOB SRC_LIBS_MGP "example/MultiThreadParallelMachine/src/*.cpp")

#add_library(mthpGP SHARED ${SRC_LIBS_MGP})

#target_link_libraries(mthpGP Renascence)
#target_link_libraries(mthpGP ${CMAKE_THREAD_LIBS_INIT})

