SET(TESTS_FILES
	test_bounds
	test_node
	test_object
	test_position
	test_relation
	test_relation_member
	test_relation_member_list
	test_tag_list
	test_tag_ostream
	test_way
	test_way_node
	test_way_node_list
)

FOREACH(test_name ${TESTS_FILES})
MESSAGE(${test_name})
	add_executable(${test_name} "${test_name}.cpp" ${COMMON_SRC})
	target_link_libraries (${test_name} ${COMMON_LIBS})
	ADD_TEST(${test_name} ${test_name})
ENDFOREACH(test_name)
