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
	struct monotonic_buffer_resource : public bml::memory_resource {
	public:
		// The baremetal version of this allocator can't be default constructed,
		// so we explicitly forbid it by deleting the constructor.
		monotonic_buffer_resource() = delete;

		explicit monotonic_buffer_resource(bml::memory_resource* upstream) {
			__upstream = upstream;
		};

		// The baremetal version of this constructor doesn't work, so we skip it.
		// explicit monotonic_buffer_resource(::ptrdiff_t) = delete;

		monotonic_buffer_resource(
			::ptrdiff_t initial_size,
			bml::memory_resource* upstream
		) {

			__upstream = upstream;
			if (__upstream != NULL) {
				__internal_buffer = __upstream->allocate(initial_size);
			}
			if (__internal_buffer != NULL) {
				__max_size = initial_size;
				__upstream_allocated_base = __internal_buffer;
			}
		};

		// This constructor is particularly relevant on baremetal. It can be
		// used to implement a top-level memory resource if given a large enough
		// starting size and buffer.
		monotonic_buffer_resource(void* buffer, ::ptrdiff_t buffer_size) {
			__internal_buffer = buffer;
			if (__internal_buffer != NULL) {
				__max_size = buffer_size;
			}

		};

		monotonic_buffer_resource(
			void* buffer,
			::ptrdiff_t buffer_size,
			bml::memory_resource* upstream
		) {
			__internal_buffer = buffer;
			__upstream = upstream;
			if (__internal_buffer != NULL) {
				__max_size = buffer_size;
			}
			else if (__upstream != NULL) {
				__internal_buffer = __upstream->allocate(buffer_size);
				if (__internal_buffer != NULL) {
					__max_size = buffer_size;
					__upstream_allocated_base = __internal_buffer;
				}
			}
		};

		// Monotonic buffers can't be copy constructed
		monotonic_buffer_resource(const monotonic_buffer_resource&) = delete;

		// Tell upstream that it should release any and all memory
		void release() {
			if (__upstream != NULL && __upstream_allocated_base != NULL) {
				__upstream->deallocate(__upstream_allocated_base, __max_size);
			}
		}

		// Cleanup upstream memory
		virtual ~monotonic_buffer_resource() {
			this->release();
		}

		bml::memory_resource* upstream_resource() const {
			return upstream;
		}

		monotonic_buffer_resource& operator=(const monotonic_buffer_resource&) = delete;

	private:
		// The base address of the region specified by a given
		// struct __alloc_block, `alloc_region`, is equal to:
		// (void*)(alloc_region) + sizeof(struct __alloc_block);
		//
		// The length field of the struct is the length of the region _after_
		// the base address
		struct __alloc_block {
			::ptrdiff_t length;
			struct __alloc_block* next;
		};

		void new_alloc_block(void* addr, ::ptrdiff_t length) {
			struct __alloc_block* block =
				reinterpret_cast<struct __alloc_block*>(addr);

			block->length = length;
			block->next = NULL;
		}

		void* do_allocate(::ptrdiff_t bytes, ::ptrdiff_t alignment) noexcept {
			void* ptr = NULL;

			// Check to determine if the region is allocated
			if (__current_size == 0) {

			}

			struct __alloc_block* block = __internal_buffer;
		};

		// For a monotonic buffer, this is a no-op.
		void do_deallocate(void* p, ::ptrdiff_t bytes, ::ptrdiff_t alignment) {
			;
		};
		// Monotonic buffers are not equal to any other memory_resource since
		// they cannot deallocate memory from other buffers
		bool do_is_equal(const memory_resource& other) const noexcept {
			return false;
		}

		bool __allocates_globally   = false;
		bool __deallocates_globally = false;

		bml::memory_resource* __upstream = NULL;
		void* __internal_buffer          = NULL;
		void* __upstream_allocated_base  = NULL;
		::ptrdiff_t __max_size           = 0;
		::ptrdiff_t __current_size       = 0;

		static ::ptrdiff_t page
	};

}
