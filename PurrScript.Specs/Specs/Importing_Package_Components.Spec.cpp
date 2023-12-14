#define spec_name Importing_Package_Components

#include "SpecHelper.h"  // IWYU pragma: keep

UseTemplate("Context");

Test("can import package and access its variables via namespace") {
    auto& somePackage   = get_env()->NewPackage("CoolThings");
    auto* coolNamespace = somePackage->DefineNamespaces({"Hello", "World"});
    somePackage->DefineGlobalFunction(
        "MyFunction", unique_function_pointer([](ScriptContext*, FunctionArguments*) -> Value* {
            _Log_("Called Hello.World.MyFunction()");
            return VoidValue();
        }),
        coolNamespace
    );

    run_code(R"(
        import("CoolThings")

        Hello.World.MyFunction()
    )");

    AssertThat(ReadLogs(), Contains("Called Hello.World.MyFunction()"));
}

Test("can 'include' a namespace to include all of its variables directly") {
    auto& somePackage   = get_env()->NewPackage("CoolThings");
    auto* coolNamespace = somePackage->DefineNamespaces({"Hello", "World"});
    somePackage->DefineGlobalFunction(
        "MyFunction", unique_function_pointer([](ScriptContext*, FunctionArguments*) -> Value* {
            _Log_("Called Hello.World.MyFunction()");
            return VoidValue();
        }),
        coolNamespace
    );

    run_code(R"(
        import("CoolThings")
        include("Hello")

        World.MyFunction()
    )");

    AssertThat(ReadLogs(), Contains("Called Hello.World.MyFunction()"));
}

Test("can 'include' a namespace to include all of its variables directly") {
    auto& somePackage   = get_env()->NewPackage("CoolThings");
    auto* coolNamespace = somePackage->DefineNamespaces({"Hello", "World"});
    somePackage->DefineGlobalFunction(
        "MyFunction", unique_function_pointer([](ScriptContext*, FunctionArguments*) -> Value* {
            _Log_("Called Hello.World.MyFunction()");
            return VoidValue();
        }),
        coolNamespace
    );

    run_code(R"(
        import("CoolThings")
        include("Hello.World")

        MyFunction()
    )");

    AssertThat(ReadLogs(), Contains("Called Hello.World.MyFunction()"));
}

Test("can 'include' and 'import' in the same line somehow") {
    auto& somePackage   = get_env()->NewPackage("CoolThings");
    auto* coolNamespace = somePackage->DefineNamespaces({"Hello", "World"});
    somePackage->DefineGlobalFunction(
        "MyFunction", unique_function_pointer([](ScriptContext*, FunctionArguments*) -> Value* {
            _Log_("Called Hello.World.MyFunction()");
            return VoidValue();
        }),
        coolNamespace
    );

    run_code(R"(
        include("Hello.World", { from = "CoolThings" })

        MyFunction()
    )");

    AssertThat(ReadLogs(), Contains("Called Hello.World.MyFunction()"));
}
