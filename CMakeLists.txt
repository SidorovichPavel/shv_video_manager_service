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

find_package(jwt-cpp REQUIRED)

# Common sources
add_library(${PROJECT_NAME}_objs OBJECT
    src/hello.hpp
    src/hello.cpp
)
target_link_libraries(${PROJECT_NAME}_objs PUBLIC userver::postgresql)

# Common components
add_library(${PROJECT_NAME}_http_components OBJECT

    # video uploader
    src/components/http/post/upload/UploadHandler.hpp
    src/components/http/post/upload/UploadHandler.cpp
    src/components/http/post/upload/fwd.hpp

    # auload access
    src/components/http/get/upload/UploadAccessor.hpp
    src/components/http/get/upload/UploadAccessor.cpp
    src/components/http/get/upload/fwd.hpp

    # options
    src/components/http/options/OptionsHandler.hpp
    src/components/http/options/OptionsHandler.cpp
    src/components/http/options/fwd.hpp
)
target_link_libraries(${PROJECT_NAME}_http_components PUBLIC userver::postgresql)
target_include_directories(${PROJECT_NAME}_http_components PRIVATE src/logic/upload)
target_include_directories(${PROJECT_NAME}_http_components PRIVATE src/components/upload)

add_library(${PROJECT_NAME}_upload_component OBJECT

    # file conroller
    src/components/upload/UploadComponent.hpp
    src/components/upload/UploadComponent.cpp
    src/components/upload/fwd.hpp
)
target_link_libraries(${PROJECT_NAME}_upload_component PUBLIC userver::core)
target_include_directories(${PROJECT_NAME}_upload_component PRIVATE src/logic/upload)

add_library(${PROJECT_NAME}_convert_component OBJECT

    # dash converter component
    src/components/convert/DashConvertComponent.hpp
    src/components/convert/DashConvertComponent.cpp
    src/components/convert/fwd.hpp
)
target_link_libraries(${PROJECT_NAME}_convert_component PUBLIC userver::core)
target_include_directories(${PROJECT_NAME}_convert_component PRIVATE src/components/upload)

add_library(${PROJECT_NAME}_log_objs OBJECT

    # interface
    src/logic/upload/controller/UploadController.hpp
    src/logic/upload/controller/UploadController.cpp

    # impl
    src/logic/upload/controller/impl/UploadControllerImpl.hpp
    src/logic/upload/controller/impl/UploadControllerImpl.cpp
    src/logic/upload/controller/impl/ExpirationFileBuilder.hpp
    src/logic/upload/controller/impl/ExpirationFileBuilder.cpp
    src/logic/upload/controller/impl/builder/FileBuilder.hpp
    src/logic/upload/controller/impl/builder/FileBuilder.cpp
    src/logic/upload/controller/impl/expiration/ExpirationController.hpp
    src/logic/upload/controller/impl/expiration/ExpirationController.cpp
)
target_link_libraries(${PROJECT_NAME}_log_objs PUBLIC userver::core)
target_include_directories(${PROJECT_NAME}_convert_component PRIVATE src/logic/upload)

add_library(${PROJECT_NAME}_convert_control OBJECT

    # ConvertController
    src/logic/convert/controller/ConvertController.hpp
    src/logic/convert/controller/ConvertController.cpp

    # ConvertController impl
    src/logic/convert/controller/impl/ConvertControllerImpl.hpp
    src/logic/convert/controller/impl/ConvertControllerImpl.cpp
    src/logic/convert/controller/impl/ConvertConfig.hpp
    src/logic/convert/controller/impl/ConvertConfig.cpp
)
target_link_libraries(${PROJECT_NAME}_convert_control PUBLIC userver::core)
target_include_directories(${PROJECT_NAME}_convert_component PRIVATE src/logic/convert)

# The Service
add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME} PUBLIC jwt-cpp::jwt-cpp)
target_link_libraries(${PROJECT_NAME} PUBLIC ${PROJECT_NAME}_objs)
target_link_libraries(${PROJECT_NAME} PUBLIC ${PROJECT_NAME}_http_components)
target_link_libraries(${PROJECT_NAME} PUBLIC ${PROJECT_NAME}_upload_component)
target_link_libraries(${PROJECT_NAME} PUBLIC ${PROJECT_NAME}_log_objs)
target_link_libraries(${PROJECT_NAME} PUBLIC ${PROJECT_NAME}_convert_component)
target_link_libraries(${PROJECT_NAME} PUBLIC ${PROJECT_NAME}_convert_control)

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

# Install
include(GNUInstallDirs)

if(DEFINED ENV{PREFIX})
    message(STATUS "Set install prefix: $ENV{PREFIX}")
    file(TO_CMAKE_PATH "$ENV{PREFIX}" PREFIX_PATH)
    set(CMAKE_INSTALL_PREFIX ${PREFIX_PATH})
endif()

file(GLOB CONFIGS_FILES ${CMAKE_CURRENT_SOURCE_DIR}/configs/*.yaml ${CMAKE_CURRENT_SOURCE_DIR}/configs/*.json)

install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT ${PROJECT_NAME})
install(FILES ${CONFIGS_FILES} DESTINATION ${CMAKE_INSTALL_SYSCONFDIR}/${PROJECT_NAME} COMPONENT ${PROJECT_NAME})
