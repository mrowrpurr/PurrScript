#define spec_name CollectPrintOutput

#include "SpecHelper.h"  // IWYU pragma: keep

UseTemplate("Context");

Test("invoke global print function") {
    auto* context = get_context();

    run_code(R"(
        print("Testing, testing, 1 2 3...")
    )");

    AssertThat(ReadLogs(), Contains("[print]Testing, testing, 1 2 3..."));
}
