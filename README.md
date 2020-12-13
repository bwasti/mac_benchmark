# Simple 1-core FMA Benchmark

Currently benchmarks pipelined FMAs on ARM and Intel chips.

To benchmark,

```
git clone --recursive https://github.com/bwasti/mac_benchmark.git
cd mac_benchmark
make arm || make intel # only one will work
./bench
```

Results collected so far:

Hardware | Chip | GFLOPs 
--- | --- | ---
2020 Macbook Air | M1 | 91
2019 16" Macbook Pro | 2.4 GHz 8-Core Intel Core i9 | 135

## Adding More Results

Please submit a pull request to update the README
