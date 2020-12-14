cmake_minimum_required(VERSION 3.1)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake/modules)

project(Vox)
set(Vox_VERSION 2.0.0)

set(CMAKE_CXX_FLAGS "-g")
#set(C_CXX_FLAGS "${C_CXX_FLAGS} -ftrack-macro-expansion=0 -fno-diagnostics-show-caret")


find_package(SDL2 REQUIRED)
if(NOT SDL2_FOUND)
    message(FATAL_ERROR "SDL2 library not found")
endif(NOT SDL2_FOUND)
include_directories(${SDL2_INCLUDE_DIR})

find_package(SDL2_mixer REQUIRED)
if(NOT SDLMIXER_FOUND)
    message(FATAL_ERROR "SDL2_mixer library not found")
endif(NOT SDLMIXER_FOUND)
include_directories(${SDL_MIXER_INCLUDE_DIR})

#find_package(Lua REQUIRED)

include_directories(external/glm)
include_directories(external/glfw/include/GLFW)
include_directories(external/libnoise/include)
include_directories(external/libnoise/examples)
include_directories(external/lua)

add_subdirectory(external)
add_subdirectory(src)
