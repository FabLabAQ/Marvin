# Compiles the static library making up the core of the application

set(CORE_HEADERS
	include/sequencepoint.h
	include/utils.h)
set(CORE_SOURCES
	src/sequencepoint.cpp)

# Creating the core library
add_library(core STATIC ${CORE_SOURCES} ${CORE_HEADERS})

# Specifying the the include directories: they are used both here and  exported
# by this library (so that targets linking this one will automatically import
# the include directories declared here)
target_include_directories(core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Adding dependencies (they are also exported, so targets linking this one will
# automatically link libraries declared here)
target_link_libraries(core Qt5::Core)
