SET(TESTS_FILES
	test_linestring_geometry
	test_point_geometry
	test_polygon_geometry
	test_haversine
)

FOREACH(test_name ${TESTS_FILES})
MESSAGE(${test_name})
	add_executable(${test_name} "${test_name}.cpp" ${COMMON_SRC})
	target_link_libraries (${test_name} ${COMMON_LIBS} ${GEOS_LIBRARY})
	ADD_TEST(${test_name} ${test_name})
ENDFOREACH(test_name)
