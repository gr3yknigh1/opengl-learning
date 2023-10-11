cmake_minimum_required(VERSION 3.22)
project(BadSandbox CXX)
enable_language(CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED true)
set(CMAKE_EXPORT_COMPILE_COMMANDS true)

set(SOURCES_DIR "${PROJECT_SOURCE_DIR}/src")
set(VENDOR_DIR  "${PROJECT_SOURCE_DIR}/vendor")
set(ASSETS_DIR  "${PROJECT_SOURCE_DIR}/assets")

set(
    PROJECT_SOURCES
    ${SOURCES_DIR}
    ${SOURCES_DIR}/BSandbox/Main.cpp
    ${SOURCES_DIR}/BEngine/Core/Transform3D.cpp
    ${SOURCES_DIR}/BEngine/Core/Transform3D.hpp
    ${SOURCES_DIR}/BEngine/Core/Application.cpp
    ${SOURCES_DIR}/BEngine/Core/Event.hpp
    ${SOURCES_DIR}/BEngine/Core/Memory.hpp
    ${SOURCES_DIR}/BEngine/Core/FrameTimer.cpp
    ${SOURCES_DIR}/BEngine/Core/FrameTimer.hpp
    ${SOURCES_DIR}/BEngine/Core/Application.hpp
    ${SOURCES_DIR}/BEngine/Render/GL/GLObject.cpp
    ${SOURCES_DIR}/BEngine/Render/GL/GLUtils.cpp
    ${SOURCES_DIR}/BEngine/Render/GL/IndexBuffer.cpp
    ${SOURCES_DIR}/BEngine/Render/GL/Shader.cpp
    ${SOURCES_DIR}/BEngine/Render/GL/Texture.cpp
    ${SOURCES_DIR}/BEngine/Render/GL/VertexArray.cpp
    ${SOURCES_DIR}/BEngine/Render/GL/VertexBuffer.cpp
    ${SOURCES_DIR}/BEngine/Render/GL/VertexBufferLayout.cpp
    ${SOURCES_DIR}/BEngine/Render/GL/Material.hpp
    ${SOURCES_DIR}/BEngine/Render/GL/Vertex.hpp
    ${SOURCES_DIR}/BEngine/Render/GL/VertexBufferLayout.hpp
    ${SOURCES_DIR}/BEngine/Render/GL/VertexArray.hpp
    ${SOURCES_DIR}/BEngine/Render/GL/GLObject.hpp
    ${SOURCES_DIR}/BEngine/Render/GL/Texture.hpp
    ${SOURCES_DIR}/BEngine/Render/GL/GLUtils.hpp
    ${SOURCES_DIR}/BEngine/Render/GL/Mesh.hpp
    ${SOURCES_DIR}/BEngine/Render/GL/VertexBufferAttribute.hpp
    ${SOURCES_DIR}/BEngine/Render/GL/Shader.hpp
    ${SOURCES_DIR}/BEngine/Render/GL/VertexBuffer.hpp
    ${SOURCES_DIR}/BEngine/Render/GL/IndexBuffer.hpp
    ${SOURCES_DIR}/BEngine/Render/Model/Mesh.cpp
    ${SOURCES_DIR}/BEngine/Render/Model/Model.cpp
    ${SOURCES_DIR}/BEngine/Render/Model/Model.hpp
    ${SOURCES_DIR}/BEngine/Render/Camera3D.cpp
    ${SOURCES_DIR}/BEngine/Render/Camera3D.hpp
    ${SOURCES_DIR}/BEngine/Render/Renderer.cpp
    ${SOURCES_DIR}/BEngine/Render/Renderer.hpp
    ${SOURCES_DIR}/BEngine/BadEngine.hpp
    )

add_definitions(-DGLEW_STATIC)
set(BUILD_SHARED_LIBS OFF)
set(ASSIMP_NO_EXPORT ON)
set(ASSIMP_BUILD_ZLIB OFF)
set(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT OFF)
set(ASSIMP_BUILD_ALL_EXPORTERS_BY_DEFAULT OFF)
set(ASSIMP_BUILD_TESTS OFF)
set(ASSIMP_INSTALL OFF)
set(ASSIMP_INSTALL_PDB OFF)

set(ASSIMP_BUILD_OBJ_IMPORTER ON)

add_subdirectory("${VENDOR_DIR}/glew"  EXCLUDE_FROM_ALL)
add_subdirectory("${VENDOR_DIR}/glfw"  EXCLUDE_FROM_ALL)
add_subdirectory("${VENDOR_DIR}/fmt"   EXCLUDE_FROM_ALL)
add_subdirectory("${VENDOR_DIR}/imgui" EXCLUDE_FROM_ALL)
add_subdirectory("${VENDOR_DIR}/assimp" EXCLUDE_FROM_ALL)

add_executable(${PROJECT_NAME})

target_sources(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE ${SOURCES_DIR})

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
