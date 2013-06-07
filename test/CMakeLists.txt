
UNSET(Boost_LIBRARIES)
UNSET(Boost_FOUND)
find_package(Boost 1.38 COMPONENTS unit_test_framework regex)

if(Boost_UNIT_TEST_FRAMEWORK_LIBRARY)

SET(DART_TESTING_TIMEOUT 5)
INCLUDE(Dart)

SET(COMMON_SRC ../../test_main.cpp ../../test_utils.cpp)

SET(COMMON_LIBS ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

IF(NOT Boost_USE_STATIC_LIBS)
add_definitions(-DBOOST_TEST_DYN_LINK)
ENDIF()

include_directories(".")

IF(CMAKE_COMPILER_IS_GNUCC)
  set(CXX_FLAGS_WARNINGS "-Wall -Wextra -Wredundant-decls -Wdisabled-optimization -pedantic -Wctor-dtor-privacy -Wnon-virtual-dtor -Woverloaded-virtual -Wsign-promo -Wno-long-long")
  set(CXXFLAGS "-g")
  SET(CMAKE_CXX_FLAGS "${CXXFLAGS} ${CXX_FLAGS_WARNINGS}")
ENDIF(CMAKE_COMPILER_IS_GNUCC)

MESSAGE(STATUS ${CMAKE_CXX_FLAGS})

add_subdirectory (t/tags)
add_subdirectory (t/utils)
add_subdirectory (t/osm)
add_subdirectory (t/osmfile)

if(GEOS_FOUND)
   include_directories(${GEOS_INCLUDE_DIR})
   add_subdirectory (t/geometry)
   add_subdirectory (t/geometry_geos)
else()
MESSAGE("GEOS library not found, skipping test groups: geometry, geometry_geos")
endif()

if(GDAL_FOUND)
   include_directories(${GDAL_INCLUDE_DIR})
   add_subdirectory (t/geometry_ogr)
else()
   MESSAGE("GDAL library not found, skipping test groups: geometry_ogr")
endif()

else()
Message("Can not build tests without BOOST unit_test_framework library")
endif()