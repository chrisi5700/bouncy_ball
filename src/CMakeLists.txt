# Add library definitions here
# See README.md for CMake library patterns and examples
target_add_library(bouncy_ball)
target_sources(bouncy_ball PRIVATE BouncyBall.cpp)
target_include_directories(bouncy_ball PUBLIC ${CMAKE_SOURCE_DIR}/include)
