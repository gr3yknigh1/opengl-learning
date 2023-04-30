cmake_minimum_required(VERSION 3.22)
project(opengl-sandbox CXX)
enable_language(CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED true)
set(CMAKE_EXPORT_COMPILE_COMMANDS true)

set(INLCUDE_DIR "${PROJECT_SOURCE_DIR}/include")
set(SOURCES_DIR "${PROJECT_SOURCE_DIR}/src")
set(VENDOR_DIR  "${PROJECT_SOURCE_DIR}/vendor")
set(ASSETS_DIR  "${PROJECT_SOURCE_DIR}/assets")

file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS "${SOURCES_DIR}/*.cpp")

add_definitions(-DGLEW_STATIC)
add_definitions(-DBUILD_SHARED_LIBS=OFF)
add_definitions(-DASSIMP_NO_EXPORT=ON)
add_definitions(-DASSIMP_BUILD_ZLIB=OFF)

add_subdirectory("${VENDOR_DIR}/glew"  EXCLUDE_FROM_ALL)
add_subdirectory("${VENDOR_DIR}/glfw"  EXCLUDE_FROM_ALL)
add_subdirectory("${VENDOR_DIR}/fmt"   EXCLUDE_FROM_ALL)
add_subdirectory("${VENDOR_DIR}/imgui" EXCLUDE_FROM_ALL)
add_subdirectory("${VENDOR_DIR}/assimp" EXCLUDE_FROM_ALL)

add_executable(${PROJECT_NAME} "${SOURCES_DIR}/main.cpp")

target_sources(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE ${INLCUDE_DIR})

target_link_libraries(${PROJECT_NAME} PRIVATE glfw)
target_link_libraries(${PROJECT_NAME} PRIVATE libglew_static)
target_link_libraries(${PROJECT_NAME} PRIVATE fmt::fmt)
target_link_libraries(${PROJECT_NAME} PRIVATE imgui imgui-glfw imgui-opengl3)
target_link_libraries(${PROJECT_NAME} PRIVATE assimp)

target_include_directories(
    ${PROJECT_NAME}
    PRIVATE "${VENDOR_DIR}/glm"
    PRIVATE "${VENDOR_DIR}/glew/include"
    PRIVATE "${VENDOR_DIR}/glfw/include"
    PRIVATE "${VENDOR_DIR}/fmt/include"
    PRIVATE "${VENDOR_DIR}/assimp/include"
    PRIVATE "${VENDOR_DIR}/stb"
    PRIVATE "${VENDOR_DIR}/imgui"
)

target_compile_definitions(${PROJECT_NAME} PUBLIC ASSETS_DIR="${ASSETS_DIR}")
