#ifdef __aarch64__
#include "xbyak_aarch64.h"
#else
#endif

int main() {

#ifdef __aarch64__
	using namespace Xbyak_aarch64;
  constexpr size_t flops_per_instr = 8;
  constexpr size_t instrs_per_call = 400;

  CodeGenerator code;
  // zero registers
	for (auto i = 16; i < 32; ++i) {
		code.fmov(SReg(i), code.wzr);
	}
	// invoke FMAs
  for (auto i = 0; i < instrs_per_call; ++i) {
    auto vreg_off = (i * 3) % 13 + 16;
    code.fmla(
        VReg4S(vreg_off),
        VReg4S(vreg_off+1),
        VReg4S(vreg_off+2));
  }
	code.ret();
	code.setProtectModeRE();
#else
#endif

	void(*f)() = code.getCode<void(*)()>();

  auto warmup = 10000000;
  auto iters = 10000000;

	for (auto i = 0; i < warmup; ++i) {
		f();
	}
	auto start = std::chrono::steady_clock::now();
	for (auto i = 0; i < iters; ++i) {
		f();
	}
	auto end = std::chrono::steady_clock::now();
	std::chrono::duration<double> elapsed_seconds = end-start;
	double gflops = (double)flops_per_instr * instrs_per_call * iters / ((double)elapsed_seconds.count() * 1e9);
	std::cout << "achieved : " <<  gflops << " gflops on a single core\n";

}

