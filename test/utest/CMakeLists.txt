# Google tests
message(STATUS "Build unit tests with Google Test.")

# Simple tests
add_executable(simple_gtest sample_gtest_simple.cpp)
target_link_libraries(simple_gtest PRIVATE my_project)
target_link_libraries(simple_gtest PRIVATE gtest gtest_main)

gtest_discover_tests(simple_gtest)
add_test(NAME gtest_simple COMMAND simple_gtest)

# Fixture tests
add_executable(fixture_gtest sample_gtest_fixture.cpp)
target_link_libraries(fixture_gtest PRIVATE my_project)
target_link_libraries(fixture_gtest PRIVATE gtest gtest_main)

gtest_discover_tests(fixture_gtest)
add_test(NAME gtest_fixture COMMAND fixture_gtest)