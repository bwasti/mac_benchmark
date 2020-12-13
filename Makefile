arm:
	g++ bench.cc -o bench -Ixbyak_aarch64/xbyak_aarch64 -std=c++17 xbyak_aarch64/src/xbyak_aarch64_impl.cpp 
intel:
	g++ bench.cc -o bench -Ixbyak/xbyak -std=c++17

