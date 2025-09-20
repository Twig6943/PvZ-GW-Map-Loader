#pragma once

#include <Types.hpp>
#include <span>

namespace Memory {

void patch(uintptr_t address, std::span<const u8> bytes);

} // namespace Memory
