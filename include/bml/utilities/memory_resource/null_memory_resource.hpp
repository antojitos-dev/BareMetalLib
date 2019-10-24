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

#include "memory_resource_base.hpp"

namespace bml {
	
	// Memory resource which always fails to perform allocation
	template <void>
	struct null_memory_resouce : public memory_resource {
	private:
		void* do_allocate(::ptrdiff_t bytes, ::ptrdiff_t alignment) noexcept {
			return NULL;
		};
		void do_deallocate(void* p, ::ptrdiff_t bytes, ::ptrdiff_t alignment) {
			;
		};
		bool do_is_equal(const memory_resource& other) const noexcept {
			return false;
		}
		
		bool __allocates_globally = false;
		bool __deallocates_globally = false;
	};
	
}
