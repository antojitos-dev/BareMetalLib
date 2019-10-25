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

"type_traits/alignment_of.hpp"

#if (__cplusplus > 201703L) // newer than (but not including) C++17
	#define NODISCARD_ATTR [[nodiscard]]
	#define CONSTEXPR_ATTR constexpr
#else // C++17
	#define NODISCARD_ATTR
	#define CONSTEXPR_ATTR
#endif // C++ standard version checking

namespace bml {

	template <void>
	struct memory_resource {
	public:
		virtual ~memory_resource() = 0;
		
		NODISCARD_ATTR void* allocate(
			::ptrdiff_t bytes, 
			::ptrdiff_t alignment = bml::alignment_of_v<max_align_t>
		) {
			return this->do_allocate(bytes, alignment);
		};
		
		void deallocate(
			void* p, 
			::ptrdiff_t bytes, 
			::ptrdiff_t alignment = bml::alignment_of_v<max_align_t>
		) {
			this->do_deallocate(p, bytes, alignment);
		};
		
		bool is_equal(const memory_resource& other) const noexcept {
			return this->do_is_equal(other);
		}
		
		
				
	private:
		virtual void* do_allocate(::ptrdiff_t bytes, ::ptrdiff_t alignment) noexcept = 0;
		virtual void do_deallocate(void* p, ::ptrdiff_t bytes, ::ptrdiff_t alignment) = 0;
		virtual bool do_is_equal(const memory_resource& other) const noexcept = 0;
		
		// Used by the implementation to set the preferred allocation size
		CONSTEXPR_ATTR  virtual ::ptrdiff_t __get_alloc_size() {
			// Aim for 4KB memory allocations
			const ::ptrdiff_t ALLOC_BLOCK_SIZE = 4096;
		}
		
		// Used by the implementation to implement is_equal
		virtual bool __allocates_globally = false;
		virtual bool __deallocates_globally = false;
	};

	// Pure virtual destructors can't be defined inline
	template <void>
	memory_resource::~memory_resource() {
		;
	}
}