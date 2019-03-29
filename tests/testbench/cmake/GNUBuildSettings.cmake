#
# Copyright (c) 2018 Wee Loong Kuan
#
# BareMetalLib is based on libc++ (https://libcxx.llvm.org/).
# 
# This file is licensed under under the Apache License v2.0 with LLVM Exceptions. For more details,
# see the LICENSE.md file in the top-level directory of this distribution, or copy at 
# https://llvm.org/LICENSE.txt.
#
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
#

target_compile_options(bml_testbench PUBLIC
    -ffreestanding -nostdinc++ -fno-rtti -fno-exceptions -Wall -Wextra)

target_link_libraries(bml_testbench PUBLIC gcc -nostdlib)
