SET(TESTS_FILES
	test_filter
	test_regex_filter
	test_tag
	test_to_string
)

FOREACH(test_name ${TESTS_FILES})
MESSAGE(${test_name})
	add_executable(${test_name} "${test_name}.cpp" ${COMMON_SRC})
	target_link_libraries (${test_name} ${COMMON_LIBS})
	ADD_TEST(${test_name} ${test_name})
ENDFOREACH(test_name)
