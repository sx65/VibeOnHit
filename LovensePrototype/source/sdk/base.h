#pragma once

#include <cstdint>

namespace rbx {
	class base {
	public:
		std::uint64_t address = 0;

		inline bool is_valid( void ) const noexcept {
			return address >= 0x10000 && address <= 0x7FFFFFFFFFFF;
		}

		inline std::uint64_t get_address( void ) const noexcept {
			return is_valid() ? address : 0;
		}

		inline explicit operator bool() const noexcept {
			return get_address() > 0;
		}

		inline bool operator!() const noexcept {
			return get_address() == 0;
		}

		inline bool operator==( const base& other ) const noexcept {
			return get_address() == other.get_address();
		}

		inline bool operator!=( const base& other ) const noexcept {
			return get_address() != other.get_address();
		}
	};
}