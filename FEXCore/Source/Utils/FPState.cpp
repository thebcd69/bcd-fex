// SPDX-License-Identifier: MIT
#include <FEXCore/Utils/FPState.h>
#include <FEXCore/Core/CoreState.h>
#include <FEXCore/Utils/LogManager.h>
#include <bit>
#include <cstdint>
#include <cstdlib>
#include <cstring>
extern "C" {
#include "SoftFloat-3e/platform.h"
#include "SoftFloat-3e/softfloat.h"
}

namespace FEXCore::FPState {
static softfloat_state StateFromControlWord(uint16_t FCW) {
  softfloat_state State {};
  State.detectTininess = softfloat_tininess_afterRounding;
  State.roundingPrecision = 80;
  constexpr uint8_t Modes[] = {softfloat_round_near_even, softfloat_round_min, softfloat_round_max, softfloat_round_minMag};
  State.roundingMode = Modes[(FCW >> 10) & 3];
  return State;
}

// madeira-bcd x87-exact: see FPState.h. Exactly "1" turns it on; one line says so.
bool X87ExactInt64() {
  static const bool On = [] {
    const char* Env = getenv("MADEIRA_FEX_X87_EXACT_INT64");
    const bool Value = Env && Env[0] == '1' && Env[1] == '\0';
    if (Value) {
      LogMan::Msg::IFmt("[fex-x87-exact] MADEIRA_FEX_X87_EXACT_INT64=1: reduced-precision x87 keeps the 64-bit integer of FILD m64 for FISTP m64");
    }
    return Value;
  }();
  return On;
}

void ExportX87Register(const FEXCore::Core::CpuStateFrame& Frame, unsigned Slot, uint64_t (&Out)[2], bool Reduced) {
  const auto& State = Frame.State;
  if (!Reduced) {
    memcpy(Out, State.mm[Slot], sizeof(Out));
    return;
  }
  extFloat80_t Value {};
  if ((Frame.x87_exact_valid & State.AbridgedFTW) & (1u << Slot)) {
    const uint64_t Bits = Frame.x87_exact_int[Slot][0];
    const bool Negative = (Bits >> 63) != 0;
    Value = ui64_to_extF80(Negative ? 0 - Bits : Bits);
    if (Negative) Value.signExp |= 0x8000;
  } else {
    auto FloatState = StateFromControlWord(State.FCW);
    Value = f64_to_extF80(&FloatState, std::bit_cast<float64_t>(State.mm[Slot][0]));
  }
  Out[0] = Value.signif;
  Out[1] = Value.signExp;
}

void ImportX87Register(FEXCore::Core::CpuStateFrame& Frame, unsigned Slot, const uint64_t (&In)[2], bool Reduced) {
  auto& State = Frame.State;
  Frame.x87_exact_valid &= ~(1u << Slot);
  if (!Reduced) {
    memcpy(State.mm[Slot], In, sizeof(In));
    return;
  }
  extFloat80_t Value {};
  Value.signif = In[0];
  Value.signExp = static_cast<uint16_t>(In[1]);
  auto FloatState = StateFromControlWord(State.FCW);
  State.mm[Slot][0] = std::bit_cast<uint64_t>(extF80_to_f64(&FloatState, Value));

  const uint64_t Exponent = Value.signExp & 0x7fff;
  const bool Negative = Value.signExp & 0x8000;
  if (Exponent < 0x3fff || Exponent > 0x403e || !(Value.signif >> 63)) return;
  const unsigned Shift = 0x403e - Exponent;
  const uint64_t Magnitude = Value.signif >> Shift;
  if ((Magnitude << Shift) != Value.signif ||
      (Exponent == 0x403e && (!Negative || Value.signif != (1ULL << 63)))) return;
  Frame.x87_exact_int[Slot][0] = Negative ? 0 - Magnitude : Magnitude;
  Frame.x87_exact_valid |= 1u << Slot;
}
} // namespace FEXCore::FPState
