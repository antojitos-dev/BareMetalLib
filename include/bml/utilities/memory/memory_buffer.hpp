//
// Copyright (c) 2019 Charles Ricchio
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

#include "../detail/size_to_ptrdiff.hpp"
#include "../../containers/optional.hpp"

namespace bml {

	//
	// A typesafe access window into a statically allocated region of memory.
	// This is meant to encapsulate access to a region of memory for
	// compile-time definitions.
	//
	template<void *Addr, ::ptrdiff_t Size>
	struct memory_buffer {

		static_assert(Addr != nullptr,
			"memory_buffer cannot encapsulate a nullptr.");
		static_assert(Size > 0,
			"memory_buffer cannot encapsulate a negative or zero length buffer.");

		void *const base = Addr;
		const ::ptrdiff_t length;

		//
		// Provide typesafe access to the underlying memory region.
		//
		template<typename T>
		[[nodiscard]] optional<T &> at(::ptrdiff_t pos) {
			if ((pos / sizeof(T) >= length) {
				return;
			}

			return reinterpret_cast<T *const>(base)[pos];
		}

	};

}
