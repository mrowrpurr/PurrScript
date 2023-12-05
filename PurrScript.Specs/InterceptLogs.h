#pragma once

#include <string_format.h>

#include "LogCollector.h"  // IWYU pragma: keep

#define __LOG_1(a) LogCollector::Get().Append(string_format("{}", a))
#define __LOG_2(a, b) LogCollector::Get().Append(string_format(a, b))
#define __LOG_3(a, b, c) LogCollector::Get().Append(string_format(a, b, c))
#define __LOG_4(a, b, c, d) LogCollector::Get().Append(string_format(a, b, c, d))

#define __LOG_EXPAND(x) x
#define __LOG_GET_MACRO(_1, _2, _3, _4, NAME, ...) NAME
#define _Log_(...) \
    __LOG_EXPAND(__LOG_GET_MACRO(__VA_ARGS__, __LOG_4, __LOG_3, __LOG_2, __LOG_1)(__VA_ARGS__))
