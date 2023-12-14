#define spec_name Invoke_Global_Print_Function

#include "SpecHelper.h"  // IWYU pragma: keep

UseTemplate("Context");

Test("invoke global print function") {
    auto* context = get_context();

    run_code(R"(
        print("Testing, testing, 1 2 3...")
    )");

    AssertThat(ReadLogs(), Contains("[print]Testing, testing, 1 2 3..."));
}
