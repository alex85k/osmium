
SET(DART_TESTING_TIMEOUT 5)
INCLUDE(Dart)

MESSAGE ("testing!!!")

SET(COMMON_SRC ../../test_main.cpp ../../test_utils.cpp)
SET(COMMON_LIBS ${Boost_LIBRARIES})

include_directories(".")

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
MESSAGE("GEOS library not found, skipping test groups: geometry, geometry_geos")
endif()
