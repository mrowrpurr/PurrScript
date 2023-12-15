#define spec_name DLL_Loading

#include "SpecHelper.h"  // IWYU pragma: keep
//

#include <PurrScript/Packages/DllLoader.h>

UseTemplate("Context");

Test("can load global function from DLL") {
    auto* dllLoaderPackage = DllLoader::GetPackage();
    context()->ImportPackage(dllLoaderPackage);

    eval("say_hello_from_dll()");

    AssertOutputContains("attempt to call global 'say_hello_from_dll' (a nil value)");

    eval(R"(
        DllLoader.LoadDLL("DLL that Has One Global Function.dll");
    )");

    AssertOutputContains(R"(Loaded DLL: "DLL that Has One Global Function.dll")");

    // AssertOutputDoesNotContain("attempt to call global 'say_hello_from_dll' (a nil value)");

    // eval("say_hello_from_dll()");

    // auto output = ReadLogs();
    // AssertThat(output, DoesNotContain("attempt to call global 'say_hello_from_dll' (a nil
    // value)"));

    // eval(R"(
    //     DllLoader.LoadDLL("DLL that Has One Global Function.dll");
    // )");

    // AssertOutputContains("[print]?????"));
}
