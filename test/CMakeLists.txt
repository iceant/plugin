cmake_minimum_required (VERSION 2.6)

project(PointCX::Plugin::Test)

SET(LOKI "/home/dev01/PointCX/repository/loki-lib")


SET(SRC_LIST DynamicLibrary.t.cpp 
			../DynamicLibrary_Unix.cpp 
			${LOKI}/src/SmallObj.cpp 
			${LOKI}/src/Singleton.cpp 
			${LOKI}/src/SmartPtr.cpp )

INCLUDE_DIRECTORIES(${LOKI}/include)

add_definitions(-DBUILD_DLL)
ADD_LIBRARY(hello_dll SHARED hello_dll.cpp)

ADD_EXECUTABLE(dynamic_lib.t ${SRC_LIST})
