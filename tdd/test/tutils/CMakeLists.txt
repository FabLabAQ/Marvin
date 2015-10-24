# Compile the test utilities library

set(TUTILS_HEADERS
	include/tutils.h)
set(TUTILS_SOURCES
	src/tutils.cpp)

# Creating the core library
add_library(tutils STATIC ${TUTILS_SOURCES} ${TUTILS_HEADERS})

# Specifying the the include directories: they are used both here and  exported
# by this library (so that targets linking this one will automatically import
# the include directories declared here)
target_include_directories(tutils PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Adding dependencies (they are also exported, so targets linking this one will
# automatically link libraries declared here)
target_link_libraries(tutils core)

