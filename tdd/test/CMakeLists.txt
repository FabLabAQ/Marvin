# Compile the tests and adds a test target

# Adding the subdirectory with test utilities
add_subdirectory(tutils)

# One test executable per source file in core
add_executable(testutils testutils.cpp)
target_link_libraries(testutils core tutils Qt5::Test)

add_executable(testsequencepoint testsequencepoint.cpp)
target_link_libraries(testsequencepoint core tutils Qt5::Test)

add_executable(testsequence testsequence.cpp)
target_link_libraries(testsequence core tutils Qt5::Test)

# Adding all tests
add_test(NAME testutils COMMAND testutils)
add_test(NAME testsequencepoint COMMAND testsequencepoint)
add_test(NAME testsequence COMMAND testsequence)
