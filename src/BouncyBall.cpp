//
// Created by chris on 12/23/25.
//
#include <BouncyBall.hpp>
#include <cmath>

double bounce_recursive(double h, double r, int n)
{
	auto dist = h;
	if (n != 1)
	{
		dist += r * h;
		return dist + bounce_recursive(h * r, r, n - 1);
	}
	return h;
}
double bounce_recursive_tco(double h, double r, int n, double accumulator)
{
	auto dist = accumulator + h;
	if (n != 1)
	{
		dist += r * h;
		return bounce_recursive_tco(h * r, r, n - 1, dist);
	}
	return dist;
}
double bounce_loop(double h, double r, int n)
{
	double dist = h;
	double pow	= r;
	for (int i = 1; i < n; ++i)
	{
		dist += 2 * h * pow;
		pow *= r;
	}
	return dist;
}
double bounce_geometric_loop(double h, double r, int n)
{
	double sum = 0;
	double pow = r;
	for (int i = 1; i < n; ++i)
	{
		sum += pow;
		pow *= r;
	}
	return h + (sum * 2 * h);
}
double bounce_o1_geometric(double h, double r, int n)
{
	if (r == 1.0) return h + 2 * h * (n-1);
	auto pow = std::pow(r, n);
	auto geo = (pow - r) / (r - 1); // Aha! div by 0 potentially
	return h + geo * 2 * h;
}
double bounce_geometric(double h, double r, int n)
{
	if (r == 1.0) [[unlikely]]
		return h * (2 * n - 1);
	if (n == 1)
		return h;
	return h + (2.0 * h * r * (1.0 - std::pow(r, n - 1))) / (1.0 - r);
}
double bounce_fast_exp(double h, double r, int n)
{
	if (r == 1.0) [[unlikely]]
		return h * (2 * n - 1);
	if (n == 1)
		return h;

	double rn	= 1.0;
	double base = r;
	for (unsigned e = n - 1; e; e >>= 1, base *= base)
		if (e & 1)
			rn *= base;

	return h + (2.0 * h * r * (1.0 - rn)) / (1.0 - r);
}
double bounce_fma(double h, double r, int n)
{
	if (r == 1.0) [[unlikely]]
		return std::fma(2.0, h * n, -h); // h * (2n - 1)
	if (n == 1)
		return h;

	double rn	= 1.0;
	double base = r;
	for (unsigned e = n; e; e >>= 1, base *= base)
		if (e & 1)
			rn *= base;

	// h * (1 + r - 2r^n) / (1 - r)
	double numer = std::fma(-2.0, rn, 1.0 + r); // 1 + r - 2r^n
	return h * numer / (1.0 - r);
}
double bounce_branchless(double h, double r, int n) {
	double rn = 1.0;
	double base = r;
	for (unsigned e = n; e; e >>= 1, base *= base)
		if (e & 1) rn *= base;

	double numer = 1.0 + r - 2.0 * rn;
	double denom = 1.0 - r;

	// Branchless denominator fix: replace 0 with 1 to avoid NaN
	double is_valid = static_cast<double>(std::abs(denom) > 1e-15);
	double safe_denom = is_valid * denom + (1.0 - is_valid);  // denom or 1.0

	double geometric = h * numer / safe_denom;
	double linear = h * (2 * n - 1);

	return is_valid * geometric + (1.0 - is_valid) * linear;
}
double bounce_hybrid(double h, double r, int n)
{
	switch (n)
	{
		case 1: return h;
		case 2: return h + 2.0 * h * r;
		case 3: return h + 2.0 * h * r * (1.0 + r);
		case 4: return h + 2.0 * h * r * (1.0 + r + r * r);
		default:
		{
			if (r == 1.0) [[unlikely]]
				return h * (2 * n - 1);
			double rn = 1.0, base = r;
			for (unsigned e = n; e; e >>= 1, base *= base)
				if (e & 1)
					rn *= base;
			return h * (1.0 + r - 2.0 * rn) / (1.0 - r);
		}
	}
}

