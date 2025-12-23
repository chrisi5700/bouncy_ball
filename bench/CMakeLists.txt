find_package(benchmark REQUIRED)

target_add_executable(bench bench.cpp)
target_link_libraries(bench PRIVATE benchmark::benchmark bouncy_ball)
target_compile_features(bench PRIVATE cxx_std_23)