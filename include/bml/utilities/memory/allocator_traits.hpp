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
#include "new.hpp"
#include "forward.hpp"

#if (__cplusplus > 201703L) // newer than (but not including) C++17
	#define NODISCARD_ATTR [[nodiscard]]
#else
	#define NODISCARD_ATTR
#endif

namespace bml {
	
	template <class Alloc>
	struct allocator_traits {
	public:
		typedef Alloc allocator_type;
		typedef Alloc::value_type value_type;
		typedef bml::add_pointer<Alloc::value_type>::type pointer;
		typedef bml::add_const<pointer>::type const_pointer;
		typedef void* void_pointer;
		typedef const void* const_void_pointer;
		typedef Alloc::difference_type difference_type;
		typedef Alloc::size_type size_type;
		typedef bml::false_type propagate_on_container_copy_assignment;
		typedef bml::false_type propagate_on_container_move_assignment;
		typedef bml::false_type propagate_on_container_swap;
		typedef Alloc::is_always_equal is_always_equal;

		NODISCARD_ATTR static pointer allocate(Alloc& a, size_type n) {
			a.allocate(n);
		}
		
		NODISCARD_ATTR static pointer allocate(
			Alloc& a, 
			size_type n, 
			const_void_pointer hint
		) {
			a.allocate(n);
		}
		
		static void deallocate(Alloc& a, pointer p, size_type n) {
			a.deallocate(p, n);
		}
		
		template <class T, class... Args>
		static void construct(Alloc& a, T* p, Args&&... args) {
			::new (static_cast<void*>(p)) T(bml::forward<Args>(args)...);
		}
		
		template <class T>
		static void destroy(Alloc& a, T* p) {
			p->~T();
		}
		
		static size_type max_size(const Alloc& a) noexcept {
			return ~((size_type)0);
		}
	
		static Alloc select_in_container_copy_construction(const Alloc& a) {
			return a;
		}
	
	};
	
}

// Prevent leaking internal defines
#undef NODISCARD_ATTR
