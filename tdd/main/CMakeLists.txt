# Compiles the application, linking the core module (that is a static library)

set(SOURCES main.cpp)

# Creating the final executable
add_executable(sequencerGUI ${SOURCES})

# Adding dependencies
#target_link_libraries(sequencerGUI core)
target_link_libraries(sequencerGUI Qt5::Core)
