#pragma once

#include "../Common.hpp"

namespace Froggo {

class Bus {
private:
public:
  void Write8(u32 address, u8 value);
  void Write16(u32 address, u16 value);
  void Write32(u32 address, u32 value);

  u8 Read8(u32 address);
  u8 Read16(u32 address);
  u8 Read32(u32 address);

  void Write(u32 address, std::size_t size, u32 value);
  u32 Read(u32 address, std::size_t size);
};
} // namespace Froggo