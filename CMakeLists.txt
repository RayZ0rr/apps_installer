cmake_minimum_required(VERSION 3.12)
# set the project name
project(Apps_Installer VERSION 0.9.6)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


include_directories( "${PROJECT_SOURCE_DIR}/include" )

add_library(commands OBJECT "${PROJECT_SOURCE_DIR}/src/commands.cxx")
add_library(arguments OBJECT "${PROJECT_SOURCE_DIR}/src/arguments.cxx")
add_library(processtools OBJECT "${PROJECT_SOURCE_DIR}/src/processtools.cxx")
add_library(misctools OBJECT "${PROJECT_SOURCE_DIR}/src/misctools.cxx")
add_library(filetools OBJECT "${PROJECT_SOURCE_DIR}/src/filetools.cxx")
add_library(stringtools OBJECT "${PROJECT_SOURCE_DIR}/src/stringtools.cxx")

# add the source subdirectory
# add_subdirectory(src)

# add the executable
add_executable(apps_installer "${PROJECT_SOURCE_DIR}/src/main.cpp")

target_link_libraries(apps_installer PRIVATE
		      commands
		      processtools
		      misctools
		      filetools
		      stringtools
		      )

# target_include_directories(apps_installer PUBLIC
# 			  "${PROJECT_SOURCE_DIR}/include"
#                            )

install(TARGETS apps_installer DESTINATION "${PROJECT_SOURCE_DIR}/bin")

# Copy to source directory
add_custom_target(
    copy-compile-commands ALL
    DEPENDS
    ${CMAKE_SOURCE_DIR}/compile_commands.json
    )
add_custom_command(
    OUTPUT ${CMAKE_SOURCE_DIR}/compile_commands.json
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_BINARY_DIR}/compile_commands.json
        ${CMAKE_SOURCE_DIR}/compile_commands.json
    DEPENDS
        # Unlike "proper" targets like executables and libraries,
        # custom command / target pairs will not set up source
        # file dependencies, so we need to list file explicitly here
        generate-compile-commands
        ${CMAKE_BINARY_DIR}/compile_commands.json
    )

# Generate the compilation commands. Necessary so cmake knows where it came
# from and if for some reason you delete it.
add_custom_target(generate-compile-commands
    DEPENDS
        ${CMAKE_BINARY_DIR}/compile_commands.json
    )
add_custom_command(
    OUTPUT ${CMAKE_BINARY_DIR}/compile_commands.json
    COMMAND ${CMAKE_COMMAND} -B${CMAKE_BINARY_DIR} -S${CMAKE_SOURCE_DIR}
    )
