// Copyright (c) 2009-2021 The DigiByte Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DIGIBYTE_TEST_FUZZ_FUZZ_H
#define DIGIBYTE_TEST_FUZZ_FUZZ_H

#include <span.h>

#include <cstdint>
#include <functional>
#include <string_view>

using FuzzBufferType = Span<const uint8_t>;

using TypeTestOneInput = std::function<void(FuzzBufferType)>;
using TypeInitialize = std::function<void()>;
using TypeHidden = bool;

void FuzzFrameworkRegisterTarget(std::string_view name, TypeTestOneInput target, TypeInitialize init, TypeHidden hidden);

inline void FuzzFrameworkEmptyInitFun() {}

#define FUZZ_TARGET(name) \
    FUZZ_TARGET_INIT(name, FuzzFrameworkEmptyInitFun)

#define FUZZ_TARGET_INIT(name, init_fun) \
    FUZZ_TARGET_INIT_HIDDEN(name, init_fun, false)

#define FUZZ_TARGET_INIT_HIDDEN(name, init_fun, hidden)                               \
    void name##_fuzz_target(FuzzBufferType);                                          \
    struct name##_Before_Main {                                                       \
        name##_Before_Main()                                                          \
        {                                                                             \
            FuzzFrameworkRegisterTarget(#name, name##_fuzz_target, init_fun, hidden); \
        }                                                                             \
    } const static g_##name##_before_main;                                            \
    void name##_fuzz_target(FuzzBufferType buffer)

#endif // DIGIBYTE_TEST_FUZZ_FUZZ_H
