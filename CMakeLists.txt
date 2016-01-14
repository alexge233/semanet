PROJECT(smnet)
set(V_MAJOR 0)
set(V_MINOR 1)
message(${CMAKE_SYSTEM_NAME})
cmake_minimum_required(VERSION 2.8)
set(LIBRARY_PATH ${LIBRARY_PATH}
		      /lib
		      /usr/lib
		      /usr/lib64
		      /usr/local/lib
		      /usr/local/lib64
		      /usr/lib/x86_64-linux-gnu)

set(INCLUDE_DIRS ${INCLUDE_DIRS} /usr/includes
                                 /usr/local/includes)
include_directories(SYSTEM ${INCLUDE_DIRS})
set (SRC_DIR ${SRC_DIR} src)
include_directories(${SRC_DIR}/)
set (SOURCES ${SOURCES} 
                        ${SRC_DIR}/graph/graph.cpp
                        ${SRC_DIR}/path_finder/path_finder.cpp
                        ${SRC_DIR}/utility/utility.cpp)

# WordNet 3.0
# NOTE: OSX fix is to set the flags to search given path, Linux doesn't need this
# PATHS /usr/local/WordNet-3.0 PATH_SUFFIXES lib)
find_library(WORDNET NAMES wordnet WN WN.a libWN.a wordnet-3.0) 
if (WORDNET)
    message(STATUS ${WORDNET})
else (!WORDNET)
    message(FATAL_ERROR "ERROR: missing libwordnet")
endif(WORDNET)

# BOOST 
find_package(Boost 1.56 COMPONENTS serialization REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

# Setup library stuff
add_library (smnet OBJECT ${SOURCES})
add_library (smnet-${V_MAJOR}.${V_MINOR} SHARED $<TARGET_OBJECTS:smnet>)
target_link_libraries(smnet-${V_MAJOR}.${V_MINOR} ${Boost_SERIALIZATION_LIBRARY} ${WORDNET})
set(LIBOBJ ${LIBOBJ} smnet-${V_MAJOR}.${V_MINOR})

set(CMAKE_COLOR_MAKEFILE on)
set(CMAKE_BUILD_TYPE Release)

# Add hypernym example
add_executable(hypernym examples/hypernym.cpp)
add_executable(hyponym examples/hyponym.cpp)
add_executable(synonym examples/synonym.cpp)
add_executable(pathfind examples/pathfind.cpp)
add_executable(pathfind_loop examples/pathfind_loop.cpp)
add_executable(utilities examples/utilities.cpp)
add_executable(delta_best examples/delta_best.cpp)

target_link_libraries(hypernym ${LIBOBJ})
target_link_libraries(hyponym ${LIBOBJ})
target_link_libraries(synonym ${LIBOBJ})
target_link_libraries(pathfind ${LIBOBJ})
target_link_libraries(pathfind_loop ${LIBOBJ})
target_link_libraries(utilities ${LIBOBJ})
target_link_libraries(delta_best ${LIBOBJ})

# NOTE - Support for Linux and OSX - Not Windows !
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  message (STATUS "Clang/OSX" )
  execute_process(COMMAND ${CMAKE_C_COMPILER} -dumpversion OUTPUT_VARIABLE GCC_VERSION)
  if (CLANG_VERSION_MAJOR VERSION_GREATER 3 OR CLANG_VERSION_MAJOR VERSION_EQUAL 3)
    message(STATUS "Info: Clang >= 3.0 using C++11")
  elseif ()
    message(FATAL_ERROR "Clang version >= 3.0 is required")
  endif()
  set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++11 -Wall")
  message(STATUS "CXX Flags: " ${CMAKE_CXX_FLAGS})
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -mtune=native -march=native")
  set(BUILD_SHARED_LIBS OFF)
  message(STATUS "CXX Linker: " ${CMAKE_EXE_LINKER_FLAGS})

elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  message (STATUS "GCC/Linux" )
  execute_process(COMMAND ${CMAKE_C_COMPILER} -dumpversion OUTPUT_VARIABLE GCC_VERSION)
  if (GCC_VERSION VERSION_GREATER 4.9 OR GCC_VERSION VERSION_EQUAL 4.9)
    message(STATUS "Info: g++ version >= 4.9 using C++11")
  elseif ()
    message (FATAL_ERROR "g++ version >= 4.9 is required")
  endif()
  set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=gnu++11 -Wall -fPIC")
  message(STATUS "CXX Flags: " ${CMAKE_CXX_FLAGS})
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -mtune=native -march=native") # -s ?
  set(BUILD_SHARED_LIBS OFF)
  message(STATUS "CXX Linker: " ${CMAKE_EXE_LINKER_FLAGS})

elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
    message (FATAL_ERROR "unsupported compiler")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    message (FATAL_ERROR "unsupported compiler")
endif()

# Install stuff
install(TARGETS ${LIBOBJ}  DESTINATION /usr/local/lib/)
install(DIRECTORY src/ DESTINATION /usr/local/include/${PROJECT_NAME} FILES_MATCHING PATTERN "*.ihh" PATTERN "*.hpp" PATTERN "*.hxx")
# Master include file
install(FILES src/smnet DESTINATION /usr/local/include/${PROJECT_NAME})
