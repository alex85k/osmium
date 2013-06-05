SET(TESTS_FILES
	test_timestamp
)

find_package (Boost 1.53 COMPONENTS date_time REQUIRED)

FOREACH(test_name ${TESTS_FILES})
MESSAGE(${test_name})
	add_executable(${test_name} "${test_name}.cpp" ${COMMON_SRC})
	target_link_libraries (${test_name} ${COMMON_LIBS} ${Boost_DATE_TIME_LIBRARY})
	get_target_property(test_path ${test_name} LOCATION)
	ADD_TEST(${test_name} ${test_path})
ENDFOREACH(test_name)
