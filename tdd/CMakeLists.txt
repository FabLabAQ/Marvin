# The main CMake script. This basically only sets up things common to all
# subprojects and includes scripts in subdirectories

# The minimum required version of CMake
cmake_minimum_required(VERSION 3.0)

# The name of the project
project(SequencerGUI)

# Adding the current directory to the include path
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

# Adding QT. We do it here because it is needed everywhere
find_package(Qt5Qml)
find_package(Qt5Quick)
find_package(Qt5Widgets)
find_package(Qt5SerialPort)
find_package(Qt5Test)
find_package(Qt5QuickTest)

# Adding all subdirectories
add_subdirectory(test)
add_subdirectory(core)
add_subdirectory(main)
