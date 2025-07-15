add_executable(sample_test test_sample.cpp)
target_link_libraries(sample_test my_project)
add_test(NAME sample_test COMMAND sample_test)

add_subdirectory(unit_test)