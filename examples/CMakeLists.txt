

IF(CMAKE_COMPILER_IS_GNUCC)
    SET(WARNING_FLAGS "-Wall -Wextra -Wdisabled-optimization -pedantic -Wctor-dtor-privacy -Wnon-virtual-dtor -Woverloaded-virtual -Wsign-promo -Wno-long-long")
    SET(CXX_FLAGS "-O3 -g")
    SET(CMAKE_CXX_FLAGS ${CXX_FLAGS} ${WARNINGS_FLAGS})
ENDIF(CMAKE_COMPILER_IS_GNUCC)

add_definitions( -D_LARGEFILE_SOURCE -D_FILE_OFFSET_BITS=64 )

SET(BASIC_LIBS ${EXPAT_LIBRARY} ${OSMPBF_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})

SET(SIMPLE_EXAMPLES
	osmium_debug
	osmium_find_bbox
	osmium_progress
	osmium_relation_members
	osmium_sizeof
	osmium_store_and_debug
	osmium_time
)

FOREACH(example ${SIMPLE_EXAMPLES})
MESSAGE(${example})
   add_executable(${example} "${example}.cpp")
   target_link_libraries (${example} ${BASIC_LIBS})
ENDFOREACH(example)

if(SPARSEHASH_FOUND)
   include_directories(${SPARSEHASH_INCLUDE_DIR})
endif()

SET(EXAMPLES_WITH_XML2
	osmium_convert
	osmium_range_from_history
)


if (LIBXML2_FOUND)
   include_directories(${LIBXML2_INCLUDE_DIR})
   FOREACH(example ${EXAMPLES_WITH_XML2})
   MESSAGE(${example})
      add_executable(${example} "${example}.cpp") 
      target_link_libraries (${example} ${LIBXML2_LIBRARIES} ${WIN_LIBS} ${BASIC_LIBS} )
   ENDFOREACH(example)
else()
   MESSAGE("LibXML2 not found, skipping examples " ${EXAMPLES_WITH_XML2})
endif()


SET(example osmium_mpdump)
if (GEOS_FOUND AND SPARSEHASH_FOUND)
   include_directories(${GEOS_INCLUDE_DIR})
   add_executable(${example} "${example}.cpp")
   MESSAGE(${example})
   target_link_libraries (${example} ${GEOS_LIBRARY} ${BASIC_LIBS} )
else()
   MESSAGE("GEOS+SparseHash not found, skipping example " ${example})
endif()

SET(example nodedensity)
if (GD_FOUND)
   include_directories(${GD_INCLUDE_DIR})
   include_directories(${PNG_INCLUDE_DIR})
   add_executable(${example} "${example}.cpp")
   MESSAGE(${example})
   target_link_libraries (${example} ${GD_LIBRARY} ${PNG_LIBRARIES} ${BASIC_LIBS} )
else()
   MESSAGE("GD not found, skipping example " ${example})
endif()

SET(EXAMPLES_WITH_GDAL 
	osmium_toogr	
	osmium_to_postgis)

if (GDAL_FOUND AND SPARSEHASH_FOUND)
   include_directories(${GDAL_INCLUDE_DIR})
   FOREACH(example ${EXAMPLES_WITH_GDAL})
      add_executable(${example} "${example}.cpp")
      MESSAGE(${example})
      target_link_libraries (${example}   ${GDAL_LIBRARIES} ${BASIC_LIBS})
   ENDFOREACH(example)
else()
   MESSAGE("GDAL+SparseHash not found, skipping examples " ${EXAMPLES_WITH_GDAL})
endif()


SET(example osmium_toogr2)
if (GDAL_FOUND AND GEOS_FOUND AND SPARSEHASH_FOUND)
   include_directories(${GDAL_INCLUDE_DIR})
   include_directories(${GEOS_INCLUDE_DIR})
   add_executable(${example} "${example}.cpp")
   MESSAGE(${example})
   target_link_libraries (${example} ${GDAL_LIBRARIES} ${GEOS_LIBRARY} ${BASIC_LIBS} )
else()
   MESSAGE("GDAL+GEOS+SparseHash not found, skipping example " ${example})
endif()


SET(example osmium_toshape)
if (SHP_FOUND AND SPARSEHASH_FOUND)
   include_directories(${SHP_INCLUDE_DIR})
   add_executable(${example} "${example}.cpp")
   MESSAGE(${example})
   target_link_libraries (${example}   ${SHP_LIBRARY} ${BASIC_LIBS} )
else()
   MESSAGE("libshp+SparseHash not found, skipping example " ${example})
endif()
