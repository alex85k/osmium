MESSAGE(test_geometry_ogr)
add_executable(test_geometry_ogr "test_geometry.cpp" ${COMMON_SRC})
target_link_libraries (test_geometry_ogr ${COMMON_LIBS} ${GDAL_LIBRARIES})
ADD_TEST(test_geometry_ogr test_geometry_ogr)
