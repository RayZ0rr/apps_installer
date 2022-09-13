cmake_minimum_required(VERSION 3.12)
# set the project name
project(Apps_Installer VERSION 0.9.6)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories( "${PROJECT_SOURCE_DIR}/../include" )

# Static executable
# ----------------------------------------------------------
add_library(AppsInstaller STATIC
		      "${PROJECT_SOURCE_DIR}/../src/commands.cxx"
		      "${PROJECT_SOURCE_DIR}/../src/processtools.cxx"
		      "${PROJECT_SOURCE_DIR}/../src/misctools.cxx"
		      "${PROJECT_SOURCE_DIR}/../src/filetools.cxx"
		      "${PROJECT_SOURCE_DIR}/../src/stringtools.cxx"
		      )

# add the executable
add_executable(apps_installer_static "${PROJECT_SOURCE_DIR}/../src/main.cpp")

target_link_libraries(apps_installer_static PRIVATE
		      AppsInstaller
		      )

install(TARGETS AppsInstaller COMPONENT static DESTINATION "${PROJECT_SOURCE_DIR}/../lib")
install(TARGETS apps_installer_static COMPONENT static DESTINATION "${PROJECT_SOURCE_DIR}/../bin")
