#define spec_name CollectPrintOutput

#include "SpecHelper.h"  // IWYU pragma: keep

TestGroup("Collect Print Output");

Test("invoke global function (print() function)") {
    PurrScriptSpecEnvironment env;
    auto*                     context = env.CreateContext();

    context->ExecuteCode(R"(
            print("Testing, testing, 1 2 3...")
        )");

    AssertThat(ReadLogs(), Contains("[print]Testing, testing, 1 2 3..."));
}
