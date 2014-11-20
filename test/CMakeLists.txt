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
ADD_LIBRARY(hello_dll SHARED hello_dll.c)

ADD_EXECUTABLE(dynamic_lib.t ${SRC_LIST})

find_package(Boost COMPONENTS system filesystem REQUIRED)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})

ADD_EXECUTABLE(PluginIter.t PluginIter.t.cpp ../PluginIter.cpp)
target_link_libraries(PluginIter.t ${Boost_LIBRARIES})

ADD_EXECUTABLE(PluginManager.t PluginManager.t.cpp 
    ../PluginManager.cpp ../PluginIter.cpp ../DynamicLibrary_Unix.cpp simple_object.c
    ${LOKI}/src/SmallObj.cpp
    ${LOKI}/src/Singleton.cpp
    ${LOKI}/src/SmartPtr.cpp)
target_link_libraries(PluginManager.t ${Boost_LIBRARIES})

ADD_LIBRARY(simple_object_plugin SHARED simple_plugin.c simple_object.c)
