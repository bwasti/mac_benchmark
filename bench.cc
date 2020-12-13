#ifdef __aarch64__
#include "xbyak_aarch64.h"
#else
#include "xbyak.h"
#endif

int main() {
#ifdef __aarch64__
  using namespace Xbyak_aarch64;
  constexpr size_t flops_per_instr = 8;
  constexpr size_t independent_calls = (32 / 2);
  constexpr size_t block_calls = 100;
  constexpr size_t instrs_per_fn = independent_calls * block_calls;

  CodeGenerator code(instrs_per_fn * 16);
  // save callee-preserved registers
  code.sub(code.sp, code.sp, 16 * 16);
  for (auto i = 0; i < 16; ++i) {
    code.str(QReg(i), ptr(code.sp, i * 16));
  }
  // zero registers
  for (auto i = 0; i < 32; ++i) {
    code.eor(VReg16B(i), VReg16B(i), VReg16B(i));
  }
  // invoke FMAs
  for (auto i = 0; i < block_calls; ++i) {
    for (auto reg = 0; reg < independent_calls; ++reg) {
      code.fmla(VReg4S(reg), VReg4S(reg + 1), VReg4S(reg + 1));
    }
  }
  for (auto i = 0; i < 16; ++i) {
    code.ldr(QReg(i), ptr(code.sp, i * 16));
  }
  code.add(code.sp, code.sp, 16 * 16);
  code.ret();
  code.setProtectModeRE();
#else
  using namespace Xbyak;
  constexpr size_t flops_per_instr = 16;
  constexpr size_t independent_calls = (16 / 2);
  constexpr size_t block_calls = 100;
  constexpr size_t instrs_per_fn = independent_calls * block_calls;

  CodeGenerator code;
  // zero 16 registers
  for (auto i = 0; i < 16; ++i) {
    code.vxorps(Xmm(i), Xmm(i), Xmm(i));
  }
  // invoke FMAs
  for (auto i = 0; i < block_calls; ++i) {
    for (auto reg = 0; reg < independent_calls; ++reg) {
      code.vfmadd132ps(Ymm(reg), Ymm(reg + 1), Ymm(reg + 1));
    }
  }
  code.ret();
  code.setProtectModeRE();
#endif

  void (*f)() = code.getCode<void (*)()>();

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
  std::chrono::duration<double> elapsed_seconds = end - start;
  double gflops = (double)flops_per_instr * instrs_per_fn * iters /
                  ((double)elapsed_seconds.count() * 1e9);
  std::cout << "achieved : " << gflops << " gflops on a single core\n";
}
