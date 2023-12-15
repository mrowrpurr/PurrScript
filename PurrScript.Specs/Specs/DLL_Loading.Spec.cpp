#define spec_name DLL_Loading

#include "SpecHelper.h"  // IWYU pragma: keep
//

#include <PurrScript/Packages/DllLoader.h>

UseTemplate("Context");

Test("can load global function from DLL") {
    auto* dllLoaderPackage = DllLoader::GetPackage();
    get_context()->ImportPackage(dllLoaderPackage);

    run_code(R"(
        DllLoader.LoadDLL("DLL that Has One Global Function.dll");
    )");

    AssertThat(ReadLogs(), Contains("[print]?????"));
}
