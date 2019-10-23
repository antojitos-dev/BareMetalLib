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

#if (__cplusplus > 201703L) // newer than (but not including) C++17
	#define NODISCARD_ATTR [[nodiscard]]
	#define CONSTEXPR_ATTR constexpr
#else // C++17 is okay
	#define NODISCARD_ATTR
	#define CONSTEXPR_ATTR
#endif // C++ standard version checking

namespace bml {

	template <typename T>
	struct static_allocator {
	private:
		static ::size_t max_size = 0;
	public:
		typedef bml::true_type is_always_equal;
		typedef T value_type;
		typedef ::size_t size_type;
		typedef ::ptrdiff_t difference_type;
		
		CONSTEXPR_ATTR static_allocator() noexcept {};
		CONSTEXPR_ATTR static_allocator(const static_allocator& other) noexcept {};
		
		template <typename U>
		CONSTEXPR_ATTR static_allocator(const static_allocator<U>& other) noexcept {};
		
		static void resize(::size_t s) {
			max_size = s;
		}
		
		NODISCARD_ATTR static CONSTEXPR_ATTR bml::add_pointer<T>::type allocate(size_type n) {
			return NULL;
		}
		
		static CONSTEXPR_ATTR void deallocate(bml::add_pointer<T>::type p, ::size_t n) {};
		
	};
}


#undef NODISCARD_ATTR
#undef CONSTEXPR_ATTR