import json
import matplotlib.pyplot as plt
from collections import defaultdict

def load_benchmarks(filepath: str) -> dict[str, list[tuple[int, float]]]:
    with open(filepath) as f:
        data = json.load(f)

    results = defaultdict(list)

    for bench in data["benchmarks"]:
        # Skip aggregate results (BigO, RMS)
        if bench["run_type"] != "iteration":
            continue

        name = bench["name"]
        # Parse "BM_recursive/512" -> ("recursive", 512)
        parts = name.split("/")
        if len(parts) != 2:
            continue

        func_name = parts[0].removeprefix("BM_")
        n = int(parts[1])
        cpu_time = bench["cpu_time"]

        results[func_name].append((n, cpu_time))

    # Sort by n
    for func in results:
        results[func].sort(key=lambda x: x[0])

    return results

def plot_benchmarks(results: dict[str, list[tuple[int, float]]]):
    plt.figure(figsize=(12, 8))

    for func_name, data in results.items():
        ns = [d[0] for d in data]
        times = [d[1] for d in data]
        plt.plot(ns, times, marker='o', label=func_name, linewidth=2, markersize=4)

    plt.xlabel("n", fontsize=12)
    plt.ylabel("Time (ns)", fontsize=12)
    plt.title("Bounce Ball Implementations Benchmark", fontsize=14)
    plt.legend(loc="upper left")
    plt.grid(True, alpha=0.3)
    plt.xscale("log", base=2)
    plt.yscale("log")

    plt.tight_layout()
    plt.savefig("benchmark_results.png", dpi=150)
    plt.show()

if __name__ == "__main__":
    import sys
    filepath = sys.argv[1] if len(sys.argv) > 1 else "../build/release-vcpkg/bench/results.json"
    results = load_benchmarks(filepath)
    plot_benchmarks(results)