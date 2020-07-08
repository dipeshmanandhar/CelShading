cmake_minimum_required(VERSION 3.0.0)
project(CelShading VERSION 0.1.0)

# include(CTest)
# enable_testing()

# library locations
set(GLFW_ROOT_DIR "lib/glfw-3.3.2" CACHE STRING "Path to the root directory of GLFW library.")
set(GLAD_ROOT_DIR "lib/glad" CACHE STRING "Path to the root directory of GLAD library.")
set(GLM_ROOT_DIR "lib/glm" CACHE STRING "Path to the root directory of GLM library.")
set(ASSIMP_ROOT_DIR "libassimp-5.0.1" CACHE STRING "Path to the root directory of Assimp library.")

set(GLFW_SUB_DIRS ${GLFW_ROOT_DIR} "${GLFW_ROOT_DIR}/build")
set(GLFW_INCLUDE_DIRS "${GLFW_ROOT_DIR}/include")

set(GLAD_SOURCE "${GLAD_ROOT_DIR}/src/glad.c")
set(GLAD_INCLUDE_DIRS "${GLAD_ROOT_DIR}/include")

set(GLM_INCLUDE_DIRS ${GLM_ROOT_DIR})

# set(ASSIMP_LIBRARY_DIRS "C:/Users/Dipesh/OpenGL_Libs/assimp-5.0.1/build/code")
set(ASSIMP_SUB_DIRS ${ASSIMP_ROOT_DIR} "${ASSIMP_ROOT_DIR}/build")
set(ASSIMP_INCLUDE_DIRS "${ASSIMP_ROOT_DIR}/include" "${ASSIMP_ROOT_DIR}/build/include")

# set(LIBRARIES ${ASSIMP_LIBRARY_DIRS})
set(INCLUDES ${GLFW_INCLUDE_DIRS} ${GLAD_INCLUDE_DIRS} ${GLM_INCLUDE_DIRS} ${ASSIMP_INCLUDE_DIRS})

file(GLOB_RECURSE SOURCES "Headers/*.h" "Source/*.cpp")

add_executable(CelShading ${SOURCES} ${GLAD_SOURCE})

# GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(${GLFW_SUB_DIRS})

target_link_libraries(CelShading glfw)

# Assimp
add_subdirectory(${ASSIMP_SUB_DIRS})

target_link_libraries(CelShading assimp)

# all include/libs
include_directories(${INCLUDES})
# link_directories(${LIBRARIES})

# CPack
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
