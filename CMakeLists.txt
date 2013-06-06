SET(PACKAGE OSMIUM)
SET(PACKAGE_NAME OSMIUM)

CMAKE_MINIMUM_REQUIRED(VERSION 2.8 FATAL_ERROR)

PROJECT(OSMIUM)

OPTION(BUILD_EXAMPLES "compile example programs" 1)
OPTION(BUILD_TESTING "compile tests, please run them with ctest" 1)
OPTION(BUILD_OSMJS  "compile osmjs executable - V8 needed" 1)

SET(SEARCH_PREFIX "" CACHE PATH "Place where to search additional libraries")
SET(OLD_PATH ${CMAKE_PREFIX_PATH})
SET(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} ${SEARCH_PREFIX})

include(FindBoost)
#set (Boost_USE_STATIC_LIBS ON)
#set (Boost_USE_MULTITHREADED ON)

find_package (Boost 1.46 REQUIRED)

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
else()
    set(BOOST_ROOT "NOT FOUND: please choose" CACHE PATH "" FORCE)
    MESSAGE("PLEASE, specify the directory where the Boost library is installed in BOOST_ROOT")
endif(Boost_FOUND)
UNSET(Boost_LIBRARIES)
UNSET(Boost_FOUND)

INCLUDE (CheckIncludeFiles)
CHECK_INCLUDE_FILES(winsock2.h HAVE_WINSOCK)
if(HAVE_WINSOCK)
   add_definitions( -DD_HAVE_WINSOCK )
   LIST(ADD WIN_LIBS ws2_32)
endif() 

if(CMAKE_COMPILER_IS_GNUCC AND WIN32)
LIST(ADD WIN_LIBS iconv)
endif()

find_package(ZLIB REQUIRED)
include_directories(${ZLIB_INCLUDE_DIR})

find_package(EXPAT REQUIRED)
include_directories(${EXPAT_INCLUDE_DIR})

find_package(Protobuf REQUIRED)
include_directories(${PROTOBUF_INCLUDE_DIR})

find_package(Threads)
find_package(LibXml2)

##### Find OSMPBF  Library 
find_path(OSMPBF_INCLUDE_DIR osmpbf/osmpbf.h)
find_library(OSMPBF_LIBRARY NAMES osmpbf )
include_directories(${OSMPBF_INCLUDE_DIR})
if(OSMPBF_LIBRARY AND OSMPBF_INCLUDE_DIR)
  SET(OSMPBF_FOUND 1)
  SET(OSMPBF_LIBRARIES ${OSMPBF_LIBRARY} ${PROTOBUF_LITE_LIBRARY} ${ZLIB_LIBRARY})
  MESSAGE( STATUS "OSMPBF library found")
else()
  MESSAGE( STATUS "OSMPBF library found")
endif()
#####


##### Find GEOS Library 
find_path(GEOS_INCLUDE_DIR geos/geom.h)
find_library(GEOS_LIBRARY NAMES geos)
if(GEOS_INCLUDE_DIR AND GEOS_LIBRARY)
  MESSAGE( STATUS "GEOS library found")
  SET(GEOS_FOUND 1)
else()
  MESSAGE( STATUS "GEOS library not found")
endif()
#####

##### Find SHP Library 
find_path(SHP_INCLUDE_DIR shapefil.h )
if(NOT SHP_INCLUDE_DIR)
   find_path(SHP_INCLUDE_DIR2 libshp/shapefil.h)
   SET(SHP_INCLUDE_DIR ${SHP_INCLUDE_DIR2}/libshp)
endif()
find_library(SHP_LIBRARY NAMES shp)
if(SHP_INCLUDE_DIR AND SHP_LIBRARY)
  SET(SHP_FOUND 1)
  MESSAGE( STATUS "libshp library found")
else()
  MESSAGE( STATUS "libshp library not found")
endif()
#####

##### Find GD Library 
find_path(GD_INCLUDE_DIR gd.h REQUIRED)
find_library(GD_LIBRARY NAMES libgd-static gd)
if(GD_INCLUDE_DIR AND GD_LIBRARY)
  SET(GD_FOUND 1)
  find_package(PNG REQUIRED)
  MESSAGE( STATUS "GD library found")
else()
  MESSAGE( STATUS "GD library not found")
endif()
#####

find_package(GDAL)
if (GDAL_FOUND) 
 SET(GDAL_LIBRARIES ${GDAL_LIBRARY} ${ZLIB_LIBRARY} ${WIN_LIBS})
endif()

if(WIN32)
	ADD_DEFINITIONS( -DWIN32 -D_WIN32 -DMSWIN32 -DBGDWIN32 -DWINVER=0x0500  -D_WIN32_WINNT=0x0500 -D_WIN32_IE=0x0600)
	set(CPACK_GENERATOR ZIP)
else(WIN32)
	set(CPACK_GENERATOR TGZ)
endif(WIN32)

include_directories(BEFORE include)

enable_testing()


OPTION(OSMIUM_WITH_DEBUG 0)
if(OSMIUM_WITH_DEBUG)
 add_definitions( -DOSMIUM_WITH_DEBUG )
endif()


if(BUILD_EXAMPLES)
 add_subdirectory(examples)
endif()

if(BUILD_OSMJS)
 add_subdirectory(osmjs)
endif()

if(BUILD_TESTING)
  add_subdirectory(test)
endif()

install( DIRECTORY include/osmium DESTINATION include)
install( FILES include/osmium.hpp DESTINATION include)

