cmake_minimum_required(VERSION 3.12)
project(svh_video_service CXX)

# Adding userver dependency
find_package(userver COMPONENTS core postgresql QUIET)

if(NOT userver_FOUND) # Fallback to subdirectory usage
    # Enable userver libraries that are needed in this project
    set(USERVER_FEATURE_POSTGRESQL ON CACHE BOOL "" FORCE)

    # Compatibility mode: some systems don't support these features
    set(USERVER_FEATURE_CRYPTOPP_BLAKE2 OFF CACHE BOOL "" FORCE)
    set(USERVER_FEATURE_GRPC_CHANNELZ OFF CACHE BOOL "" FORCE)
    set(USERVER_FEATURE_REDIS_HI_MALLOC ON CACHE BOOL "" FORCE)

    if(EXISTS third_party/userver)
        message(STATUS "Using userver framework from third_party/userver")
        add_subdirectory(third_party/userver)
    else()
        message(FATAL_ERROR "Either install the userver or provide a path to it")
    endif()
endif()

userver_setup_environment()

# Common sources
add_library(${PROJECT_NAME}_objs OBJECT
    src/hello.hpp
    src/hello.cpp
)
target_link_libraries(${PROJECT_NAME}_objs PUBLIC userver::postgresql)

# Common components
add_library(${PROJECT_NAME}_comps_objs OBJECT

    # uploader
    src/components/http/post/uploader/VideoUploader.hpp
    src/components/http/post/uploader/VideoUploader.cpp
    src/components/http/post/uploader/fwd.hpp

    # options
    src/components/http/options/OptionsHandler.hpp
    src/components/http/options/OptionsHandler.cpp
    src/components/http/options/fwd.hpp
)
target_link_libraries(${PROJECT_NAME}_comps_objs PUBLIC userver::postgresql)

# The Service
add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_NAME}_objs)
target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_NAME}_comps_objs)

# Unit Tests
add_executable(${PROJECT_NAME}_unittest
    src/hello_test.cpp
)
target_link_libraries(${PROJECT_NAME}_unittest PRIVATE ${PROJECT_NAME}_objs userver::utest)
add_google_tests(${PROJECT_NAME}_unittest)

# Benchmarks
add_executable(${PROJECT_NAME}_benchmark
    src/hello_benchmark.cpp
)
target_link_libraries(${PROJECT_NAME}_benchmark PRIVATE ${PROJECT_NAME}_objs userver::ubench)
add_google_benchmark_tests(${PROJECT_NAME}_benchmark)

# Functional Tests
include(UserverTestsuite)

userver_testsuite_add_simple()
