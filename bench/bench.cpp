#include <benchmark/benchmark.h>
#include <BouncyBall.hpp>

constexpr std::size_t MAX_N = 1<<12;

static void BM_recursive(benchmark::State& state) {
    int n = state.range(0);
    for (auto _ : state)
        benchmark::DoNotOptimize(bounce_recursive(10.0, 0.85, n));
    state.SetComplexityN(n);
}
BENCHMARK(BM_recursive)->RangeMultiplier(2)->Range(1, MAX_N)->Complexity();

static void BM_loop(benchmark::State& state) {
    int n = state.range(0);
    for (auto _ : state)
        benchmark::DoNotOptimize(bounce_loop(10.0, 0.85, n));
    state.SetComplexityN(n);
}
BENCHMARK(BM_loop)->RangeMultiplier(2)->Range(1, MAX_N)->Complexity();

static void BM_geometric_loop(benchmark::State& state) {
    int n = state.range(0);
    for (auto _ : state)
        benchmark::DoNotOptimize(bounce_geometric_loop(10.0, 0.85, n));
    state.SetComplexityN(n);
}
BENCHMARK(BM_geometric_loop)->RangeMultiplier(2)->Range(1, MAX_N)->Complexity();

static void BM_o1_geometric(benchmark::State& state) {
    int n = state.range(0);
    for (auto _ : state)
        benchmark::DoNotOptimize(bounce_o1_geometric(10.0, 0.85, n));
    state.SetComplexityN(n);
}
BENCHMARK(BM_o1_geometric)->RangeMultiplier(2)->Range(1, MAX_N)->Complexity();

static void BM_geometric(benchmark::State& state) {
    int n = state.range(0);
    for (auto _ : state)
        benchmark::DoNotOptimize(bounce_geometric(10.0, 0.85, n));
    state.SetComplexityN(n);
}
BENCHMARK(BM_geometric)->RangeMultiplier(2)->Range(1, MAX_N)->Complexity();

static void BM_fast_exp(benchmark::State& state) {
    int n = state.range(0);
    for (auto _ : state)
        benchmark::DoNotOptimize(bounce_fast_exp(10.0, 0.85, n));
    state.SetComplexityN(n);
}
BENCHMARK(BM_fast_exp)->RangeMultiplier(2)->Range(1, MAX_N)->Complexity();

static void BM_fma(benchmark::State& state) {
    int n = state.range(0);
    for (auto _ : state)
        benchmark::DoNotOptimize(bounce_fma(10.0, 0.85, n));
    state.SetComplexityN(n);
}
BENCHMARK(BM_fma)->RangeMultiplier(2)->Range(1, MAX_N)->Complexity();

static void BM_branchless(benchmark::State& state) {
    int n = state.range(0);
    for (auto _ : state)
        benchmark::DoNotOptimize(bounce_branchless(10.0, 0.85, n));
    state.SetComplexityN(n);
}
BENCHMARK(BM_branchless)->RangeMultiplier(2)->Range(1, MAX_N)->Complexity();

static void BM_hybrid(benchmark::State& state) {
    int n = state.range(0);
    for (auto _ : state)
        benchmark::DoNotOptimize(bounce_hybrid(10.0, 0.85, n));
    state.SetComplexityN(n);
}
BENCHMARK(BM_hybrid)->RangeMultiplier(2)->Range(1, MAX_N)->Complexity();

BENCHMARK_MAIN();