#define spec_name Invoke_Global_Print_Function

#include "SpecHelper.h"  // IWYU pragma: keep

UseTemplate("Context");

Test("invoke global print function") {
    auto* context = context();

    eval(R"(
        print("Testing, testing, 1 2 3...")
    )");

    AssertOutputContains("[print]Testing, testing, 1 2 3...");
}
