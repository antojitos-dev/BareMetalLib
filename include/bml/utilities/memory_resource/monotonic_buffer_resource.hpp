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

#include <stdint.h>
#include "memory_resource_base.hpp"

namespace bml {

	// Memory resource which doesn't free until its destruction
	template <typename T = void>
	struct monotonic_buffer_resource : public bml::memory_resource {
	public:
		// The baremetal version of this allocator can't be default constructed,
		// so we explicitly forbid it by deleting the constructor.
		monotonic_buffer_resource() = delete;

		explicit monotonic_buffer_resource(bml::memory_resource* upstream) {
			__upstream = upstream;
		};

		// The baremetal version of the size-only constructor doesn't work,
		// so we skip it.
		// explicit monotonic_buffer_resource(::ptrdiff_t) = delete;

		monotonic_buffer_resource(
			::ptrdiff_t initial_size,
			bml::memory_resource* upstream
		) {
			// Require at least enough memory to allocate the starting block
			if (initial_size < sizeof(struct __alloc_block)) {
				initial_size = sizeof(struct __alloc_block);
			}

			// Set the upstream and get enough memory for the initial size
			__upstream = upstream;
			if (__upstream != NULL) {
				__internal_buffer = __upstream->allocate(initial_size);
			}

			// If the buffer was allocated successfully, finish initialization
			if (__internal_buffer != NULL) {
				this->new_alloc_block(
					__internal_buffer,
					initial_size - sizeof(struct __alloc_block)
				);
				__end_block = static_cast<struct __alloc_block*>(__internal_buffer);
			}
		};

		// This constructor is particularly relevant on baremetal. It can be
		// used to implement a top-level memory resource if given a large enough
		// starting size and buffer.
		monotonic_buffer_resource(void* buffer, ::ptrdiff_t buffer_size) {

			__internal_buffer = buffer;

			// If there isn't enough space for a block, set the init up to fail
			if (buffer_size < sizeof(struct __alloc_block)) {
				__internal_buffer = NULL;
			}

			if (__internal_buffer != NULL) {
				this->new_alloc_block(
					__internal_buffer,
					buffer_size - sizeof(struct __alloc_block)
				);
				__end_block = static_cast<struct __alloc_block*>(__internal_buffer);
			}
		};

		monotonic_buffer_resource(
			void* buffer,
			::ptrdiff_t buffer_size,
			bml::memory_resource* upstream
		) {
			__internal_buffer = buffer;

			// If there isn't enough space for a block, ignore the buffer
			if (buffer_size < sizeof(struct __alloc_block)) {
				__internal_buffer = NULL;
			}
			else {
				this->new_alloc_block(
					__internal_buffer,
					buffer_size - sizeof(struct __alloc_block)
				);
				__end_block = static_cast<struct __alloc_block*>(__internal_buffer);
			}

			// Set the upstream resource
			__upstream = upstream;

		};

		// Monotonic buffers can't be copy constructed
		monotonic_buffer_resource(const monotonic_buffer_resource&) = delete;

		// Tell upstream that it should release any and all memory
		void release() {
			if (__upstream != NULL) {
				// Iterate over each block and release it
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
			size_t flags;
			::ptrdiff_t reserved;
		};

		constexpr size_t __BLOCK_USED = 1 << 0;

		void new_alloc_block(void* addr, ::ptrdiff_t length) {
			struct __alloc_block* block =
				static_cast<struct __alloc_block*>(addr);

			block->length = length;
			block->next = NULL;
		}

		void* do_allocate(::ptrdiff_t bytes, ::ptrdiff_t alignment) noexcept {
			void* ptr = NULL;

			// Check to determine if the first region needs to be initialized
			if (__internal_buffer == NULL) {

				// If there's no upstream, we can't allocate any more memory
				if (__upstream == NULL) {
					return NULL;
				}

				__internal_buffer = __upstream->allocate(this->__get_alloc_size());

				if (__internal_buffer == NULL) {
					return NULL;
				}
				this->new_alloc_block(
					__internal_buffer,
					this->__get_alloc_size() - sizeof(struct __alloc_block)
				);
				__end_block = static_cast<struct __alloc_block*>(__internal_buffer);
			}

			struct __alloc_block* block =
				static_cast<struct __alloc_block*>(__internal_buffer);

			struct __alloc_block* selected = NULL;

			// Iterate over blocks until we find a sufficiently large one
			while (block->next != NULL) {
				// If this block is free...
				if (block->flags & __BLOCK_USED == 0) {
					if (block->length > (bytes + alignment)) {
						selected = block;
						break;
					}
				}
				block = block->next;
			}

			// If we didn't find a valid block, we need to allocate more memory.
			if (selected == NULL) {
				// Figure out how many times the normal allocation size is
				// necessary to fit the region.
				::ptrdiff_t multiple = 1;
				multiple += (bytes + alignment +
					sizeof(struct __alloc_block)) / this->__get_alloc_size();
				__end_block->next = __upstream->allocate(
					this->__get_alloc_size() * multiple
				);

				// If the new end block is NULL, allocation has failed.
				if (__end_block->next == NULL) {
					return NULL;
				}

				this->new_alloc_block(
					__end_block->next, this->__get_alloc_size() * multiple
				);

				__end_block = __end_block->next;
				selected = __end_block;
			}
			
			// Mark the block as occupied
			selected->flags |= __BLOCK_USED;

			// Get the base address of the block
			::intptr_t base = reinterpret_cast<::intptr_t>(selected);
			selected += sizeof(struct __alloc_block);

			// Align the address
			base += (base % alignment) ? alignment - (base % alignment) : 0;

			// If there's enough memory left, split the block before returning
			::ptrdiff_t remaining = selected->length - (bytes + alignment);
			if (remaining >= __BLOCK_SPLIT_SIZE) {
				::intptr_t split_base = base + bytes + alignment;
				this->new_alloc_block(
					split_base, 
					selected->length - (bytes + alignment + sizeof(struct __alloc_block))
				);
				struct __alloc_block* old_next = selected->next;
				struct __alloc_block* split_block =
					reinterpret_cast<struct __alloc_block*>(split_base);
				split_block->next = old_next;
				selected->next = split_block;
			}
			
			return base;
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
		
		// Minimum required space to justify a block split
		constexpr ::ptrdiff_t __BLOCK_SPLIT_SIZE = 512 + sizeof(struct __alloc_block);
		bml::memory_resource* __upstream  = NULL;
		void* __internal_buffer           = NULL;
		struct __alloc_block* __end_block = NULL;

	};

}
