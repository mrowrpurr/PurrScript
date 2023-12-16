#define spec_name DLL_Loading

#include "SpecHelper.h"  // IWYU pragma: keep
//

#include <PurrScript/Packages/LibraryLoader.h>

UseTemplate("Context");

Test("can load global function from DLL") {
    auto* LibraryLoaderPackage = LibraryLoader::GetPackage();
    context()->ImportPackage(LibraryLoaderPackage);

    eval(R"(
        import("OneGlobalFunctionDLL")
    )");

    AssertOutputContains("Package 'OneGlobalFunctionDLL' not found");

    eval("say_hello_from_dll()");

    AssertOutputContains("attempt to call global 'say_hello_from_dll' (a nil value)");

    eval(R"(
        LibraryLoader.LoadSharedLibrary("DLL that Has One Global Function.dll");
    )");

    AssertOutputContains(R"(Loaded DLL: "DLL that Has One Global Function.dll")");

    eval(R"(
        import("OneGlobalFunctionDLL")
    )");

    auto output = ReadLogs();
    AssertThat(output, DoesNotContain("Package 'OneGlobalFunctionDLL' not found"));
    AssertThat(output, Contains("ImportPackage('OneGlobalFunctionDLL')"));

    eval("say_hello_from_dll()");

    output = ReadLogs();
    AssertThat(output, DoesNotContain("attempt to call global 'say_hello_from_dll' (a nil value)"));
    AssertThat(output, Contains("[print]Hello from DLL![/print]"));
}
