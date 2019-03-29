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

namespace bml
{
    //
    // See std::is_rvalue_reference.
    //
    template <typename T> struct is_rvalue_reference      : false_type {};
    template <typename T> struct is_rvalue_reference<T&&> : true_type  {};

    //
    // See std::is_rvalue_reference_v.
    //
    template <typename T>
    inline constexpr auto is_rvalue_reference_v = bool(is_rvalue_reference<T>::value);
}
