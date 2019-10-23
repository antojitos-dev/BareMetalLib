//
// Copyright (c) 2019 Colin Vanden Heuvel
//
// BareMetalLib is based on libc++ (https://libcxx.llvm.org/).
// 
// This file is licensed under under the Apache License v2.0 with LLVM Exceptions. For more details,
// see the LICENSE.md file in the top-level directory of this distribution, or copy at 
// https://llvm.org/LICENSE.txt.
//
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//

#pragma once
#include "type_traits.hpp"


#if (__cplusplus > 201703L) // newer than (but not including) C++17
	#define NODISCARD_ATTR [[nodiscard]]
	#define CONSTEXPR_ATTR constexpr
#else // C++17
	#define NODISCARD_ATTR
	#define CONSTEXPR_ATTR
#endif // C++ standard version checking

namespace bml {
	
// Constant allocator (returns a pointer to an address known at compile time)
	template <typename T, void* Addr>
	struct const_allocator {
	public:
		typedef bml::true_type is_always_equal;
		typedef T value_type;
		typedef ::size_t size_type;
		typedef ::ptrdiff_t difference_type;
		
		CONSTEXPR_ATTR const_allocator() noexcept {};
		CONSTEXPR_ATTR const_allocator(const const_allocator& other) noexcept {};
		
		template <typename U>
		CONSTEXPR_ATTR const_allocator(const const_allocator<U, Addr>& other) noexcept {};
		
		NODISCARD_ATTR CONSTEXPR_ATTR bml::add_pointer<T>::type allocate(size_type n) {
			return Addr;
		}
		
		CONSTEXPR_ATTR void deallocate(bml::add_pointer<T>::type p, ::size_t n) {};
	};

// Public Non-member Operators
	template <typename T1, typename T2>
	CONSTEXPR_ATTR bool operator==(
		const constant_allocator<T1>& lhs,
		const const_allocator<T2>& rhs
	) noexcept {
		return true;
	}

#if (__cplusplus == 201703L) // standard is exactly C++17
	template <typename T1, typename T2>
		bool operator!=(
			const const_allocator<T1>& lhs,
			const const_allocator<T2>& rhs
		) noexcept {
			return false;
		}
#endif // C++ standard version checking

#undef NODISCARD_ATTR
#undef CONSTEXPR_ATTR
	
}