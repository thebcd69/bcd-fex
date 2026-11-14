// SPDX-License-Identifier: MIT
#undef NDEBUG
#include <FEXCore/Core/CoreState.h>
#include <FEXCore/Utils/FPState.h>
#include <cassert>
#include <cstdint>
#include <random>

int main() {
  // madeira-bcd x87-exact: the integers and their mask are in the frame (CoreState.h), the registers in its CPU state.
  FEXCore::Core::CpuStateFrame frame {};
  auto& state = frame.State;
  state.FCW = 0x37f;
  state.AbridgedFTW = 1u << 5;
  state.mm[5][1] = 0x123456789abcdef0ULL;
  std::mt19937_64 random {19};
  for (unsigned i=0; i<10000; ++i) {
    uint64_t value = i==0 ? 0 : i==1 ? 0x8000000000000000ULL : i==2 ? 0x7fffffffffffffffULL : random();
    frame.x87_exact_int[5][0] = value;
    frame.x87_exact_valid = 1u << 5;
    uint64_t saved[2] {};
    FEXCore::FPState::ExportX87Register(frame, 5, saved, true);
    FEXCore::FPState::ImportX87Register(frame, 5, saved, true);
    assert(state.mm[5][1] == 0x123456789abcdef0ULL);
    if (value) assert((frame.x87_exact_valid & (1u << 5)) && frame.x87_exact_int[5][0] == value);
    else assert(!(frame.x87_exact_valid & (1u << 5)));
  }
  uint64_t fractional[2] {0xc000000000000000ULL, 0x3fff};
  FEXCore::FPState::ImportX87Register(frame, 5, fractional, true);
  assert(state.mm[5][0] == 0x3ff8000000000000ULL && !(frame.x87_exact_valid & (1u << 5)));
  uint64_t roundtrip[2] {};
  FEXCore::FPState::ExportX87Register(frame, 5, roundtrip, true);
  assert(roundtrip[0] == fractional[0] && roundtrip[1] == fractional[1]);
  uint64_t arbitrary[2] {0xabcdef, 0x123456};
  FEXCore::FPState::ImportX87Register(frame, 5, arbitrary, false);
  FEXCore::FPState::ExportX87Register(frame, 5, roundtrip, false);
  assert(roundtrip[0] == arbitrary[0] && roundtrip[1] == arbitrary[1]);
}
