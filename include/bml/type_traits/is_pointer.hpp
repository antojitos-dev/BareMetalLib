//
// Copyright (c) 2019 Wee Loong Kuan
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
#include "true_type.hpp"
#include "false_type.hpp"
#include "remove_cv.hpp"

namespace bml
{
    namespace detail::is_pointer_detail
    {
        template <typename T> struct impl     : false_type {};
        template <typename T> struct impl<T*> : true_type  {};
    }
    
    //
    // See std::is_pointer.
    //
    template <typename T>
    struct is_pointer : detail::is_pointer_detail::impl<remove_cv_ty<T>> {};

    //
    // See std::is_pointer_v.
    //
    template <typename T>
    inline constexpr auto is_pointer_v = bool(is_pointer<T>::value);
}
