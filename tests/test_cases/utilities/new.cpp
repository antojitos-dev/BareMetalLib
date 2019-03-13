//
// Copyright (c) 2019 Wee Loong Kuan
//
// This file is licensed under the MIT license. For more details, see the LICENSE.md file in the
// top-level directory of this distribution.
//

#include <bml_testbench.hpp>
#include <bml/type_traits/aligned_storage.hpp>
#include <bml/type_traits/is_same.hpp>
#include <bml/utilities/new.hpp>

struct x
{
    inline static auto ctor_invoked = false;
    inline static auto dtor_invoked = false;
    
    x() { ctor_invoked = true; }
    ~x() { dtor_invoked = true; }
};

auto test_main() noexcept -> int
{
    // Check that placement new can be invoked and returns a pointer to the right object type.
    {
        auto storage = bml::aligned_storage_ty<sizeof(x), alignof(x)>();
        
        auto p = ::new(&storage) x();
        static_assert(bml::is_same_v<decltype(p), x*>);
        bmltb_assert(x::ctor_invoked);
        bmltb_assert(!x::dtor_invoked);
        
        p->~x();
        bmltb_assert(x::ctor_invoked);
        bmltb_assert(x::dtor_invoked);
    }

    return 0;
}
