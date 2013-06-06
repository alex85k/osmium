MESSAGE(test_geometry_geos)
add_executable(test_geometry_geos "test_point_geometry.cpp" ${COMMON_SRC})
target_link_libraries (test_geometry_geos ${COMMON_LIBS} ${GEOS_LIBRARY})
ADD_TEST(test_geometry_geos test_geometry_geos)
