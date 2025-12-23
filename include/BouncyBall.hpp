//
// Created by chris on 12/23/25.
//

#ifndef TEST_PROJECT_BOUNCEBALL_HPP
#define TEST_PROJECT_BOUNCEBALL_HPP

double bounce_recursive(double h, double r, int n);

double bounce_loop(double h, double r, int n);

double bounce_geometric_loop(double h, double r, int n);

double bounce_o1_geometric(double h, double r, int n);


// Closed-form with std::pow - O(1) but pow overhead
// f(n) = h + 2hr(1 - r^(n-1)) / (1 - r)
double bounce_geometric(double h, double r, int n);

// O(log n) binary exponentiation
double bounce_fast_exp(double h, double r, int n);

// FMA-optimized: restructure for fused multiply-add
// f(n) = h * (1 + 2r(1 - r^(n-1)) / (1 - r))
//      = h * (1 - r + 2r - 2r^n) / (1 - r)
//      = h * (1 + r - 2r^n) / (1 - r)
double bounce_fma(double h, double r, int n);

// Branchless using bit manipulation for r==1 case
double bounce_branchless(double h, double r, int n);

// Unrolled loop for small n, falls back to closed-form
// Good when n is often small
double bounce_hybrid(double h, double r, int n);

#endif // TEST_PROJECT_BOUNCEBALL_HPP
