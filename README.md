# FMA (fused-multiply accumulate) Benchmark

Currently benchmarks pipelined FMAs on ARM and Intel chips.

To benchmark on a single thread,

```
git clone --recursive https://github.com/bwasti/mac_benchmark.git
cd mac_benchmark
make arm || make intel # only one will work
./bench
```

For multiple threads, rebuild the binary:

```
T=-DTHREADS=$(sysctl -a | grep machdep.cpu.core_count | awk '{print $2}')
make arm CFLAGS=T || make intel CFLAGS=T
./bench
```

Results collected so far:

Hardware | Chip | Single Core GFLOPs | Cores | All Cores GFLOPs
--- | --- | --- | --- | ---
2020 Macbook Air | M1 | 91 | 8 | 460
2019 16" Macbook Pro | 2.4 GHz 8-Core Intel Core i9 | 135 | 8 | -

## Adding More Results

Please submit a pull request to update the README
