#include <benchmark/benchmark.h>
#include "io_png.hh"
#include "filter.cuh"

constexpr int niteration = 20;

void BM_Rendering_gpu(benchmark::State& st)
{
    int width;
    int height;
    const auto image = read_png("../../resources/forest.png", &width, &height);
    for (auto _ : st)
        oil_filter(image, width, height);

    st.counters["frame_rate"] = benchmark::Counter(st.iterations(), benchmark::Counter::kIsRate);
}

BENCHMARK(BM_Rendering_gpu)
->Unit(benchmark::kMillisecond)
->UseRealTime()
->Iterations(niteration);

BENCHMARK_MAIN();
