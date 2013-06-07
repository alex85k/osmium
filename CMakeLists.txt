SET(PACKAGE OSMIUM)
SET(PACKAGE_NAME OSMIUM)

CMAKE_MINIMUM_REQUIRED(VERSION 2.8 FATAL_ERROR)

PROJECT(OSMIUM)

OPTION(BUILD_EXAMPLES "compile example programs" 1)
OPTION(BUILD_TESTING "compile tests, please run them with ctest" 1)
OPTION(BUILD_OSMJS  "compile osmjs executable - V8 needed" 1)
OPTION(BUILD_WITH_STATIC_BOOST "compile tests and examples with static Boost libraries" 0)
OPTION(BUILD_WITH_MULTITHREADED_BOOST "compile tests and examples with multithreaded Boost libraries" 1)


SET(SEARCH_PREFIX "" CACHE PATH "Place where to search additional libraries")
SET(OLD_PATH ${CMAKE_PREFIX_PATH})
SET(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} ${SEARCH_PREFIX})

########## section for manual editing (if you do not want to run cmake-gui or give complex command line)
#SET(BUILD_EXAMPLES 1)
#SET(BUILD_TESTING 1)
#SET(BUILD_OSMJS 1)
#SET(BUILD_WITH_STATIC_BOOST 1)
#SET(BUILD_WITH_MULTITHREADED_BOOST 1)
set(Boost_DEBUG ON)
#set(BOOST_ROOT D:/LIBS/BOOST)
#set(ZLIB_INCLUDE_DIR D:/LIBS/ZLIB/include)
#set(ZLIB_LIBRARY D:/LIBS/ZLIB/lib/zlibstatic.lib)
#set(EXPAT_INCLUDE_DIR D:/LIBS/expat/include)
#set(EXPAT_LIBRARY D:/LIBS/expat/lib/libexpat.lib)
#set(PROTOBUF_INCLUDE_DIR D:/LIBS/Protobuf25/include)
#set(PROTOBUF_LITE_LIBRARY D:/LIBS/Protobuf25/lib/libprotobuf-lite.lib)
########## section for manual editing 


include(FindBoost)
if (BUILD_WITH_STATIC_BOOST)
	set (Boost_USE_STATIC_LIBS ON)
else()
	set (Boost_USE_STATIC_LIBS OFF)
endif()

if (BUILD_WITH_MULTITHREADED_BOOST)
	set (Boost_USE_MULTITHREADED ON)
else()
	set (Boost_USE_MULTITHREADED OFF)
endif()


find_package (Boost 1.46 REQUIRED)
MARK_AS_ADVANCED(CLEAR BOOST_ROOT)

INCLUDE (CheckIncludeFiles)
CHECK_INCLUDE_FILES(winsock2.h HAVE_WINSOCK)
if(HAVE_WINSOCK)
   add_definitions( -DD_HAVE_WINSOCK )
   LIST(APPEND WIN_LIBS ws2_32)
endif() 

if(CMAKE_COMPILER_IS_GNUCC AND WIN32)
LIST(APPEND WIN_LIBS iconv)
endif()

find_package(ZLIB)
find_package(EXPAT)

find_package(Protobuf)
if ( PROTOBUF_INCLUDE_DIR AND PROTOBUF_LITE_LIBRARY )
    include_directories(${PROTOBUF_INCLUDE_DIR})
else()
    MARK_AS_ADVANCED(CLEAR PROTOBUF_INCLUDE_DIR PROTOBUF_LITE_LIBRARY )
    message(FATAL_ERROR "Please, specify the Google PROTOBUF library dir")
endif()

find_package(Threads)


##### Find OSMPBF  Library 
find_path(OSMPBF_INCLUDE_DIR osmpbf/osmpbf.h)
find_library(OSMPBF_LIBRARY NAMES osmpbf )
#####

# check that the essential libraries were found
if (BUILD_EXAMPLES OR BUILD_TESTING OR BUILD_TESTING OR BUILD_OSMJS)

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
else()
    set(BOOST_ROOT "NOT FOUND: please choose" CACHE PATH "")
    MESSAGE(FATAL_ERROR "PLEASE, specify the directory where the Boost library is installed in BOOST_ROOT")
endif(Boost_FOUND)

if ( ZLIB_FOUND )
    include_directories(${ZLIB_INCLUDE_DIR})
else(ZLIB)
    MARK_AS_ADVANCED(CLEAR ZLIB_LIBRARY ZLIB_INCLUDE_DIR)
    message(FATAL_ERROR "Please, specify the ZLIB includes directory and the library file")
endif( ZLIB_FOUND )

if ( EXPAT_FOUND )
    include_directories(${EXPAT_INCLUDE_DIR})
else(EXPAT_FOUND )
    MARK_AS_ADVANCED(CLEAR EXPAT_LIBRARY EXPAT_INCLUDE_DIR)
    message(FATAL_ERROR "Please, specify the EXPAT includes directory and the library file")
endif( EXPAT_FOUND  )

if(OSMPBF_LIBRARY AND OSMPBF_INCLUDE_DIR)
  include_directories(${OSMPBF_INCLUDE_DIR})
  SET(OSMPBF_FOUND 1)
  SET(OSMPBF_LIBRARIES ${OSMPBF_LIBRARY} ${PROTOBUF_LITE_LIBRARY} ${ZLIB_LIBRARY})
  MESSAGE( STATUS "OSMPBF library found")
else()
  MESSAGE( FATAL_ERROR "OSMPBF library not found, please specify its location manually")
endif()
endif()


find_package(LibXml2)

##### Find Google SparseHash
find_path(SPARSEHASH_INCLUDE_DIR sparsehash/sparsetable.h)
if(SPARSEHASH_INCLUDE_DIR)

  MESSAGE( STATUS "SparseHash library found")
  SET(SPARSEHASH_FOUND 1)
else()
  MESSAGE( STATUS "SparseHash library not found")
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
 SET(GDAL_LIBRARIES ${GDAL_LIBRARY} ${ZLIB_LIBRARY} ${WIN_LIBS} )
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

INCLUDE(CPack)
