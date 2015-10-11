# Compiles the application, linking the core module (that is a static library)

set(MAIN_SOURCES main.cpp)

# Creating the final executable
add_executable(sequencerGUI ${MAIN_SOURCES})

# This code needs at least c++11 to compile (but c++14 would be better)
set_property(TARGET core PROPERTY CXX_STANDARD 11)
set_property(TARGET core PROPERTY CXX_STANDARD_REQUIRED ON)

# Adding dependencies
target_link_libraries(sequencerGUI core Qt5::Widgets)

# The executeble should be installed in the bin/ directory
install(TARGETS sequencerGUI DESTINATION bin)
