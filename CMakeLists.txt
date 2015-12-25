PROJECT(smnet)

set (smnet_VERSION_MAJOR 0)
set (smnet_VERSION_MINOR 1)

message(${CMAKE_SYSTEM_NAME})

cmake_minimum_required(VERSION 2.8)

set(LIBRARY_PATH ${LIBRARY_PATH}
		      /lib
		      /usr/lib
		      /usr/lib64
		      /usr/local/lib
		      /usr/local/lib64
		      /usr/lib/x86_64-linux-gnu)

set(INCLUDE_DIRS ${INCLUDE_DIRS} /usr/includes)
include_directories(SYSTEM ${INCLUDE_DIRS})

set (INCLUDES ${INCLUDES} includes)
# Any CPP file of the library here!!!
set (SOURCES ${SOURCES} ${INCLUDES}/DeltaPath/DeltaPath.cpp
                        ${INCLUDES}/DeltaPathFinder/DeltaPathFinder.cpp
                        ${INCLUDES}/HypernymHandler/HypernymHandler.cpp
                        ${INCLUDES}/HyponymHandler/HyponymHandler.cpp
                        ${INCLUDES}/SemanticGraph/SemanticGraph.cpp
                        ${INCLUDES}/SemanticWordNet/SemanticWordNet.cpp
                        ${INCLUDES}/SynonymHandler/SynonymHandler.cpp
                        ${INCLUDES}/Wrapper/Wrapper.cpp
                        )

# Setup library stuff
add_library (lsmnet OBJECT ${SOURCES})
add_library (lsmnet.0.1 SHARED $<TARGET_OBJECTS:lsmnet>)
set(CMAKE_COLOR_MAKEFILE on)
set(CMAKE_BUILD_TYPE Debug)

# BOOST 
find_package( Boost 1.49 REQUIRED )
include_directories( ${Boost_INCLUDE_DIR} )

# Cereal C++11
find_path(CerealIncludes cereal/details/helpers.hpp PATHS ${INCLUDE_DIRS})
if(NOT CerealIncludes)
  message(FATAL_ERROR "Can't find Header Dir containing cereal/details/helpers.hpp")
endif()

# WordNet 3.0
# NOTE: OSX fix is to set the flags to search given path, Linux doesn't need this
# TODO: use if-else with${CMAKE_SYSTEM_NAME} and perform the correct check!
# PATHS /usr/local/WordNet-3.0 PATH_SUFFIXES lib)
find_library(WORDNET NAMES wordnet WN WN.a libWN.a wordnet-3.0) 
if (WORDNET)
    message(STATUS ${WORDNET})
    target_link_libraries(lsmnet.0.1 ${WORDNET})
else (!WORDNET)
    message(FATAL_ERROR "ERROR: missing libwordnet")
endif(WORDNET)

# Threads
find_package (Threads)
target_link_libraries (lsmnet.0.1 ${CMAKE_THREAD_LIBS_INIT})

# TODO: Add Example
#add_executable (example1 examples/???.cpp)


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

# Install Library and Headers
install (TARGETS ${LIB_SHARED} DESTINATION /usr/local/lib/)
install (DIRECTORY includes/ 
         DESTINATION /usr/local/include/${PROJECT_NAME} 
         FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp" PATTERN "*.hxx")
