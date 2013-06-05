
SET(DART_TESTING_TIMEOUT 5)
INCLUDE(Dart)

MESSAGE ("testing!!!")

SET(COMMON_SRC ../../test_main.cpp ../../test_utils.cpp)
MESSAGE ("Boost libs: " ${BOOST_LIBRARIES})
SET(COMMON_LIBS ${Boost_LIBRARIES})

include_directories("../..")

add_subdirectory (t/tags)
add_subdirectory (t/utils)
add_subdirectory (t/osm)
add_subdirectory (t/osmfile)

