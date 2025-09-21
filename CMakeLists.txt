cmake_minimum_required(VERSION 3.24)
project(PvZ-GW-Map-Loader VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_C_STANDARD 23)
set(CMAKE_CXX_STANDARD 23)

include_directories(
    ${CMAKE_SOURCE_DIR}/Include
    ${CMAKE_SOURCE_DIR}/ThirdParty/MinHook
    ${CMAKE_SOURCE_DIR}/ThirdParty/imgui
)

# Project header files
file(GLOB_RECURSE PROJECT_SOURCES_HH
    ${CMAKE_SOURCE_DIR}/Include/*.hpp
    ${CMAKE_SOURCE_DIR}/Include/*.h
)

# Project source files
file(GLOB_RECURSE PROJECT_SOURCES_CXX
    ${CMAKE_SOURCE_DIR}/Source/*.cpp
    ${CMAKE_SOURCE_DIR}/Source/*.c
)

file(GLOB_RECURSE THIRDPARTY_SOURCES_HH
    ${CMAKE_SOURCE_DIR}/ThirdParty/*.hpp
    ${CMAKE_SOURCE_DIR}/ThirdParty/*.h
)

file(GLOB_RECURSE THIRDPARTY_SOURCES_CXX
    ${CMAKE_SOURCE_DIR}/ThirdParty/*.cpp
    ${CMAKE_SOURCE_DIR}/ThirdParty/*.c
)

if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -W4 -WX")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror")
endif()

add_library(${CMAKE_PROJECT_NAME} SHARED 
    ${PROJECT_SOURCES_CXX}
    ${PROJECT_SOURCES_HH}
    ${THIRDPARTY_SOURCES_CXX}
    ${THIRDPARTY_SOURCES_HH}
)

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE 
    "${CMAKE_SOURCE_DIR}/ThirdParty/MinHook/libMinHook.x64.lib"
    d3d11.lib
)

# Include this bit to export our proxy function as DirectInput8Create
target_sources(${CMAKE_PROJECT_NAME} PRIVATE
    "${CMAKE_SOURCE_DIR}/Source/DllProxy.def"
)