#pragma once

#include <PurrScript/API.h>

#ifdef _WIN32
    #define PurrScript_DLL extern "C" __declspec(dllexport) void PurrScript_Load
#else
    #define PurrScript_DLL extern "C" void PurrScript_Load
#endif
