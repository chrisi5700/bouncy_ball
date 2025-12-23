#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <catch2/benchmark/catch_benchmark.hpp>
#include <BouncyBall.hpp>

using BounceFn = double(*)(double, double, int);

TEST_CASE("Verify problem statement examples", "[bounce][sanity]") {
    // n=1: just drop from h
    CHECK(bounce_recursive(10.0, 0.5, 1) == Catch::Approx(10.0));
    
    // n=2: h + 2hr
    CHECK(bounce_recursive(10.0, 0.5, 2) == Catch::Approx(10.0 + 2.0 * 10.0 * 0.5));
    
    // n=3: h + 2hr + 2hr²
    CHECK(bounce_recursive(10.0, 0.5, 3) == Catch::Approx(10.0 + 10.0 + 5.0));
}

TEST_CASE("All bounce implementations match sum_recursive", "[bounce]") {
    const auto fn = GENERATE(
        std::make_pair("sum_loop",         static_cast<BounceFn>(bounce_loop)),
        std::make_pair("sum_geometric",    static_cast<BounceFn>(bounce_geometric_loop)),
        std::make_pair("sum_o1",           static_cast<BounceFn>(bounce_o1_geometric)),
        std::make_pair("bounce_geometric", static_cast<BounceFn>(bounce_geometric)),
        std::make_pair("bounce_fast_exp",  static_cast<BounceFn>(bounce_fast_exp)),
        std::make_pair("bounce_fma",       static_cast<BounceFn>(bounce_fma)),
        std::make_pair("bounce_branchless",static_cast<BounceFn>(bounce_branchless)),
        std::make_pair("bounce_hybrid",    static_cast<BounceFn>(bounce_hybrid))
    );
    
    SECTION(fn.first) {
        SECTION("n = 1 (single drop)") {
            auto h = GENERATE(1.0, 5.0, 100.0, 0.001);
            auto r = GENERATE(0.1, 0.5, 0.9, 1.0, 2.0);
            CAPTURE(h, r);
            CHECK(fn.second(h, r, 1) == Catch::Approx(bounce_recursive(h, r, 1)).epsilon(1e-10));
        }
        
        SECTION("small n") {
            auto [h, r, n] = GENERATE(table<double, double, int>({
                {1.0, 0.5, 2},
                {1.0, 0.5, 3},
                {1.0, 0.5, 5},
                {10.0, 0.9, 4},
                {5.0, 0.75, 6},
                {100.0, 0.8, 10},
            }));
            CAPTURE(h, r, n);
            CHECK(fn.second(h, r, n) == Catch::Approx(bounce_recursive(h, r, n)).epsilon(1e-10));
        }
        
        SECTION("r = 1 (no energy loss)") {
            auto [h, n] = GENERATE(table<double, int>({
                {1.0, 2},
                {1.0, 5},
                {5.0, 10},
                {0.5, 20},
            }));
            CAPTURE(h, n);
            CHECK(fn.second(h, 1.0, n) == Catch::Approx(bounce_recursive(h, 1.0, n)).epsilon(1e-10));
        }
        
        SECTION("r > 1 (gaining energy - weird but valid)") {
            auto [h, r, n] = GENERATE(table<double, double, int>({
                {1.0, 1.1, 5},
                {1.0, 2.0, 4},
                {0.5, 1.5, 6},
            }));
            CAPTURE(h, r, n);
            CHECK(fn.second(h, r, n) == Catch::Approx(bounce_recursive(h, r, n)).epsilon(1e-9));
        }
        
        SECTION("large n") {
            auto [h, r, n] = GENERATE(table<double, double, int>({
                {1.0, 0.5, 50},
                {1.0, 0.9, 100},
                {1.0, 0.99, 200},
                {10.0, 0.95, 150},
            }));
            CAPTURE(h, r, n);
            CHECK(fn.second(h, r, n) == Catch::Approx(bounce_recursive(h, r, n)).epsilon(1e-8));
        }
        
        SECTION("extreme values") {
            auto [h, r, n] = GENERATE(table<double, double, int>({
                {0.001, 0.5, 10},
                {1e6, 0.5, 5},
                {1.0, 0.001, 10},
                {1.0, 0.9999, 50},
            }));
            CAPTURE(h, r, n);
            CHECK(fn.second(h, r, n) == Catch::Approx(bounce_recursive(h, r, n)).epsilon(1e-8));
        }
    }
}



TEST_CASE("Benchmark bounce implementations", "[bounce][.benchmark]") {
    const double h = 10.0;
    const double r = 0.85;
    
    BENCHMARK("sum_recursive n=20") { return bounce_recursive(h, r, 20); };
    BENCHMARK("sum_recursive n=100") { return bounce_recursive(h, r, 100); };
    

    
    BENCHMARK("bounce_geometric n=20") { return bounce_geometric(h, r, 20); };
    BENCHMARK("bounce_geometric n=100") { return bounce_geometric(h, r, 100); };
    
    BENCHMARK("bounce_fast_exp n=20") { return bounce_fast_exp(h, r, 20); };
    BENCHMARK("bounce_fast_exp n=100") { return bounce_fast_exp(h, r, 100); };
    
    BENCHMARK("bounce_fma n=20") { return bounce_fma(h, r, 20); };
    BENCHMARK("bounce_fma n=100") { return bounce_fma(h, r, 100); };
    
    BENCHMARK("bounce_branchless n=20") { return bounce_branchless(h, r, 20); };
    BENCHMARK("bounce_branchless n=100") { return bounce_branchless(h, r, 100); };
    
    BENCHMARK("bounce_hybrid n=4") { return bounce_hybrid(h, r, 4); };
    BENCHMARK("bounce_hybrid n=20") { return bounce_hybrid(h, r, 20); };
}