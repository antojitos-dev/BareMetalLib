//
// Copyright (c) 2019 Wee Loong Kuan
//
// This file is licensed under the MIT license. For more details, see the LICENSE.md file in the
// top-level directory of this distribution.
//

#pragma once
#include "is_referenceable.hpp"

namespace bml
{
    namespace detail::add_rvalue_reference_detail
    {
        template <typename T, bool = is_referenceable_v<T>>
        struct impl
        {
            using type = T;
        };
        
        template <typename T>
        struct impl<T, true>
        {
            using type = T&&;
        };
    }

    //
    // See std::add_rvalue_reference.
    //
    template <typename T>
    struct add_rvalue_reference : detail::add_rvalue_reference_detail::impl<T> {};

    //
    // See std::add_rvalue_reference_t, except that this is named add_rvalue_reference_ty for
    // POSIX compatibility.
    //
    template <typename T>
    using add_rvalue_reference_ty = typename add_rvalue_reference<T>::type;
}
