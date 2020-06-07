cmake_minimum_required(VERSION 3.14.0)
project(cmake_externalproject_example)
add_executable(cmake_externalproject_example src/main.cpp)

find_package(Git REQUIRED)
if(Git_FOUND)
  message("Git found: ${GIT_EXECUTABLE}")
endif()

#REMOVEME
set(VERSBOSE ON)

include(ExternalProject)
ExternalProject_Add(nifti
    GIT_REPOSITORY https://github.com/NIFTI-Imaging/nifti_clib.git
    GIT_TAG master
    PATCH_COMMAND "${GIT_EXECUTABLE}" apply "${CMAKE_SOURCE_DIR}/001_disable_zlib.patch"  --ignore-whitespace --whitespace=nowarn --verbose
    CMAKE_ARGS
        "-DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}"
        "-DNIFTI_BUILD_APPLICATIONS=OFF"
        "-DNIFTI_BUILD_TESTING=OFF"
    BUILD_COMMAND ${CMAKE_COMMAND} --build . --target nifti2 --config $<CONFIG>
    INSTALL_COMMAND ""
    TEST_COMMAND ""
    LOG_MERGED_STDOUTERR ON
    LOG_OUTPUT_ON_FAILURE ON
    # needed so patch does not apply twice (and fail)
    UPDATE_DISCONNECTED ON
)
target_link_libraries(cmake_externalproject_example PUBLIC nifti2)

# TODO this should be automatic from nifti2 target, and should also be automatic of build type
target_link_directories(cmake_externalproject_example PUBLIC "${CMAKE_CURRENT_BINARY_DIR}/nifti-prefix/src/nifti-build/nifti2/$<CONFIG>" "${CMAKE_CURRENT_BINARY_DIR}/nifti-prefix/src/nifti-build/znzlib/$<CONFIG>")
target_include_directories(cmake_externalproject_example PUBLIC "${CMAKE_CURRENT_BINARY_DIR}/nifti-prefix/src/nifti/nifti2" "${CMAKE_CURRENT_BINARY_DIR}/nifti-prefix/src/nifti/znzlib")
target_link_libraries(cmake_externalproject_example PUBLIC znz)

