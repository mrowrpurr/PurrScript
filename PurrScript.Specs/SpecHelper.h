#pragma once

#include "SpecHelpers/InterceptLogs.h"  // IWYU pragma: keep
// Include before everything else to override _Log_ to collect all logs from PurrScript (etc)

// #include <Shared Example Struct Implementations.h>
#include <Shared Example Struct Implementations.h>  // IWYU pragma: keep
#include <Specs.h>
#include <Specs/Snowhouse.h>
#include <string_format.h>

#include <iostream>  // IWYU pragma: keep

#include "SpecHelpers/LogCollector.h"           // IWYU pragma: keep
#include "SpecHelpers/PurrScriptEnvironment.h"  // IWYU pragma: keep

using namespace std;
using namespace PurrScript;
using namespace PurrScript::Packages;
using namespace PurrScript::Implementations;
using namespace PurrScript::Implementations::Lua;
using namespace PurrScript::Implementations::Factories;

#define get_env() current_spec->var<PurrScriptSpecEnvironment*>("env")
#define get_context() current_spec->var<ScriptContext*>("context")
#define run_code(code) get_context()->ExecuteCode(code)

#define __PRINT_1(a) std::cout << string_format("{}", a) << std::endl
#define __PRINT_2(a, b) std::cout << string_format(a, b) << std::endl
#define __PRINT_3(a, b, c) std::cout << string_format(a, b, c) << std::endl
#define __PRINT_4(a, b, c, d) std::cout << string_format(a, b, c, d) << std::endl

#define __PRINT_EXPAND(x) x
#define __PRINT_GET_MACRO(_1, _2, _3, _4, NAME, ...) NAME
#define Print(...)                                                                              \
    __PRINT_EXPAND(                                                                             \
        __PRINT_GET_MACRO(__VA_ARGS__, __PRINT_4, __PRINT_3, __PRINT_2, __PRINT_1)(__VA_ARGS__) \
    )

inline string ReadLogs() { return LogCollector::Get().ToString(); }
