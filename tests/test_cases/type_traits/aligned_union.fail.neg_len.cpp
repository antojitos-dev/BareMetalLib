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

#include <bml_testbench.hpp>
#include <bml/type_traits/aligned_union.hpp>

auto test_main() noexcept -> int
{
    // Check that a compile error occurs when Len is negative.
    {
        using T = typename bml::aligned_union<-2, int, double>::type;
    }

    return 0;
}
