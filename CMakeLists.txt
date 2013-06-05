SET(PACKAGE OSMIUM)
SET(PACKAGE_NAME OSMIUM)

CMAKE_MINIMUM_REQUIRED(VERSION 2.8 FATAL_ERROR)

PROJECT(OSMIUM)

#SET(CMAKE_MODULE_PATH "${GD_SOURCE_DIR}/cmake/modules")

SET(SEARCH_PREFIX "d:/usr" CACHE PATH "Place where to search additional libraries")
SET(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} ${SEARCH_PREFIX})

OPTION(ENABLE_PNG "Enable PNG support" 0)

include(FindBoost)
set (Boost_USE_STATIC_LIBS ON)
set (Boost_USE_MULTITHREADED ON)

set (BOOST_COMPONENTS unit_test_framework regex)
find_package (Boost 1.53 COMPONENTS ${BOOST_COMPONENTS} REQUIRED)

IF(CMAKE_COMPILER_IS_GNUCC)
  set(CMAKE_CXX_FLAGS "-O3 -Wall -Wextra -Wredundant-decls -Wdisabled-optimization -pedantic")
ENDIF(CMAKE_COMPILER_IS_GNUCC)


if(Boost_FOUND)
	include_directories(${Boost_INCLUDE_DIRS})
else()
    set(BOOST_ROOT "NOT FOUND: please choose" CACHE PATH "")
    MESSAGE("PLEASE, specify the directory where the Boost library is installed in BOOST_ROOT")
endif(Boost_FOUND)

find_package(ZLIB)
include_directories(${ZLIB_INCLUDE_DIR})

find_path(OSMPBF_INCLUDE_DIR osmpbf/osmpbf.h)
find_library(OSMPBF_LIBRARY NAMES osmpbf )
include_directories(${OSMPBF_INCLUDE_DIR})

find_package(EXPAT REQUIRED)
include_directories(${EXPAT_INCLUDE_DIR})

find_path(SQLITE3_INCLUDE_DIR sqlite3.h)
find_library(SQLITE3_LIBRARY NAMES sqlite3 )
include_directories(${SQLITE3_INCLUDE_DIR})

find_package(Protobuf REQUIRED)
include_directories(${PROTOBUF_INCLUDE_DIR})

find_path(GD_INCLUDE_DIR gd.h)
find_library(GD_LIBRARY NAMES libgd-static)
include_directories(${GD_INCLUDE_DIR})

#find_path(GEOS_INCLUDE_DIR gd.h)
#find_library(GEOS_LIBRARY NAMES libgeos)
#include_directories(${GEOS_INCLUDE_DIR})


find_package(Threads)

if(WIN32)
	set(CPACK_GENERATOR ZIP)
else(WIN32)
	set(CPACK_GENERATOR TGZ)
endif(WIN32)

include_directories(include)

enable_testing()
add_subdirectory(test)


install( DIRECTORY include/osmium DESTINATION include)
install( FILES include/osmium.hpp DESTINATION include)

