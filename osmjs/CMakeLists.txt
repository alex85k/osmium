IF(CMAKE_COMPILER_IS_GNUCC)
    SET(WARNING_FLAGS "-Wall -Wextra -Wdisabled-optimization -pedantic -Wctor-dtor-privacy -Wnon-virtual-dtor -Woverloaded-virtual -Wsign-promo -Wno-long-long")
    SET(CXX_FLAGS "-O3 -g")
    SET(CMAKE_CXX_FLAGS ${CXX_FLAGS} ${WARNINGS_FLAGS})
ENDIF(CMAKE_COMPILER_IS_GNUCC)

add_definitions( -D_LARGEFILE_SOURCE -D_FILE_OFFSET_BITS=64 )

find_path(V8_INCLUDE_DIR v8.h)
find_library(V8_LIBRARY NAMES v8)
find_library(ICUUC_LIBRARY NAMES icuuc)

if (V8_INCLUDE_DIR AND V8_LIBRARY) 
   SET(V8_FOUND 1)
endif()

if (LIBXML2_FOUND AND GEOS_FOUND AND GEOS_FOUND AND V8_FOUND)
   include_directories(${GEOS_INCLUDE_DIR})
   include_directories(${SHP_INCLUDE_DIR})
   include_directories(${V8_INCLUDE_DIR})
   add_executable(osmjs "osmjs.cpp")
   target_link_libraries (osmjs ${EXPAT_LIBRARY} ${OSMPBF_LIBRARIES} ${GEOS_LIBRARY} ${SHP_LIBRARY} ${V8_LIBRARY} ${ICUUC_LIBRARY})
   install(TARGETS osmjs RUNTIME DESTINATION bin)
else()
   MESSAGE("can not comipile JS")
endif()


