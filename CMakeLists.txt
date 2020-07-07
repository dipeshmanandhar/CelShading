cmake_minimum_required(VERSION 3.0.0)
project(CelShading VERSION 0.1.0)

# include(CTest)
# enable_testing()

# library locations
set(GLAD_SOURCE "C:/Users/Dipesh/OpenGL_Libs/glad/src/glad.c")
set(GLAD_INCLUDE_DIRS "C:/Users/Dipesh/OpenGL_Libs/glad/include")

set(GLM_INCLUDE_DIRS "C:/Users/Dipesh/OpenGL_Libs/glm")

set(ASSIMP_LIBRARY_DIRS "C:/Users/Dipesh/OpenGL_Libs/assimp-5.0.1/build/code")
set(ASSIMP_INCLUDE_DIRS "C:/Users/Dipesh/OpenGL_Libs/assimp-5.0.1/include" "C:/Users/Dipesh/OpenGL_Libs/assimp-5.0.1/build/include")

set(LIBRARIES ${ASSIMP_LIBRARY_DIRS})
set(INCLUDES ${GLAD_INCLUDE_DIRS} ${GLM_INCLUDE_DIRS} ${ASSIMP_INCLUDE_DIRS})

file(GLOB_RECURSE SOURCES "Headers/*.h" "Source/*.cpp")

add_executable(CelShading ${SOURCES} ${GLAD_SOURCE})

# GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(
    C:/Users/Dipesh/OpenGL_Libs/glfw-3.3.2
    C:/Users/Dipesh/OpenGL_Libs/glfw-3.3.2/build
    )

target_link_libraries(CelShading glfw)

include_directories(${INCLUDES})
link_directories(${LIBRARIES})

# CPack
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
