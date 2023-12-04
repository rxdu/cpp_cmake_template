cmake_minimum_required(VERSION 3.12.0)
project(my_project VERSION 0.1.0)

find_program(CCACHE_PROGRAM ccache)

if (CCACHE_PROGRAM)
    message(STATUS "Found ccache")
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_PROGRAM}")
endif ()

# # Project Options
option(BUILD_TESTING "Build tests" OFF)
option(STATIC_CHECK "Run static check" OFF)
option(ASAN_CHECK "Build with address sanitizer support" OFF)
option(ATHD_CHECK "Build with thread sanitizer support" OFF)

# # Generate symbols for IDE indexer
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# # Static check
if (STATIC_CHECK)
    find_program(CPPCHECK cppcheck)

    if (CPPCHECK)
        message(STATUS "Found cppcheck")
        set(CMAKE_CXX_CPPCHECK cppcheck;--std=c++11;--enable=all)
    endif ()
endif ()

# # Address sanitizer check
if (ASAN_CHECK)
    message(STATUS "Compile with -fsanitize=address")
    add_compile_options(-fsanitize=address)
    add_link_options(-fsanitize=address)
endif ()

if (ATHD_CHECK)
    message(STATUS "Compile with -fsanitize=thread")
    add_compile_options(-fsanitize=thread)
    add_link_options(-fsanitize=thread)
endif ()

# # Additional cmake module path
set(USER_CMAKE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
list(APPEND CMAKE_MODULE_PATH "${USER_CMAKE_PATH}/modules")
list(APPEND CMAKE_PREFIX_PATH "/usr/lib/${CMAKE_SYSTEM_PROCESSOR}-linux-gnu/cmake")

# # Add external dependencies
set(CONAN_CMAKE_SETUP ${CMAKE_BINARY_DIR}/conan_paths.cmake)

if (EXISTS ${CONAN_CMAKE_SETUP} AND NOT IS_DIRECTORY ${CONAN_CMAKE_SETUP})
    include(${CONAN_CMAKE_SETUP})
endif ()

# # Set compiler to use c++ 14 features
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# # Chosse build type
set(default_build_type "Release")

if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "Setting build type to '${default_build_type}' as none was specified.")
    set(CMAKE_BUILD_TYPE "${default_build_type}" CACHE
            STRING "Choose the type of build." FORCE)

    # Set the possible values of build type for cmake-gui
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
            "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif ()

# generate version header
configure_file(cmake/version.hpp.in ${PROJECT_BINARY_DIR}/version.hpp @ONLY)

# # Use GNUInstallDirs to install libraries into correct locations on all platforms.
include(GNUInstallDirs)

# # Put all binary files into /bin and libraries into /lib
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/${CMAKE_INSTALL_BINDIR})

# Build tests
if (PROJECT_NAME STREQUAL CMAKE_PROJECT_NAME AND BUILD_TESTING)
    enable_testing()
    include(GoogleTest)
    include(FetchContent)
    FetchContent_Declare(
            googletest
            GIT_REPOSITORY https://github.com/google/googletest.git
            GIT_TAG v1.14.0
    )
    set(BUILD_TESTS ON)
    message(STATUS "Tests will be built")
else ()
    set(BUILD_TESTS OFF)
    message(STATUS "Tests will not be built")
endif ()

# Build libraries
find_package(Threads REQUIRED)

add_library(my_project
        src/sample_class.cpp)
target_link_libraries(my_project PUBLIC Threads::Threads)
target_compile_options(my_project PUBLIC -fPIC)
target_include_directories(my_project PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}>
        $<INSTALL_INTERFACE:include>
        PRIVATE src)

if (BUILD_TESTS)
    add_subdirectory(test)
endif ()

# # Installation configuration
message(STATUS "Project will be installed to ${CMAKE_INSTALL_PREFIX} with 'make install'")

# Offer the user the choice of overriding the installation directories
set(INSTALL_LIBDIR ${CMAKE_INSTALL_LIBDIR} CACHE PATH "Installation directory for libraries")
set(INSTALL_BINDIR ${CMAKE_INSTALL_BINDIR} CACHE PATH "Installation directory for executables")
set(INSTALL_INCLUDEDIR ${CMAKE_INSTALL_INCLUDEDIR} CACHE PATH "Installation directory for header files")

if (WIN32 AND NOT CYGWIN)
    set(DEF_INSTALL_CMAKEDIR CMake)
else ()
    set(DEF_INSTALL_CMAKEDIR share/cmake/my_project)
endif ()

set(INSTALL_CMAKEDIR ${DEF_INSTALL_CMAKEDIR} CACHE PATH "Installation directory for CMake files")

# Report to user
foreach (p LIB BIN INCLUDE CMAKE)
    file(TO_NATIVE_PATH ${CMAKE_INSTALL_PREFIX}/${INSTALL_${p}DIR} _path)
    message(STATUS " - To install ${p} components to ${_path}")
    unset(_path)
endforeach ()

# Targets to install
install(TARGETS my_project
        EXPORT my_projectTargets
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
        INCLUDES DESTINATION include)

install(DIRECTORY include
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(FILES ${PROJECT_BINARY_DIR}/version.hpp
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/my_project)

# export target configuration
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
        my_projectConfigVersion.cmake
        VERSION "${${PROJECT_NAME}_VERSION_MAJOR}.${${PROJECT_NAME}_VERSION_MINOR}.${${PROJECT_NAME}_VERSION_PATCH}"
        COMPATIBILITY SameMajorVersion)

install(EXPORT my_projectTargets
        FILE my_projectTargets.cmake
        NAMESPACE rxdu::
        DESTINATION lib/cmake/my_project)

configure_file(cmake/my_projectConfig.cmake.in my_projectConfig.cmake @ONLY)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/my_projectConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/my_projectConfigVersion.cmake"
        DESTINATION lib/cmake/my_project)

# Packaging support
message(STATUS "Project will be installed to '/opt/my-project' with generated Debian package")

set(CPACK_DEBIAN_PACKAGE_NAME "my-project")
set(CPACK_PACKAGE_VENDOR "Ruixiang Du")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "My Package Description Summary")
set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
set(CPACK_PACKAGING_INSTALL_PREFIX "/opt/my-project")
set(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/README.md")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
# set(CPACK_DEBIAN_PACKAGE_DEPENDS "dep-lib (>= 0.9.0), dep-lib (<< 1.0.0)")
set(CPACK_GENERATOR "DEB")
set(CPACK_DEBIAN_FILE_NAME DEB-DEFAULT)
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Ruixiang Du (ruixiang.du@gmail.com)")
set(CPACK_SOURCE_IGNORE_FILES .git dist .*build.* /\\\\.DS_Store)
include(CPack)