# Compile the tests and adds a test target

# One test executable per source file in core
add_executable(testutils testutils.cpp)
target_link_libraries(testutils core Qt5::Test)

add_executable(testsequencepoint testsequencepoint.cpp)
target_link_libraries(testsequencepoint core Qt5::Test)

# Adding all tests
add_test(NAME testutils COMMAND testutils)
add_test(NAME testsequencepoint COMMAND testsequencepoint)
