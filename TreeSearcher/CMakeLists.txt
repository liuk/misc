# Setup the project
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(treeSearcher CXX C)

# Put executables into a bin
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_MACOSX_RPATH 1)  # only needed to supress cmake warning

# Command line options
option(DEBUG    "General debug flag for non-production compiles" ON)
if(NOT(DEBUG))
  add_definitions(-DNDEBUG)
endif()

# Add version number from git if exists
if(EXISTS "${PROJECT_SOURCE_DIR}/.git")
  execute_process(COMMAND git rev-parse --abbrev-ref HEAD OUTPUT_VARIABLE GIT_BRANCH OUTPUT_STRIP_TRAILING_WHITESPACE)
  execute_process(COMMAND git rev-parse --short HEAD      OUTPUT_VARIABLE GIT_HASH   OUTPUT_STRIP_TRAILING_WHITESPACE)
  add_definitions("-DGIT_VERSION=\"${GIT_BRANCH}-${GIT_HASH}\"")
else()
  add_definitions("-DGIT_VERSION=\"1.0.0\"")
endif()

# ROOT dict generation
add_custom_command (
  OUTPUT ${PROJECT_SOURCE_DIR}/src/TSDict.cxx ${PROJECT_SOURCE_DIR}/src/TSDict.h
  COMMAND rootcint
  ARGS -f ${PROJECT_SOURCE_DIR}/src/TSDict.cxx -c ${PROJECT_SOURCE_DIR}/inc/TrackParam.h ${PROJECT_SOURCE_DIR}/inc/HitCombination.h ${PROJECT_SOURCE_DIR}/inc/TrackCandidate.h ${PROJECT_SOURCE_DIR}/inc/LinkDef.h
  )

# tree searcher source code
include_directories(${PROJECT_SOURCE_DIR}/inc)
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cxx)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.h)
add_executable(treeSearcher treeSearcher.cxx ${sources} ${headers} ${PROJECT_SOURCE_DIR}/src/TSDict.cxx)
add_executable(lutMaker lutMaker.cxx ${sources} ${headers} ${PROJECT_SOURCE_DIR}/src/TSDict.cxx)
add_executable(test1 test1.cxx ${sources} ${headers} ${PROJECT_SOURCE_DIR}/src/TSDict.cxx)

# ROOT
find_program(ROOTCONF "root-config")
if(ROOTCONF)
  message("-- Detecting ROOT:    found at ${ROOTCONF}")
else()
  message(FATAL_ERROR "-- Detecting ROOT:    not found")
endif()
execute_process(COMMAND root-config --prefix OUTPUT_VARIABLE ROOT_PREFIX  OUTPUT_STRIP_TRAILING_WHITESPACE)
execute_process(COMMAND root-config --cflags OUTPUT_VARIABLE ROOT_CFLAGS  OUTPUT_STRIP_TRAILING_WHITESPACE)
execute_process(COMMAND root-config --libs   OUTPUT_VARIABLE ROOT_LINK    OUTPUT_STRIP_TRAILING_WHITESPACE)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${ROOT_CFLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${ROOT_LINK} ")
target_link_libraries(treeSearcher ${ROOT_LINK})
target_link_libraries(lutMaker ${ROOT_LINK})
target_link_libraries(test1 ${ROOT_LINK})
