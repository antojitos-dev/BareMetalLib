//
// Copyright (c) 2018 Wee Loong Kuan
//
// This file is licensed under the MIT license. For more details, see the LICENSE.md file in the
// top-level directory of this distribution.
//

#pragma once
#include "../../../bool_constant.hpp"

namespace bml::detail::is_trivially_copyable_detail
{
    template <typename T>
    struct impl : bool_constant<__is_trivially_copyable(T)> {};
}
